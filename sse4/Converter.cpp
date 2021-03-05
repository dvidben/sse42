#include "Converter.h"

__declspec(align(16)) const float floatArray[8] { 10000000, 1000000, 100000, 10000, 1000, 100, 10, 1  };
__declspec(align(16)) const float floatArray2[4] { 0.1, 0.01, 0.001, 0.0001};
__declspec(align(16)) const float baseArray[4] { '0', '0', '0', '0' };

__m128 Converter::simdMultiplier = _mm_load_ps(floatArray);

Converter::Converter(char * str)
{
	this->str = str;
	char * strStart = str;
	char * bufferStart = this->buffer;
	while (strStart && *strStart > '0' && *strStart < '9')
	{
		*bufferStart++ = *strStart++;
	}
	*bufferStart = NULL;
	auto len = strlen(this->buffer);
	auto offset = 5 - len;
	//simdMultiplier = _mm_load_ps(floatArray1);
	simdBase = _mm_load_ps(baseArray);
}

int Converter::toInt()
{
	auto len = strlen(this->buffer);
	char * bufferStart = this->buffer;
	const float * simdCacheStart = &floatArray[8-len];
	__m128 simdCache, simdMultiplierCache;
	
	for (int i = 0; i < len; i+=4)
	{
		floatCache[0] = *bufferStart++;
		floatCache[1] = *bufferStart++;
		floatCache[2] = *bufferStart++;
		floatCache[3] = *bufferStart++;

		simdCache = _mm_load_ps(floatCache);
		simdMultiplierCache = _mm_load_ps(simdCacheStart);
		__m128 sub = _mm_sub_ps(simdCache, simdBase);
		__m128 result = _mm_dp_ps(simdMultiplierCache, sub, 241);

		return result.m128_f32[0];

	}
	
	
	
	



}

Converter::~Converter()
{
}
