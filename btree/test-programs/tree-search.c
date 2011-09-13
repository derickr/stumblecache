#include "btree.h"

int main(void)
{
	btree_tree *tmp;
	uint32_t idx;
	uint64_t i;

	tmp = btree_open("test.mmap");
	btree_dump(tmp);

	for (i = 0; i < 125000000; i++) {
		btree_search(tmp, tmp->root, 'F', &idx);
		btree_search(tmp, tmp->root, 'y', &idx);
		btree_search(tmp, tmp->root, '0', &idx);
		btree_search(tmp, tmp->root, '1', &idx);
	}
	btree_free(tmp);

	return 0;
}
