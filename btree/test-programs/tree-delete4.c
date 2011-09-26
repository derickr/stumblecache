#include "btree.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void setup(btree_tree *tmp)
{
	uint32_t data_idx;

	/* Testing with full root node */
	btree_insert(tmp, 'F', &data_idx); printf("%u\n", data_idx);
	btree_insert(tmp, 'Q', &data_idx); printf("%u\n", data_idx);
	btree_insert(tmp, 'Z', &data_idx); printf("%u\n", data_idx);
	btree_insert(tmp, 'E', &data_idx); printf("%u\n", data_idx);
	btree_insert(tmp, 'A', &data_idx); printf("%u\n", data_idx);
	btree_insert(tmp, 'B', &data_idx); printf("%u\n", data_idx);
	btree_insert(tmp, 'P', &data_idx); printf("%u\n", data_idx);
	btree_insert(tmp, 'G', &data_idx); printf("%u\n", data_idx);
	btree_insert(tmp, 'R', &data_idx); printf("%u\n", data_idx);
	btree_insert(tmp, 'Y', &data_idx); printf("%u\n", data_idx);
	btree_insert(tmp, 'H', &data_idx); printf("%u\n", data_idx);
	btree_insert(tmp, 'C', &data_idx); printf("%u\n", data_idx);
	btree_insert(tmp, 'D', &data_idx); printf("%u\n", data_idx);
	btree_insert(tmp, 'S', &data_idx); printf("%u\n", data_idx);
	btree_dump(tmp);

	btree_delete(tmp, 'H');
	btree_dump(tmp);

	btree_delete(tmp, 'G');
	btree_dump(tmp);

	btree_delete(tmp, 'P');
	btree_dump(tmp);

	btree_delete(tmp, 'F');
	btree_dump(tmp);

	btree_delete(tmp, 'D');
	btree_dump(tmp);

	btree_delete(tmp, 'Q');
	btree_dump(tmp);
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

	/* Should cause collapse */
	btree_delete(tmp, 'E');
	btree_dump(tmp);
	/* Another bit less */
	btree_delete(tmp, 'B');
	btree_delete(tmp, 'C');
	btree_dump(tmp);
	/* Should cause collapse to root */
	btree_delete(tmp, 'Z');
	btree_dump(tmp);
	/* Clean up */
	btree_delete(tmp, 'S');
	btree_delete(tmp, 'A');
	btree_delete(tmp, 'R');
	btree_delete(tmp, 'Y');
	btree_dump(tmp);


	setup(tmp);
	btree_dump(tmp);
	/* Should cause collapse */
	btree_delete(tmp, 'Z');
	btree_dump(tmp);
	/* Add a few more to create a split */
	btree_insert(tmp, 'T', &data_idx); printf("%u\n", data_idx);
	btree_insert(tmp, 'Z', &data_idx); printf("%u\n", data_idx);
	btree_dump(tmp);
	/* Should cause collapse */
	btree_delete(tmp, 'A');
	btree_dump(tmp);
	/* Delete root node */
	btree_delete(tmp, 'S');
	btree_dump(tmp);
	/* Delete root node (3x) */
	btree_delete(tmp, 'R');
	btree_delete(tmp, 'E');
	btree_delete(tmp, 'T');
	btree_dump(tmp);
	/* Clean up */
	btree_delete(tmp, 'B');
	btree_delete(tmp, 'Z');
	btree_delete(tmp, 'Y');
	btree_delete(tmp, 'C');
	btree_dump(tmp);

	setup(tmp);
	btree_dump(tmp);
	/* Should cause collapse */
	btree_delete(tmp, 'B');
	btree_dump(tmp);

	btree_free(tmp);

	return 0;
}
