#include <stdio.h>
#include <nmmintrin.h>
#include <emmintrin.h>
#include "StringTokenizer.h"
#include "Converter.h"
#include <intrin.h>
#include <stdint.h>

uint64_t rdtsc();

int main()
{

	char* intStr = (char*)_aligned_malloc(10, 16);
	strcpy(intStr, "1245");

	char* strings[7];
	char * token;
	/*char strings[7][128];*/
	strings[0] = (char*)_aligned_malloc(128, 16);
	strings[1] = (char*)_aligned_malloc(128, 16);
	strings[2] = (char*)_aligned_malloc(128, 16);
	strings[3] = (char*)_aligned_malloc(128, 16);
	strings[4] = (char*)_aligned_malloc(128, 16);
	strings[5] = (char*)_aligned_malloc(128, 16);
	strings[6] = (char*)_aligned_malloc(128, 16);

	strcpy(strings[0], "hello1 world1 hello2 world3 hello3 world4 hello4 world5 hello5 world6");
	strcpy(strings[1], "hello1hello2hello3 world1world2world3 world1world2world3 hello1hello2hello3");
	strcpy(strings[2], "he llo he llo wo rld wo rld he llo he llo wo rld wo rld he llo he llo wo rld wo rld");
	strcpy(strings[3], "hello1hello2 hello1hello2 hello1hello2 hello1hello2 hello1hello2 hello1hello2");
	strcpy(strings[4], "hello1hello2hello3 h hello1hello2hello3 f hello1hello2hello3 g hello1hello2hello3 i");
	strcpy(strings[5], "hello1hello2hello3hello1hello2hello3hello1hello2hello3 hello1hello2hello3hello1hello2hello3hello1hello2hello3");
	strcpy(strings[6], "he llo hello1 he llo wo hello1 rld wo hello1 rld he llo hello1 he llo wo hello1 rld wo rld he llo hello1 he llo wo rld wo rld");

	int dummy = 0;
	Converter converter(intStr);
	int x = converter.toInt();

	uint64_t start, end, duration1, duration2;
	/*-----------------------------------------*/
	start = rdtsc();
	//char buffer[128];
	for (int i = 0; i < 1000000; i++)
	{
		//strcpy(buffer, strings[i % 7]);
		strtok(strings[i % 7], " ");
		while ((token = strtok(NULL, " ")) != NULL){
			dummy=0;
		}
	}
	end = rdtsc();
	duration1 = end - start;
	printf_s("STANDARD %i\n", duration1);
	/*-----------------------------------------*/
	start = rdtsc();
	StringTokenizer tokenizer(' ');
	for (int i = 0; i < 1000000; i++)
	{
		tokenizer.setString(strings[i%7]);
		while ((token = tokenizer.nextToken()) != NULL){
			dummy = 0;
		}
	}
	 end = rdtsc();
	 duration2 = (end - start);
	printf_s("SIMD---- %i\n", duration2);
	printf_s("Speedup = %f\n", duration1*1.0 / duration2);
	int j = getchar();
	/*-----------------------------------------*/
	
	int i = getchar();

	return 0;
}

uint64_t rdtsc(){
	return __rdtsc();
}