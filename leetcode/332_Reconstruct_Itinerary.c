#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>
#include <math.h>

/* ["JFK","MUC","LHR","SFO","SJC"] */
// char *test_tickets[4][2] = {{"MUC","LHR"},{"JFK","MUC"},{"SFO","SJC"},{"LHR","SFO"}};

/* ["JFK","ATL","JFK","SFO","ATL","SFO"] */
char *test_tickets[5][2] = { { "JFK", "SFO" },
			     { "JFK", "ATL" },
			     { "SFO", "ATL" },
			     { "ATL", "JFK" },
			     { "ATL", "SFO" } };

/* ["JFK","AAA","JFK","CCC","JFK","BBB"] */
// char *test_tickets[5][2] = {{"JFK","AAA"},{"AAA","JFK"},{"JFK","CCC"},{"CCC","JFK"}, {"JFK","BBB"}};

/* ["JFK","AAA","JFK","BBB","JFK","CCC","JFK"] */
// char *test_tickets[6][2] = {{"JFK","AAA"},{"AAA","JFK"},{"JFK","BBB"},{"BBB","JFK"},{"JFK","CCC"},{"CCC","JFK"}};

/* ["JFK","AAA","JFK","BBB","CCC","BBB","DDD"] */
// char *test_tickets[6][2] = {{"JFK","AAA"},{"AAA","JFK"},{"JFK","BBB"},{"BBB","CCC"},{"BBB","DDD"},{"CCC","BBB"}};

#define node_t_SIZE (sizeof(test_tickets) / sizeof(test_tickets[0]))

#define MAGIC_NUM 31L
#define MODULE_NUM 1000007L
long mod(long n, long m)
{
	return (n % m + m) % m;
}

long hash(char *charater)
{
	long hash_value = 0;
	hash_value += MAGIC_NUM * charater[0];
	hash_value = mod(hash_value, MODULE_NUM);

	hash_value += MAGIC_NUM * MAGIC_NUM * charater[1];
	hash_value = mod(hash_value, MODULE_NUM);

	hash_value += MAGIC_NUM * MAGIC_NUM * MAGIC_NUM * charater[2];
	hash_value = mod(hash_value, MODULE_NUM);

	return hash_value;
}

#define NEXT_MAX 50
typedef struct _node_t {
	struct _node_t *adj_list[NEXT_MAX];
	int adj_count;
	int i_count;
	struct _node_t *left;
	struct _node_t *right;
	long hash_val;
	char *string_p;
} node_t;

node_t *generate_itinerary(long hash_val)
{
	node_t *node = malloc(sizeof(node_t));
	for (int i = 0; i < NEXT_MAX; i++)
		node->adj_list[i] = NULL;
	node->hash_val = hash_val;
	node->adj_count = 0;
	node->i_count = 0;
	node->left = NULL;
	node->right = NULL;

	return node;
}

node_t *insert_bst(node_t *node, long hash_val, node_t **insert_node)
{
	if (node == NULL) {
		// end node
		node = generate_itinerary(hash_val);
		*insert_node = node;
		return node;
	}

	if (node->hash_val == hash_val) {
		return NULL;
		// assert(0);
	} else if (hash_val < node->hash_val) {
		// go left
		node_t *temp = insert_bst(node->left, hash_val, insert_node);
		if (temp != node->left) {
			node->left = temp;
			*insert_node = temp;
		}
	} else if (hash_val > node->hash_val) {
		// go right
		node_t *temp = insert_bst(node->right, hash_val, insert_node);
		if (temp != node->right) {
			node->right = temp;
			*insert_node = temp;
		}
	}

	return node;
}

node_t *search_in_bst(node_t *node, long hash_val)
{
	if (node == NULL)
		return NULL;

	if (hash_val == node->hash_val) {
		return node;
	}
	if (hash_val < node->hash_val) {
		// go left
		return search_in_bst(node->left, hash_val);
	} else if (hash_val > node->hash_val) {
		// go right
		return search_in_bst(node->right, hash_val);
	} else {
		return NULL;
	}
}

int laxical_determine(char *s1, char *s2)
{
	for (int i = 0; i < 3; i++) {
		if (s1[i] < s2[i]) {
			return 0;
		} else if (s1[i] > s2[i]) {
			return 1;
		}
	}

	return 0;
}

