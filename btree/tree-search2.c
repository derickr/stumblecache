#include "btree.h"
#include <stdio.h>

int main(int argc, char *argv[])
{
	btree_tree *tmp;
	FILE *f;
	char urlBuffer[2048], *data;
	uint64_t id;
	uint64_t i = 0, found = 0;
	uint32_t idx;

	tmp = btree_open("test.mmap");
	if (!tmp) {
		printf("Couldn't create tree from disk image.\n");
		exit(1);
	}

	f = fopen(argv[1], "r");
	while (!feof(f)) {
		fgets(urlBuffer, 2048, f);
		data = strchr(urlBuffer, ' ');
		if (data) {
			data++;
			data[-1] = '\0';
			id = atoll(urlBuffer);
			found += btree_search(tmp, tmp->root, id, &idx);
			found += btree_search(tmp, tmp->root, id + 1, &idx);
			i++;
		}
	}
	printf("%d %d\n", found, i);

	btree_free(tmp);
}
