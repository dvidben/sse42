#pragma once
#include <nmmintrin.h>
#include <smmintrin.h>
#include <string.h>

class Converter
{
private:
	char * str;
	static __m128 simdMultiplier;
	__m128 simdBase;
	__declspec(align(16))  float floatCache[4];
	char buffer[15];
public:
	int toInt();
	Converter(char *str);
	~Converter();
};