void swap_node(node_t **n1, node_t **n2)
{
	node_t *temp = *n1;
	*n1 = *n2;
	*n2 = temp;
}

void print_bst(node_t *node)
{
	if (node == NULL)
		return;

	print_bst(node->left);
	printf("[%s] hash: %ld, count: %d\n", node->string_p, node->hash_val,
	       node->adj_count);
	print_bst(node->right);
}

void traverse_graph(node_t *node, char ***result, int *count)
{
	while (node->i_count < node->adj_count) {
		traverse_graph(node->adj_list[node->i_count++], result, count);
	}
	// post-order
	// printf("traverse: %s\n", node->string_p);
	memcpy((*result)[*count], node->string_p, 3 * sizeof(char));
	(*count)++;
}

/**
 * Note: The returned array must be malloced, assume caller calls free().
 */
char **findItinerary(char ***tickets, int ticketsSize, int *ticketsColSize,
		     int *returnSize)
{
	char **result = malloc(ticketsSize * 2 * sizeof(char *));
	long start_hash = hash("JFK");
	// printf("start hash: %ld\n", start_hash);

	for (int i = 0; i < ticketsSize * 2; i++) {
		result[i] = malloc(4 * sizeof(char));
		memset(result[i], 0, 4 * sizeof(char));
	}

	node_t *st_itinerary = NULL;
	node_t *bst_root = NULL;
	int node_count = 0;

	/* main logic */
	for (int i = 0; i < ticketsSize; i++) {
		long from_hash = hash(tickets[i][0]);
		long to_hash = hash(tickets[i][1]);

		node_t *from_node = search_in_bst(bst_root, from_hash);
		node_t *to_node = search_in_bst(bst_root, to_hash);

		// printf("[%s]: %ld,  [%s]: %ld\n", tickets[i][0], from_hash, tickets[i][1], to_hash);
		// printf("%p, %p\n", from_node, to_node);
		if (NULL == from_node) {
			bst_root = insert_bst(bst_root, from_hash, &from_node);
			node_count++;
		}
		if (NULL == to_node) {
			bst_root = insert_bst(bst_root, to_hash, &to_node);
			node_count++;
		}

		from_node->string_p = tickets[i][0];
		to_node->string_p = tickets[i][1];

		if (from_hash == start_hash)
			st_itinerary = from_node;

		from_node->adj_list[from_node->adj_count++] = to_node;
		for (int k = from_node->adj_count - 2; k >= 0; k--) {
			if (laxical_determine(
				    from_node->adj_list[k]->string_p,
				    from_node->adj_list[k + 1]->string_p)) {
				swap_node(&from_node->adj_list[k],
					  &from_node->adj_list[k + 1]);
			}
		}
	}

	if (st_itinerary == NULL)
		return NULL;
	// print_bst(bst_root);

	/* traverse the itinerary */
	int col = 0;
	traverse_graph(st_itinerary, &result, &col);

	/* Reverse the result */
	for (int i = 0; i < col >> 1; i++) {
		char *temp = result[i];
		result[i] = result[col - i - 1];
		result[col - i - 1] = temp;
	}

	*ticketsColSize = col;
	*returnSize = col;

	return result;
}

int main(int argc, char *argv[])
{
	// for(int i=0;i<argc;i++) {
	//     printf("arg[%d]: %s\n", i, argv[i]);
	// }

	char ***tickets = malloc(node_t_SIZE * sizeof(char **));
	for (int i = 0; i < node_t_SIZE; i++) {
		tickets[i] = malloc(2 * sizeof(char *));
		for (int j = 0; j < 2; j++) {
			tickets[i][j] = malloc(3 * sizeof(char));
			memcpy(tickets[i][j], test_tickets[i][j],
			       3 * sizeof(char));
		}
	}

	int col_size;
	int return_size;
	char **result =
		findItinerary(tickets, node_t_SIZE, &col_size, &return_size);
	printf("==== %p\n", result);
	printf("size: %d\n", return_size);

	if (result == NULL)
		return 1;

	printf("result: \n");
	for (int j = 0; j < return_size; j++) {
		for (int k = 0; k < 3; k++) {
			printf("%c", result[j][k]);
		}
		printf(", ");
	}

	printf("\n");

	return 0;
}
