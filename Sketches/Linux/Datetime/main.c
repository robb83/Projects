// gcc -Wall -O2 -o test rfc3339.c main.c
#include <stdio.h>
#include <stdlib.h>
#include "rfc3339.h"

int main(int argc, char** argv) {
	
	printf("%d *\n", (int)time(NULL));
	printf("%d\n", (int)rfc3339("2014-07-01T21:50:00Z"));
	printf("%d\n", (int)rfc3339("2014-07-01T21:50:00+00:00"));
	printf("%d\n", (int)rfc3339("2014-07-01T22:50:00+01:00"));
	printf("%d\n", (int)rfc3339("2014-07-01T23:50:00+02:00"));
	
	return 0;
}
