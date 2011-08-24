#define BTREE_T    102
#define BTREE_T2   204

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
	btree_node *root;
} btree;

int btree_search(btree_node *node, uint64_t key, uint32_t *idx)
{
	int i = 1;
	while (i <= node->nr_of_keys && key > node->keys[i]) {
		i++;
	}

	if (i <= node->nr_of_keys && key = node->keys[i]) {
		*idx = node->keys[i]->idx;
		return 1;
	}

	if (node->leaf) {
		return 0;
	} else {
		btree_node *tmp_node = btree_get_node(node->branch[i]);
		return btree_search(tmp_node, key, idx);
	}
}

btree_tree *btree_allocate()
{
}

btree_node *btree_allocate_node()
{
	idx = mmap_find_next_index();
	return (btree_node*) mmap_data->data[idx * mmap_data->pagesize];
}

btree_tree *btree_create(void)
{
	btree      *tmp_tree = btree_allocate();
	btree_node *tmp_node = btree_allocate_node();
	tmp_node->leaf = 1;
	tmp_node->nr_of_keys = 0;
	btree_set_node(tmp_node);

	tmp_tree->root = tmp_node;
	return tmp_tree;
}
