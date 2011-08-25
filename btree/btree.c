#include "btree.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <stdlib.h>

btree_node *btree_get_node(btree_tree *t, uint32_t idx)
{
	return (btree_node*) (t->nodes + (idx * 4096));
}

static void btree_allocate(char *path, uint32_t nr_of_items, uint32_t data_size)
{
	int fd;
	int64_t bytes;
	char buffer[4096];
	int written = 0;

	bytes = BTREE_HEADER_SIZE + ((nr_of_items / BTREE_T2) * sizeof(btree_node)) + (nr_of_items * data_size);

	fd = open(path, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);

	memset(buffer, 0, 4096);
	/* FIXME: Check for signals */
	while (written < bytes) {
		write(fd, buffer, 4096);
		written += 4096;
	}
	close(fd);
}

btree_node *btree_allocate_node(btree_tree *t)
{
	btree_node *tmp_node;

	/* LOCK */
	tmp_node = t->nodes + (t->header->next_node_idx * 4096);
	tmp_node->idx = t->header->next_node_idx;
	t->header->next_node_idx++;
	/* UNLOCK */

	return tmp_node;
}

static btree_tree *btree_open(char *path)
{
	int fd;
	void *memory;
	btree_tree *t;
	struct stat fileinfo;

	stat(path, &fileinfo);
	fd = open(path, O_RDWR);
	memory = mmap(NULL, fileinfo.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

	t = malloc(sizeof(btree_tree));
	t->fd = fd;
	t->mmap = memory;
	t->header = (btree_header*) t->mmap;
	t->nodes  = t->mmap + BTREE_HEADER_SIZE;

	return t;
}

btree_tree *btree_create(char *path, uint32_t nr_of_items, uint32_t data_size)
{
	btree_tree *tmp_tree;
	btree_node *tmp_node;

	btree_allocate(path, nr_of_items, data_size);

	tmp_tree = btree_open(path);

	tmp_tree->header->version = 1;
	tmp_tree->header->max_items = nr_of_items;
	tmp_tree->header->item_size = data_size;
	tmp_tree->header->next_node_idx = 0;
	tmp_tree->header->next_idx = 0;

	tmp_node = btree_allocate_node(tmp_tree);
	tmp_node->leaf = 1;
	tmp_node->nr_of_keys = 0;
	btree_set_node(tmp_node);

	tmp_tree->root = tmp_node;

	return tmp_tree;
}

int btree_set_node(btree_node *node)
{
}

int btree_search(btree_tree *t, btree_node *node, uint64_t key, uint32_t *idx)
{
	int i = 0;
	while (i < node->nr_of_keys && key > node->keys[i].key) {
		i++;
	}

	if (i < node->nr_of_keys && key == node->keys[i].key) {
		*idx = node->keys[i].idx;
		return 1;
	}

	if (node->leaf) {
		return 0;
	} else {
		btree_node *tmp_node = btree_get_node(t, node->branch[i]);
		return btree_search(t, tmp_node, key, idx);
	}
}

btree_split_child(btree_tree *t, btree_node *parent, uint32_t key_nr, btree_node *child)
{
	uint32_t j;

	btree_node *tmp_node = btree_allocate_node(t);
	tmp_node->leaf = child->leaf;
	tmp_node->nr_of_keys = BTREE_T - 1;

	for (j = 0; j < BTREE_T - 1; j++) {
		tmp_node->keys[j] = child->keys[j + BTREE_T];
	}
	if (!child->leaf) {
		for (j = 0; j < BTREE_T; j++) {
			tmp_node->branch[j] = child->branch[j + BTREE_T];
		}
	}
	child->nr_of_keys = BTREE_T - 1;
	for (j = parent->nr_of_keys; j > key_nr + 1; j--) {
		parent->branch[j + 1] =  parent->branch[j];
	}
	parent->branch[key_nr + 1] = tmp_node->idx;

	for (j = child->nr_of_keys - 1; j < key_nr; j--) {
		parent->keys[j + 1] = parent->keys[j];
	}
	parent->keys[key_nr] = child->keys[BTREE_T];
	parent->nr_of_keys++;

	btree_set_node(parent);
	btree_set_node(tmp_node);
	btree_set_node(child);
}

btree_insert(btree_tree *t, uint64_t key, uint32_t *data_idx)
{
	btree_node *r = t->root;
	if (r->nr_of_keys == BTREE_T2 - 1) {
		btree_node *tmp_node;

		tmp_node = btree_allocate_node(t);
		t->root = tmp_node;
		tmp_node->leaf = 0;
		tmp_node->nr_of_keys = 0;
		tmp_node->branch[0] = r->idx;
		btree_split_child(t, tmp_node, 0, r);
		btree_insert_non_full(t, tmp_node, key, data_idx);
	} else {
		btree_insert_non_full(t, r, key, data_idx);
	}
}

btree_insert_non_full(btree_tree *t, btree_node *node, uint64_t key, uint32_t *data_idx)
{
	uint32_t i;
	btree_node *tmp_node;

	i = node->nr_of_keys;
	if (node->leaf) {
		while (i > 0 && key < node->keys[i - 1].key) {
			node->keys[i] = node->keys[i - 1];
			i--;
		}
		node->keys[i].key = key;
		/* Fetch data index, and set it to the idx element here too */
		node->nr_of_keys++;
	} else {
		while (i > 0 && key < node->keys[i - 1].key) {
			i--;
		}
		tmp_node = btree_get_node(t, node->branch[i]);
		if (tmp_node->nr_of_keys == BTREE_T2 - 1) {
			btree_split_child(t, node, i, tmp_node);
			if (key > node->keys[i].key) {
				i++;
			}
		}
		btree_insert_non_full(t, btree_get_node(t, node->branch[i]), key, data_idx);
	}
}

void btree_dump_node(btree_tree *t, btree_node *node)
{
	int i;

	printf("\nIDX: %d\n   ", node->idx);
	for (i = 0; i < node->nr_of_keys; i++) {
		printf("%4d ", node->keys[i].key);
	}
	if (!node->leaf) {
		printf("\n");
		for (i = 0; i < node->nr_of_keys + 1; i++) {
			printf("%4d ", node->branch[i]);
		}
		for (i = 0; i < node->nr_of_keys + 1; i++) {
			btree_dump_node(t, btree_get_node(t, node->branch[i]));
		}
	}
}

void btree_dump_tree(btree_tree *t)
{
	btree_dump_node(t, t->root);
	printf("\n");
}
