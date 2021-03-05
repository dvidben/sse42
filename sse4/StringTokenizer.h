#pragma once
#include <nmmintrin.h>
#include <emmintrin.h>
#include <string.h>


class StringTokenizer
{
private:
	char* bufferStart;
	char* bufferCurrentPtr;
	char* bufferSimdCurrentPtr;
	char* bufferLimitPtr;

	__m128i simdSeparator;
	const __m128i* simdBufferCurrentPtr;
	size_t initialSize;
	size_t maskBitShift;
	unsigned long maskResult;

	void loadSeparator(char separator);

public:
	StringTokenizer(char separator, size_t initialSize);
	StringTokenizer(char separator);
	void setString(char string[]);
	char * nextToken();
	~StringTokenizer();
};

