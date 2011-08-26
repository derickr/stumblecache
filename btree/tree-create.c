#include "btree.h"

int main(void)
{
	btree_tree *tmp;

	tmp = btree_create("test.mmap", 3, 400, 1024);
	btree_insert(tmp, 'A', (void*) 0);
	btree_insert(tmp, 'L', (void*) 0);
	btree_insert(tmp, 'D', (void*) 0);
	btree_insert(tmp, 'F', (void*) 0);

	btree_insert(tmp, '4', (void*) 0);
	btree_insert(tmp, '2', (void*) 0);
	btree_insert(tmp, '3', (void*) 0);
	btree_insert(tmp, '5', (void*) 0);
	btree_insert(tmp, '1', (void*) 0);

	btree_insert(tmp, 'N', (void*) 0);
	btree_insert(tmp, 'P', (void*) 0);
	btree_insert(tmp, 'd', (void*) 0);
	btree_insert(tmp, 'f', (void*) 0); /* */
	btree_insert(tmp, 'n', (void*) 0);
	btree_insert(tmp, 'p', (void*) 0);
	btree_insert(tmp, 'H', (void*) 0);
	btree_insert(tmp, 'C', (void*) 0);
	btree_insert(tmp, 'B', (void*) 0);
	btree_insert(tmp, 'E', (void*) 0);
	btree_insert(tmp, 'G', (void*) 0); /* */

	btree_insert(tmp, 'I', (void*) 0);
	btree_insert(tmp, 'K', (void*) 0);
	btree_insert(tmp, 'J', (void*) 0);
	btree_insert(tmp, 'M', (void*) 0);
	btree_insert(tmp, 'o', (void*) 0); /* */

	btree_insert(tmp, 'q', (void*) 0);
	btree_insert(tmp, 'r', (void*) 0);
	btree_insert(tmp, 'i', (void*) 0);

	btree_insert(tmp, 'j', (void*) 0);
	btree_insert(tmp, 'k', (void*) 0);
	btree_insert(tmp, 's', (void*) 0);
	btree_insert(tmp, 't', (void*) 0);
	btree_insert(tmp, 'm', (void*) 0);

	btree_insert(tmp, 'O', (void*) 0);
	btree_insert(tmp, 'Q', (void*) 0);
	btree_insert(tmp, 'R', (void*) 0);
	btree_insert(tmp, 'S', (void*) 0);
	btree_insert(tmp, 'T', (void*) 0);
	btree_insert(tmp, 'U', (void*) 0);

	btree_insert(tmp, 'x', (void*) 0);
	btree_insert(tmp, 'w', (void*) 0);
	btree_insert(tmp, 'y', (void*) 0);
	btree_insert(tmp, 'u', (void*) 0);
	btree_insert(tmp, 'v', (void*) 0);

	btree_dump(tmp);
	btree_free(tmp);
}
