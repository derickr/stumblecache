#include "btree.h"
#include <stdio.h>

int main(int argc, char *argv[])
{
	btree_tree *tmp;
	FILE *f;
	char urlBuffer[2048], *data;
	uint64_t id;
	uint64_t found = 0;
	uint32_t idx, data_size;

	tmp = btree_open("test.mmap");
	if (!tmp) {
		printf("Couldn't create tree from disk image.\n");
		exit(1);
	}

	id = atoll(argv[1]);
	found += btree_search(tmp, tmp->root, id, &idx);
	data = (char*) btree_get_data(tmp, idx, &data_size);
	printf("Found: %d (%d)\n%s\n", found, idx, data);

	btree_free(tmp);
}
