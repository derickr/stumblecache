#include "btree.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
	btree_tree *tmp;
	char *data;
	uint64_t id;
	uint64_t found = 0;
	uint32_t idx;
	size_t data_size;
	time_t ts;

	tmp = btree_open("test.mmap");
	if (!tmp) {
		printf("Couldn't create tree from disk image.\n");
		exit(1);
	}

	id = atoll(argv[1]);
	found += btree_search(tmp, tmp->root, id, &idx);
	data = (char*) btree_get_data(tmp, idx, &data_size, &ts);
	printf("Found: %lu (%u)\n%s\n", found, idx, data);

	btree_free(tmp);

	return 0;
}
