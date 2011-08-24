#include "btree.h"

int main(void)
{
	btree_tree *tmp;

	tmp = btree_create("test.mmap", 400, 1024);
	btree_insert(tmp, 'A', (void*) 0);
	btree_insert(tmp, 'L', (void*) 0);
	btree_insert(tmp, 'D', (void*) 0);
	btree_insert(tmp, 'F', (void*) 0);
	btree_insert(tmp, 'N', (void*) 0);
	btree_insert(tmp, 'P', (void*) 0);
	btree_insert(tmp, 'H', (void*) 0);

	btree_insert(tmp, 'C', (void*) 0);
}
