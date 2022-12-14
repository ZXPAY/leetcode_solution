#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>
#include <math.h>

// int test_array[] = {2,7,4,1,8,1}; // ans: 1
int test_array[] = { 9, 3, 2, 10 }; // ans: 0
#define ARRAY_SIZE (sizeof(test_array) / sizeof(int))

void get_two_max(int *stones, int stonesSize, int *i_max, int *i_sec_max)
{
	int temp = INT32_MIN;
	for (int i = 0; i < stonesSize; i++) {
		if (temp < stones[i]) {
			temp = stones[i];
			*i_max = i;
		}
	}
	int temp_sec = INT32_MIN;
	for (int i = 0; i < stonesSize; i++) {
		if (i == *i_max)
			continue;
		if (temp_sec < stones[i]) {
			temp_sec = stones[i];
			*i_sec_max = i;
		}
	}
}

int lastStoneWeight(int *stones, int stonesSize)
{
	int count = stonesSize;
	int i_max = 0, i_sec_max = 0;
	while (count > 1) {
		get_two_max(stones, stonesSize, &i_max, &i_sec_max);
		// printf("%d, %d\n", stones[i_max], stones[i_sec_max]);
		stones[i_sec_max] = stones[i_max] - stones[i_sec_max];
		stones[i_max] = 0;
		for (int i = 0; i < stonesSize; i++) {
			printf("%d, ", stones[i]);
		}
		printf("\n");
		count--;
	}

	return stones[i_sec_max];
}

int main(int argc, char *argv[])
{
	// for(int i=0;i<argc;i++) {
	//     printf("arg[%d]: %s\n", i, argv[i]);
	// }

	printf("result: %d\n", lastStoneWeight(test_array, ARRAY_SIZE));
}
