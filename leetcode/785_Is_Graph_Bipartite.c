#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>
#include <math.h>

/* ans: 0 */
// int r1[3] = {1, 2, 3};
// int r2[2] = {0, 2};
// int r3[3] = {0, 1, 3};
// int r4[2] = {0, 2};

/* ans: 1 */
// int r1[2] = {1, 3};
// int r2[2] = {0, 2};
// int r3[2] = {1, 3};
// int r4[2] = {0, 2};

/* ans: 1 */
// int r1[] = {3};
// int r2[] = {2, 4};
// int r3[] = {1};
// int r4[] = {0, 4};
// int r5[] = {1, 3};

/* ans: 1 */
int r1[] = { 1 };
int r2[] = { 0 };
int r3[] = { 4 };
int r4[] = { 4 };
int r5[] = { 2, 3 };

/* ans: 1 */
// int r1[] = {1};
// int r2[] = {0, 3};
// int r3[] = {3};
// int r4[] = {1, 2};

#define R_SIZE(x) (sizeof(r##x) / sizeof(int))

// int *test_graph[] = {
//     r1, r2, r3, r4
// };

int *test_graph[] = { r1, r2, r3, r4, r5 };

#define TEST_LEN (sizeof(test_graph) / sizeof(int *))

typedef enum queue_er_em {
	QUEUE_OK = 0,
	QUEUE_EMPTY = 1,
	QUEUE_FULL = 2,
	QUEUE_ERROR = 0xFF
} queue_er_em;

typedef int queue_data_t;
#define QUEUE_BUF_SIZE 100

typedef struct _queue_t {
	queue_data_t value[QUEUE_BUF_SIZE];
	int offer_i;
	int poll_i;
	int count;
} queue_t;

queue_er_em offer_queue(queue_t *queue, queue_data_t *tdata)
{
	if (queue->count >= QUEUE_BUF_SIZE)
		return QUEUE_ERROR;
	memcpy(&queue->value[queue->offer_i++], tdata, sizeof(queue_data_t));
	if (queue->offer_i == QUEUE_BUF_SIZE)
		queue->offer_i = 0;
	queue->count++;
	return QUEUE_OK;
}

queue_er_em poll_queue(queue_t *queue, queue_data_t *rdata)
{
	if (queue->count < 1)
		return QUEUE_EMPTY;
	memcpy(rdata, &queue->value[queue->poll_i++], sizeof(queue_data_t));
	if (queue->poll_i == QUEUE_BUF_SIZE)
		queue->poll_i = 0;
	queue->count--;
	return QUEUE_OK;
}

inline int get_queue_data_count(queue_t *queue)
{
	return queue->count;
}

bool is_queue_empty(queue_t *queue)
{
	return !(queue->count != 0);
}

typedef enum _color_em {
	UNVISITED = 0,
	BLACK = 1,
	RED = 2,
} color_em;

bool isBipartite(int **graph, int graphSize, int *graphColSize)
{
	queue_t *queue = malloc(sizeof(queue_t));
	memset(queue, 0, sizeof(queue_t));

	color_em *colors = malloc(graphSize * sizeof(color_em));
	memset(colors, 0, graphSize * sizeof(color_em));

	for (int i = 0; i < graphSize; i++) {
		if (colors[i] != UNVISITED)
			continue;
		printf("=================\n");
		colors[i] = RED;
		offer_queue(queue, &i);
		while (!is_queue_empty(queue)) {
			int cur = -1;
			(void)poll_queue(queue, &cur);
			assert(cur != -1);

			int cur_color = colors[cur];
			int adj_color = colors[cur] == RED ? BLACK : RED;

			printf("cur color[%d] %d\n", cur, cur_color);
			printf("adj color: %d\n", adj_color);

			for (int c = 0; c < graphColSize[cur]; c++) {
				int i_neighbor = graph[cur][c];
				if (colors[i_neighbor] == UNVISITED) {
					colors[i_neighbor] = adj_color;
					offer_queue(queue, &i_neighbor);
				} else if (colors[i_neighbor] != adj_color) {
					return false;
				}
				printf("color[%d]: %d\n", i_neighbor,
				       colors[i_neighbor]);
			}
		}
	}

	return true;
}

int main(int argc, char *argv[])
{
	// for(int i=0;i<argc;i++) {
	//     printf("arg[%d]: %s\n", i, argv[i]);
	// }

	int *col = malloc(TEST_LEN * sizeof(int));
	col[0] = R_SIZE(1);
	col[1] = R_SIZE(2);
	col[2] = R_SIZE(3);
	col[3] = R_SIZE(4);
	col[4] = R_SIZE(5);

	printf("result: %d\n", isBipartite(test_graph, TEST_LEN, col));
}
