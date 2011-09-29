#include "btree.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void setup(btree_tree *tmp)
{
	uint32_t data_idx;

	/* Testing with full root node */
	btree_insert(tmp, 'F', &data_idx);
	btree_insert(tmp, 'Q', &data_idx);
	btree_insert(tmp, 'Z', &data_idx);
	btree_insert(tmp, 'E', &data_idx);
	btree_insert(tmp, 'A', &data_idx);
	btree_insert(tmp, 'B', &data_idx);
	btree_insert(tmp, 'N', &data_idx);
	btree_insert(tmp, 'G', &data_idx);
	btree_insert(tmp, 'R', &data_idx);
	btree_insert(tmp, 'Y', &data_idx);
	btree_insert(tmp, 'H', &data_idx);
	btree_insert(tmp, 'C', &data_idx);
	btree_insert(tmp, 'D', &data_idx);
	btree_insert(tmp, 'I', &data_idx);
	btree_insert(tmp, 'L', &data_idx);
	btree_insert(tmp, 'T', &data_idx);

	btree_insert(tmp, 'q', &data_idx);
	btree_insert(tmp, 'w', &data_idx);
	btree_insert(tmp, 'e', &data_idx);
	btree_insert(tmp, 'r', &data_idx);
	btree_insert(tmp, 't', &data_idx);
	btree_insert(tmp, 'y', &data_idx);
	btree_insert(tmp, 'u', &data_idx);
	btree_insert(tmp, 'i', &data_idx);
	btree_insert(tmp, 'o', &data_idx);
	btree_insert(tmp, 'p', &data_idx);

	btree_insert(tmp, 'S', &data_idx);
	btree_insert(tmp, 'W', &data_idx);
	btree_insert(tmp, 'U', &data_idx);
}

int main(void)
{
	btree_tree *tmp;
	uint32_t data_idx;

	tmp = btree_create("test.mmap", 3, 400, 1024);
	if (!tmp) {
		printf("Couldn't create tree from disk image.\n");
		exit(1);
	}

	setup(tmp);
	btree_insert(tmp, 'd', &data_idx);
	btree_insert(tmp, 'f', &data_idx);
	btree_insert(tmp, 'v', &data_idx);

	btree_delete(tmp, 'i');
	btree_delete(tmp, 'f');
	btree_delete(tmp, 'e');
	btree_delete(tmp, 'y');
	btree_delete(tmp, 't');
	btree_dump_dot(tmp);


	btree_free(tmp);

	return 0;
}
