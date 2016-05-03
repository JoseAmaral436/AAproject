#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <linux/random.h>

int main() {
	int seed;
	FILE *f;
	f = fopen("/dev/urandom", "r");
	fread(&seed, sizeof(seed), 1, f);
	fclose(f);
	srand(seed);
	int max = 100;
	int i;
	int number;
	printf("%d\n", max);
	for (i = 0; i < max-1; i++){
		number = rand() & 0xffffff;
		printf("A %06x\n", number);
	}
	number = rand() & 0xffffff;
	printf("A %06x", number);
	return 0;
}