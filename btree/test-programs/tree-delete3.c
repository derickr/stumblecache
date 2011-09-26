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

	/* Deleting a node that's in a child */
	btree_delete(tmp, 'Q');
	btree_dump(tmp);

	/* Deleting a node that's in a child causing underflow */
	btree_delete(tmp, 'P');
	btree_dump(tmp);

	/* Add another key */
	btree_insert(tmp, 'Y', &data_idx); printf("%u\n", data_idx);
	btree_dump(tmp);

	/* Deleting a node that's in a child causing underflow */
	btree_delete(tmp, 'B');
	btree_dump(tmp);

	/* Deleting a node that's in a child causing merge (to left) */
	btree_delete(tmp, 'Z');
	btree_dump(tmp);

	/* Adding back in two nodes */
	btree_insert(tmp, 'Z', &data_idx); printf("%u\n", data_idx);
	btree_insert(tmp, 'Q', &data_idx); printf("%u\n", data_idx);
	btree_dump(tmp);

	/* Now force a collapse from the other side */
	btree_delete(tmp, 'Y');
	btree_delete(tmp, 'E');
	btree_dump(tmp);

	btree_free(tmp);

	return 0;
}
