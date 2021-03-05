#include "StringTokenizer.h"
#include <malloc.h>
const int MODE = _SIDD_UBYTE_OPS | _SIDD_CMP_EQUAL_EACH | _SIDD_LEAST_SIGNIFICANT;
const int SIMD_LANES = 16;

StringTokenizer::StringTokenizer(char separator, size_t initialSize)
{
	this->initialSize = initialSize;
	this->bufferStart = (char*)_aligned_malloc(initialSize, SIMD_LANES);
	loadSeparator(separator);
}

StringTokenizer::StringTokenizer(char separator)
{
	this->initialSize = -1;
	this->bufferStart = NULL;
	loadSeparator(separator);
}

void StringTokenizer::setString(char* string)
{
	auto len = strlen(string);
	if (initialSize != -1 && len > initialSize)
	{
		if (len > initialSize)
		{
			//try to reuse buffer if same size or less...
			_aligned_free(bufferStart);
			bufferStart = bufferCurrentPtr = (char*)_aligned_malloc(len + 1, SIMD_LANES);
			initialSize = len + 1;
		}
		strcpy(bufferStart, string);
	}
	else
	{
		bufferStart = string;
	}
	bufferCurrentPtr = bufferStart;
	bufferLimitPtr = bufferStart + len;
	simdBufferCurrentPtr = (const __m128i*)bufferStart;
}
char * StringTokenizer::nextToken()
{
	char * tokenStart = NULL;
	if (bufferCurrentPtr < bufferLimitPtr)
	{
		tokenStart = bufferCurrentPtr;
		while (bufferCurrentPtr < bufferLimitPtr)
		{
			if (maskBitShift == 0){
				//load xmm register and calculate mask
				__m128i simdCache = _mm_load_si128(simdBufferCurrentPtr);
				bufferSimdCurrentPtr = (char*)simdBufferCurrentPtr;
				maskResult = _mm_cmpestrm(simdCache, SIMD_LANES, simdSeparator, SIMD_LANES, MODE).m128i_u64[0];
			}

			if (maskResult == 0 && (bufferCurrentPtr + SIMD_LANES  < bufferLimitPtr))
			{
				//No separator found, advance the pointer
				simdBufferCurrentPtr++;
				maskBitShift = 0;
				bufferCurrentPtr = bufferSimdCurrentPtr + SIMD_LANES;
				
			}
			else 
			{
				//separator found, return the token:
				for (maskBitShift++ ; maskBitShift < SIMD_LANES; maskBitShift++)
				{
					if (maskResult & (1 << maskBitShift))
					{
						bufferSimdCurrentPtr[maskBitShift] = '\0';
						bufferCurrentPtr = bufferSimdCurrentPtr + maskBitShift + 1;
						return tokenStart;
					}
				}
				if (maskBitShift >= SIMD_LANES){
					simdBufferCurrentPtr++;
					maskBitShift = 0;
					bufferCurrentPtr = bufferSimdCurrentPtr + SIMD_LANES;
				}
			}
		}
	}
	return tokenStart;
}

void StringTokenizer::loadSeparator(char separator)
{
	simdSeparator.m128i_u8[0] = simdSeparator.m128i_u8[1] = simdSeparator.m128i_u8[2] = simdSeparator.m128i_u8[3] = simdSeparator.m128i_u8[4] = simdSeparator.m128i_u8[5]
		= simdSeparator.m128i_u8[6] = simdSeparator.m128i_u8[7] = simdSeparator.m128i_u8[8] = simdSeparator.m128i_u8[9] = simdSeparator.m128i_u8[10] = simdSeparator.m128i_u8[11]
		= simdSeparator.m128i_u8[12] = simdSeparator.m128i_u8[13] = simdSeparator.m128i_u8[14] = simdSeparator.m128i_u8[15] = separator;
}

StringTokenizer::~StringTokenizer()
{
	if (bufferStart)
	{
		_aligned_free(bufferStart);
	}
}
