/* This example is GCC platforms available ONLY! */
#include <stdio.h>
#include "num_to_ascii.h"

int main(int argc, char const *argv[])
{
	char str[36];
	uint32_t bin = 0xAAAAAAAA;
	numberToASCIIBin(bin, str, sizeof(str));
	printf("%s", str);
	numberToASCIIDec(bin, str, sizeof(str));
	printf("%s", str);
	return 0;
}
