#include "btree.h"

int main(void)
{
	btree_tree *tmp;

	tmp = btree_open("test.mmap");
	btree_dump(tmp);
	btree_free(tmp);
}
