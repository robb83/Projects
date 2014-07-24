// gcc -std=c99 -O2 -Wall -o c_prime c_prime.c -lm
#include <stdio.h>
#include <stdint.h>
#include <math.h>

uint8_t is_prime(int32_t number) {
	if (number < 10) {
		
		if (number == 0 || number == 1 || number == 9) 
			return 0;
		if (number == 2 || number == 3 || number == 5 || number == 7)
			return 1;
	}
	
	if (number % 2 == 0) return 0;
	if (number % 5 == 0) return 0;
	
	int32_t limit = sqrt(number);
	for( int32_t i = 3; i <= limit; i += 2) {
		if ((number % i) == 0) {
			return 0;
		}
	}
	
	return 1;
}

int main(int argc, char **argv) {
	int32_t match = 0;
	
	for(int32_t i = 0; i < 5000011; ++i) {
		if (is_prime(i))
			++match;
	}
	
	printf("%d\n", match);
	
	return 0;
}
