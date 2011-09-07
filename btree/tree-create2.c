#include "btree.h"
#include <stdio.h>

int main(int argc, char *argv[])
{
	btree_tree *tmp;
	FILE *f;
	char urlBuffer[2048], *data;
	uint64_t id;
	uint64_t i = 0,cnt;
	uint32_t data_idx;

	cnt = atoll(argv[2]);
	tmp = btree_create("test.mmap", 128, 700000, 2048);

	f = fopen(argv[1], "r");
	while (!feof(f)) {
		fgets(urlBuffer, 2048, f);
		data = strchr(urlBuffer, ' ');
		if (data) {
			data++;
			data[-1] = '\0';
			id = atoll(urlBuffer);
			if (btree_safe_insert(tmp, id, &data_idx) == 1) {
				btree_set_data(tmp, data_idx, data, strlen(data));
			}
		}
		i++;
	}

	btree_free(tmp);
}