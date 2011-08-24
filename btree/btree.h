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

btree_tree *btree_allocate(uint32_t nr_of_items, uint32_t data_size)
{
	int fd;
	int64_t bytes;

	bytes = 4096 + ((nr_of_items / 200) * sizeof(struct btree_node)) + (nr_of_items * data_size);

	fd = open("/tmp/test.mmap", O_CREAT);
printf("Creating contents: %d bytes\n");
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

btree_split_child(btree_node *parent, uint32_t key_nr, btree_node *child)
{
	btree_node *tmp_node = btree_allocate_node();
	tmp_node->leaf = child->leaf;
	tmp_node->nr_of_keys = BTREE_T - 1;

	for (j = 1; j <= BTREE_T - 1; j++) {
		tmp_node->keys[j] = child_node->keys[j + BTREE_T];
	}
	if (!child->leaf) {
		for (j = 1; j <= BTREE_T; j++) {
			tmp_node->branch[j] = child->branch[j + BTREE_T];
		}
	}
	child->nr_of_keys = BTREE_T - 1;
	for (j = parent->nr_of_keys + 1; j >= key_nr + 1; j--) {
		parent->branch[j + 1] =  parent->branch[j];
	}
	parent->branch[key + 1] = tmp_node->idx;

	for (j = child->nr_of_keys; j <= key_nr; j--) {
		parent->keys[j + 1] = parent->keys[j];
	}
	parent->keys[key_nr] = child->keys[BTREE_T];
	parent->nr_of_keys++;

	btree_set_node(parent);
	btree_set_node(tmp_node);
	btree_set_node(child);
}
