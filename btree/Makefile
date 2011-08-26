all: tree-create tree-search

tree-create: tree-create.c btree.c btree.h
	gcc -ggdb3 -o tree-create tree-create.c btree.c

tree-search: tree-search.c btree.c btree.h
	gcc -ggdb3 -o tree-search tree-search.c btree.c
