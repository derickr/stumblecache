#define BTREE_HEADER_SIZE 4096
#define BTREE_T            4
#define BTREE_T2           (2 * BTREE_T)

#include <stdint.h>

typedef struct {
	uint64_t key;
	uint32_t idx;       /* index into data portion */
	uint32_t expire_ts; /* if 0, then not in use */
} btree_key; /* 16 bytes */

typedef struct {
	uint32_t      idx;        /* the node's index into the data store */
	uint32_t      branch[BTREE_T2];
	btree_key     keys[BTREE_T2 - 1];
	char          leaf;       /* whether it's a leaf node or not */
	uint16_t      nr_of_keys; /* number of keys in use */
} btree_node; /* 204*4 + 203*16 + 1 + 2 = 4067, which fits nicely in a page */

typedef struct {
	uint32_t version;
	uint32_t max_items;
	uint32_t item_size;
	uint32_t next_node_idx;
	uint32_t next_idx;
	uint32_t root_node_idx;
} btree_header;

typedef struct {
	btree_header *header;
	btree_node   *root;
	int           fd;
	void         *mmap;
	void         *nodes;
	void         *data;
} btree_tree;

btree_tree *btree_open(char *path);
btree_tree *btree_create(char *path, uint32_t nr_of_items, uint32_t data_size);
void btree_free(btree_tree *t);

int btree_search(btree_tree *t, btree_node *node, uint64_t key, uint32_t *idx);
void btree_dump_node(btree_tree *t, btree_node *node);
void btree_dump_tree(btree_tree *t);
