#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>
#include <math.h>

char test_string[] = "PAYPALISHIRINEFMEKMGEKWPLGG";
#define TEST_R 5

typedef enum _DIR_STATE_EM_ { DOWN = 0, UP_RIGHT = 1 } dir_state_em;

char *convert(char *s, int numRows)
{
	int length = strlen(s) + 1;
	if (length < numRows)
		return s;

	/* build the string table, numRows X (strlen(s) / 2) */
	char **table = (char **)malloc(numRows * sizeof(char *));
	for (int i = 0; i < numRows; i++) {
		table[i] = (char *)malloc(length / 2 * sizeof(char));
		memset(table[i], 0, length / 2 * sizeof(char));
	}

	/* New string */
	char *s_result = (char *)malloc(length * sizeof(char));
	memset(s_result, 0, length * sizeof(char));

	int i_s = 0;
	int r = 0;
	int c = 0;
	dir_state_em state = DOWN;
	int count = strlen(s);
	while (count > 0) {
		table[r][c] = s[i_s++];

		switch (state) {
		case DOWN:
			r++;
			if (r >= numRows) {
				r--;
				r--;
				c++;
				state = UP_RIGHT;
			}
			break;
		case UP_RIGHT:
			r--;
			c++;
			if (r < 0) {
				r++;
				r++;
				c--;
				state = DOWN;
			}
			break;
		}
		count--;
	}

	/* Print the table */
	i_s = 0;
	for (int i = 0; i < numRows; i++) {
		for (int j = 0; j < length / 2; j++) {
			if (table[i][j] != 0) {
				printf("%c, ", table[i][j]);
				s_result[i_s++] = table[i][j];
			} else {
				printf(" , ");
			}
		}
		printf("\n");
	}

	return s_result;
}

int main(int argc, char *argv[])
{
	// for(int i=0;i<argc;i++) {
	//     printf("arg[%d]: %s\n", i, argv[i]);
	// }

	printf("string: %s\n", test_string);
	printf("result: %s\n", convert(test_string, TEST_R));
}
