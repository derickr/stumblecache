#include "btree.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void)
{
	btree_tree *tmp;
	uint32_t data_idx, size;
	char *data;
	int i;
	char testdata[11];

	// testing too large data
	tmp = btree_create("test.mmap", 102, 204, 10);
	if (!tmp) {
		printf("Couldn't create tree from disk image.\n");
		exit(1);
	}
	for (i = 0; i < 205; i++) {
		if (btree_insert(tmp, i * 3, &data_idx)) {
			sprintf(testdata, "H: %07d", i);
			btree_set_data(tmp, data_idx, testdata, 10);
			data = (char*) btree_get_data(tmp, data_idx, &size);
			printf("%s %d\n", data, size);
		}
	}

	btree_dump(tmp);
	btree_free(tmp);

	return 0;
}
