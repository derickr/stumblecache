#include "btree.h"

int main(void)
{
	btree_tree *tmp;

	tmp = btree_create("test.mmap", 400, 1024);
	btree_insert(tmp, 2, (void*) 0);
}
