#include "btree.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void)
{
	btree_tree *tmp;
	uint32_t data_idx, size;

	// testing too many nodes
	tmp = btree_create("test.mmap", 3, 6, 10);
	if (!tmp) {
		printf("Couldn't create tree from disk image.\n");
		exit(1);
	}
	btree_insert(tmp, 'A', &data_idx);
	btree_set_data(tmp, data_idx, "HelloWorl1", 10);
	printf("%s %d\n", (char*) btree_get_data(tmp, data_idx, &size), size);

	btree_insert(tmp, 'L', &data_idx);
	btree_set_data(tmp, data_idx, "HelloWorl2", 10);
	printf("%s %d\n", (char*) btree_get_data(tmp, data_idx, &size), size);

	btree_insert(tmp, 'D', &data_idx);
	btree_set_data(tmp, data_idx, "HelloWorl3", 10);
	printf("%s %d\n", (char*) btree_get_data(tmp, data_idx, &size), size);

	btree_insert(tmp, 'X', &data_idx);
	btree_set_data(tmp, data_idx, "HelloWorl4", 10);
	printf("%s %d\n", (char*) btree_get_data(tmp, data_idx, &size), size);

	btree_insert(tmp, 'R', &data_idx);
	btree_set_data(tmp, data_idx, "HelloWorl5", 10);
	printf("%s %d\n", (char*) btree_get_data(tmp, data_idx, &size), size);

	btree_insert(tmp, 'T', &data_idx);
	btree_set_data(tmp, data_idx, "HelloWorl6", 10);
	printf("%s %d\n", (char*) btree_get_data(tmp, data_idx, &size), size);

	btree_dump_dot(tmp);
	btree_free(tmp);

	return 0;
}
