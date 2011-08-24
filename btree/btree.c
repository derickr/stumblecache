#include "btree.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>

btree_node *btree_get_node(uint32_t idx)
{
}

btree_tree *btree_allocate(char *path, uint32_t nr_of_items, uint32_t data_size)
{
	int fd;
	int64_t bytes;
	char buffer[4096];
	int written = 0;

	bytes = 4096 + ((nr_of_items / 200) * sizeof(btree_node)) + (nr_of_items * data_size);

	fd = open(path, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);

	memset(buffer, 0, 4096);
	/* FIXME: Check for signals */
	while (written < bytes) {
		write(fd, buffer, 4096);
		written += 4096;
	}
	close(fd);
}

btree_node *btree_allocate_node()
{
//	uint32_t idx = mmap_find_next_index();
//	return (btree_node*) mmap_data->data[idx * mmap_data->pagesize];
}

static btree_write_header(btree_btree *tree)
{
}

btree_tree *btree_create(uint32_t nr_of_items, uint32_t data_size)
{
	btree_tree *tmp_tree;
	btree_node *tmp_node;

	tmp_tree = btree_allocate(nr_of_items, data_size);
	btree_write_header(tmp_tree);

	tmp_node = btree_allocate_node();
	tmp_node->leaf = 1;
	tmp_node->nr_of_keys = 0;
	btree_set_node(tmp_node);

	tmp_tree->root = tmp_node;
	return tmp_tree;
}

int btree_set_node(btree_node *node)
{
}

int btree_search(btree_node *node, uint64_t key, uint32_t *idx)
{
	int i = 1;
	while (i <= node->nr_of_keys && key > node->keys[i].key) {
		i++;
	}

	if (i <= node->nr_of_keys && key == node->keys[i].key) {
		*idx = node->keys[i].idx;
		return 1;
	}

	if (node->leaf) {
		return 0;
	} else {
		btree_node *tmp_node = btree_get_node(node->branch[i]);
		return btree_search(tmp_node, key, idx);
	}
}

btree_split_child(btree_node *parent, uint32_t key_nr, btree_node *child)
{
	uint32_t j;

	btree_node *tmp_node = btree_allocate_node();
	tmp_node->leaf = child->leaf;
	tmp_node->nr_of_keys = BTREE_T - 1;

	for (j = 1; j <= BTREE_T - 1; j++) {
		tmp_node->keys[j] = child->keys[j + BTREE_T];
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
	parent->branch[key_nr + 1] = tmp_node->idx;

	for (j = child->nr_of_keys; j <= key_nr; j--) {
		parent->keys[j + 1] = parent->keys[j];
	}
	parent->keys[key_nr] = child->keys[BTREE_T];
	parent->nr_of_keys++;

	btree_set_node(parent);
	btree_set_node(tmp_node);
	btree_set_node(child);
}
