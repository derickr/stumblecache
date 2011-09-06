#include "btree.h"

int main(void)
{
	btree_tree *tmp;
	uint32_t data_idx;
	char data[1024];

	memset(data, 4, 1023);
	memset(data+1023, 0, 1);

	tmp = btree_create("test.mmap", 3, 400, 1024);
	btree_insert(tmp, 'A', &data_idx); printf("%llu\n", data_idx);
	btree_insert(tmp, 'L', &data_idx); printf("%llu\n", data_idx);
	memcpy(tmp->data + data_idx * 1024, data, 1024);
	btree_insert(tmp, 'D', &data_idx); printf("%llu\n", data_idx);
	memcpy(tmp->data + data_idx * 1024, data, 1024);
	btree_insert(tmp, 'F', &data_idx); printf("%llu\n", data_idx);

	btree_insert(tmp, '4', &data_idx); printf("%llu\n", data_idx);
	btree_insert(tmp, '2', &data_idx); printf("%llu\n", data_idx);
	btree_insert(tmp, '3', &data_idx); printf("%llu\n", data_idx);
	btree_insert(tmp, '5', &data_idx); printf("%llu\n", data_idx);
	btree_insert(tmp, '1', &data_idx); printf("%llu\n", data_idx);

	btree_insert(tmp, 'N', &data_idx); printf("%llu\n", data_idx);
	btree_insert(tmp, 'P', &data_idx); printf("%llu\n", data_idx);
	btree_insert(tmp, 'd', &data_idx); printf("%llu\n", data_idx);
	btree_insert(tmp, 'f', &data_idx); printf("%llu\n", data_idx); /* */
	btree_insert(tmp, 'n', &data_idx); printf("%llu\n", data_idx);
	btree_insert(tmp, 'p', &data_idx); printf("%llu\n", data_idx);
	btree_insert(tmp, 'H', &data_idx); printf("%llu\n", data_idx);
	btree_insert(tmp, 'C', &data_idx); printf("%llu\n", data_idx);
	btree_insert(tmp, 'B', &data_idx); printf("%llu\n", data_idx);
	btree_insert(tmp, 'E', &data_idx); printf("%llu\n", data_idx);
	btree_insert(tmp, 'G', &data_idx); printf("%llu\n", data_idx); /* */

	btree_insert(tmp, 'I', &data_idx); printf("%llu\n", data_idx);
	btree_insert(tmp, 'K', &data_idx); printf("%llu\n", data_idx);
	btree_insert(tmp, 'J', &data_idx); printf("%llu\n", data_idx);
	btree_insert(tmp, 'M', &data_idx); printf("%llu\n", data_idx);
	btree_insert(tmp, 'o', &data_idx); printf("%llu\n", data_idx); /* */

	btree_insert(tmp, 'q', &data_idx); printf("%llu\n", data_idx);
	btree_insert(tmp, 'r', &data_idx); printf("%llu\n", data_idx);
	btree_insert(tmp, 'i', &data_idx); printf("%llu\n", data_idx);

	btree_insert(tmp, 'j', &data_idx); printf("%llu\n", data_idx);
	btree_insert(tmp, 'k', &data_idx); printf("%llu\n", data_idx);
	btree_insert(tmp, 's', &data_idx); printf("%llu\n", data_idx);
	btree_insert(tmp, 't', &data_idx); printf("%llu\n", data_idx);
	btree_insert(tmp, 'm', &data_idx); printf("%llu\n", data_idx);

	btree_insert(tmp, 'O', &data_idx); printf("%llu\n", data_idx);
	btree_insert(tmp, 'Q', &data_idx); printf("%llu\n", data_idx);
	btree_insert(tmp, 'R', &data_idx); printf("%llu\n", data_idx);
	btree_insert(tmp, 'S', &data_idx); printf("%llu\n", data_idx);
	btree_insert(tmp, 'T', &data_idx); printf("%llu\n", data_idx);
	btree_insert(tmp, 'U', &data_idx); printf("%llu\n", data_idx);

	btree_insert(tmp, 'x', &data_idx); printf("%llu\n", data_idx);
	btree_insert(tmp, 'w', &data_idx); printf("%llu\n", data_idx);
	btree_insert(tmp, 'y', &data_idx); printf("%llu\n", data_idx);
	btree_insert(tmp, 'u', &data_idx); printf("%llu\n", data_idx);
	btree_insert(tmp, 'v', &data_idx); printf("%llu\n", data_idx);

	btree_dump_dot(tmp);
	btree_free(tmp);
}
