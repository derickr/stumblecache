#include "btree.h"
#include <stdio.h>

int main(int argc, char *argv[])
{
	btree_tree *tmp;
	FILE *f;
	char urlBuffer[16];
	uint64_t id;
	uint64_t i = 0,cnt;

	cnt = atoll(argv[2]);
	tmp = btree_create("test.mmap", 128, 700000, 2048);

	f = fopen(argv[1], "r");
	while (!feof(f)) {
		fgets(urlBuffer, 16, f);
		id = atoll(urlBuffer);
		if (btree_insert(tmp, id, (void*) 0) == 0) {
	//		printf("%llu already inserted\n", id);
		}
		i++;
	}

	btree_free(tmp);
}
