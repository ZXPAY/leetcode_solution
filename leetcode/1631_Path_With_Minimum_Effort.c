#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>
#include <math.h>

/* ans: 2 */
// int test_heights[3][3] = {{1, 2, 2},{3, 8, 2},{5, 3, 5}};

/* ans: 0 */
int test_heights[5][5] = { { 1, 2, 1, 1, 1 },
			   { 1, 2, 1, 2, 1 },
			   { 1, 2, 1, 2, 1 },
			   { 1, 2, 1, 2, 1 },
			   { 1, 1, 1, 2, 1 } };

/* ans: 1 */
// int test_heights[3][3] = {{1,2,3},{3,8,4},{5,3,5}};

#define M (sizeof(test_heights) / sizeof(test_heights[0]))
#define N (sizeof(test_heights) / M / sizeof(int))

typedef struct {
	int row;
	int col;
} pair_t;

typedef pair_t queue_data_t;

typedef enum queue_er_em {
	QUEUE_OK = 0,
	QUEUE_EMPTY = 1,
	QUEUE_FULL = 2,
	QUEUE_ERROR = 0xFF
} queue_er_em;

typedef struct _queue_t {
	queue_data_t *value;
	int buf_size;
	int offer_i;
	int poll_i;
	int count;
} queue_t;

queue_t *create_queue(int buf_size)
{
	queue_t *queue = malloc(sizeof(queue_t));
	memset(queue, 0, sizeof(queue_t));
	queue->buf_size = buf_size;
	queue->value = malloc(buf_size * sizeof(queue_data_t));
	memset(queue->value, 0, buf_size * sizeof(queue));

	return queue;
}

queue_er_em offer_queue(queue_t *queue, queue_data_t *tdata)
{
	if (queue->count >= queue->buf_size)
		return QUEUE_ERROR;
	memcpy(&queue->value[queue->offer_i++], tdata, sizeof(queue_data_t));
	if (queue->offer_i == queue->buf_size)
		queue->offer_i = 0;
	queue->count++;
	return QUEUE_OK;
}

queue_er_em poll_queue(queue_t *queue, queue_data_t *rdata)
{
	if (queue->count < 1)
		return QUEUE_EMPTY;
	memcpy(rdata, &queue->value[queue->poll_i++], sizeof(queue_data_t));
	if (queue->poll_i == queue->buf_size)
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

int minimumEffortPath(int **heights, int heightsSize, int *heightsColSize)
{
	int m = heightsSize;
	int n = *heightsColSize;
	queue_t *queue = create_queue(m * n);
	int **efforts = malloc(sizeof(int *) * m);
	for (int i = 0; i < m; i++) {
		efforts[i] = malloc(n * sizeof(int));
		for (int k = 0; k < n; k++)
			efforts[i][k] = INT32_MAX;
	}

	/* Record previous position */
	pair_t **prev_position = malloc(sizeof(pair_t *) * m);
	for (int i = 0; i < m; i++) {
		prev_position[i] = malloc(n * sizeof(pair_t));
		memset(prev_position[i], 0, n * sizeof(pair_t));
	}

	// left, up, right, down
	int dirx[4] = { -1, 0, 1, 0 };
	int diry[4] = { 0, -1, 0, 1 };

	pair_t node = {
		.row = 0,
		.col = 0,
	};

	efforts[0][0] = 0;
	(void)offer_queue(queue, &node);

	/* Dijsktra algorithm */
	while (!is_queue_empty(queue)) {
		(void)poll_queue(queue, &node);

		// pos_now
		int cur_r = node.row;
		int cur_c = node.col;
		int cur_effort = efforts[cur_r][cur_c];

		// printf("[%d, %d]: %d\n", cur_r, cur_c, cur_effort);

		assert(cur_r >= 0 && cur_r < m);
		assert(cur_c >= 0 && cur_c < n);

		// go left, right, up or down
		for (int i = 0; i < 4; i++) {
			int nr = cur_r + dirx[i];
			int nc = cur_c + diry[i];

			if (nr < 0 || nc < 0 || nr >= m || nc >= n)
				continue;

			// effort = height_now - height_dir
			int new_effort =
				heights[cur_r][cur_c] - heights[nr][nc];
			if (new_effort < 0)
				new_effort = -new_effort;

			int max_effort = new_effort > cur_effort ? new_effort :
								   cur_effort;

			if (max_effort < efforts[nr][nc]) {
				efforts[nr][nc] = max_effort;
				prev_position[nr][nc].row = cur_r;
				prev_position[nr][nc].col = cur_c;
				node.row = nr;
				node.col = nc;
				offer_queue(queue, &node);
			}
		}
	}

	/* Print the path that is the miminal effort */
	// last position to start position
	int p_r = m - 1;
	int p_c = n - 1;
	printf("(%d,%d)->", p_r, p_c);
	while (p_r != 0 || p_c != 0) {
		int temp = p_r;
		p_r = prev_position[p_r][p_c].row;
		p_c = prev_position[temp][p_c].col;
		printf("(%d,%d)->", p_r, p_c);
	}
	printf("Complete\n");

	return efforts[m - 1][n - 1];
}

int main(int argc, char *argv[])
{
	// for(int i=0;i<argc;i++) {
	//     printf("arg[%d]: %s\n", i, argv[i]);
	// }

	int **heights = malloc(M * sizeof(int *));
	for (int i = 0; i < M; i++) {
		heights[i] = malloc(N * sizeof(int));
		memcpy(heights[i], &test_heights[i], N * sizeof(int));
	}

	int col_size = N;
	printf("result: %d\n", minimumEffortPath(heights, M, &col_size));
}
