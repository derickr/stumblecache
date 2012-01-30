#include "btree.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void setup(btree_tree *tmp)
{
	uint32_t data_idx;
	int i;

	for (i = 0; i < 80; i++) {
		btree_insert(tmp, i, &data_idx);
	}
}

int main(void)
{
	btree_tree *tmp;
	int i;
	uint32_t data_idx;

	tmp = btree_create("test.mmap", 5, 64, 32);
	if (!tmp) {
		printf("Couldn't create tree from disk image.\n");
		exit(1);
	}

	setup(tmp);
	for (i = 25; i < 31; i++) {
		btree_delete(tmp, i);
	}
	btree_dump(tmp);
	btree_delete(tmp, 31);
	btree_dump(tmp);

	btree_free(tmp);
	unlink("test.mmap");

	return 0;
}
