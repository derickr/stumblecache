#include "btree.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void)
{
	btree_tree *tmp;
	uint32_t data_idx;
	char data[1024];

	memset(data, 4, 1023);
	memset(data+1023, 0, 1);

	tmp = btree_create("test.mmap", 3, 400, 1024);
	if (!tmp) {
		printf("Couldn't create tree from disk image.\n");
		exit(1);
	}

	/* Testing with full root node */
	btree_insert(tmp, 'F', &data_idx); printf("%u\n", data_idx);
	btree_insert(tmp, 'Q', &data_idx); printf("%u\n", data_idx);
	btree_insert(tmp, 'Z', &data_idx); printf("%u\n", data_idx);
	btree_insert(tmp, 'E', &data_idx); printf("%u\n", data_idx);
	btree_insert(tmp, 'A', &data_idx); printf("%u\n", data_idx);
	btree_insert(tmp, 'B', &data_idx); printf("%u\n", data_idx);
	btree_insert(tmp, 'P', &data_idx); printf("%u\n", data_idx);
	btree_dump(tmp);
	/* Delete internal node (the root node in this case) */
	btree_delete(tmp, 'F');
	btree_dump(tmp);

	/* Delete internal node (the root node in this case, but now the previous child has less than N keys) */
	btree_delete(tmp, 'E');
	btree_dump(tmp);

	/* Delete internal node (the root node in this case, but now the previous child and next child have less than N keys) */
	btree_delete(tmp, 'P'); /* Left: ABQZ */
	btree_dump(tmp);

	/* Delete node so that the root node becomes one sized */
	btree_delete(tmp, 'A');
	btree_delete(tmp, 'B');
	btree_delete(tmp, 'Z');
	btree_dump(tmp);

	/* Delete last node */
	btree_delete(tmp, 'Q');
	btree_dump(tmp);

	/* Add one more just to see if that works */
	btree_insert(tmp, 'W', &data_idx); printf("%u\n", data_idx);
	btree_dump(tmp);

	btree_free(tmp);

	return 0;
}
