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

	/* Testing one node in the root only */
	btree_insert(tmp, 'A', &data_idx); printf("%u\n", data_idx);
	btree_dump(tmp);
	btree_delete(tmp, 'A');
	btree_dump(tmp);


	/* Testing two node in the root only (1) */
	btree_insert(tmp, 'G', &data_idx); printf("%u\n", data_idx);
	btree_insert(tmp, 'Q', &data_idx); printf("%u\n", data_idx);
	btree_dump(tmp);
	btree_delete(tmp, 'G');
	btree_dump(tmp);
	/* - cleanup for next test */
	btree_delete(tmp, 'Q');


	/* Testing two node in the root only (2) */
	btree_insert(tmp, 'G', &data_idx); printf("%u\n", data_idx);
	btree_insert(tmp, 'Q', &data_idx); printf("%u\n", data_idx);
	btree_dump(tmp);
	btree_delete(tmp, 'Q');
	btree_dump(tmp);
	/* - cleanup for next test */
	btree_delete(tmp, 'G');


	/* Testing with full root node */
	btree_insert(tmp, 'E', &data_idx); printf("%u\n", data_idx);
	btree_insert(tmp, 'A', &data_idx); printf("%u\n", data_idx);
	btree_insert(tmp, 'P', &data_idx); printf("%u\n", data_idx);
	btree_insert(tmp, 'F', &data_idx); printf("%u\n", data_idx);
	btree_insert(tmp, 'Q', &data_idx); printf("%u\n", data_idx);
	btree_dump(tmp);
	/* - remove last node */
	btree_delete(tmp, 'Q');
	btree_dump(tmp);
	/* - remove middle node */
	btree_delete(tmp, 'E');
	btree_dump(tmp);
	/* - remove first node */
	btree_delete(tmp, 'A');
	btree_dump(tmp);

	btree_free(tmp);

	return 0;
}
