CFLAGS=-ggdb3 -Wall

all: tree-create tree-search tree-create2 tree-search2 tree-search3 tree-create-failures

clean:
	rm tree-create tree-search tree-create2 tree-search2 tree-search3

tree-create: tree-create.c btree.c btree.h
	gcc ${CFLAGS} -o tree-create tree-create.c btree.c

tree-create-failures: tree-create-failures.c btree.c btree.h
	gcc ${CFLAGS} -o tree-create-failures tree-create-failures.c btree.c

tree-create2: tree-create2.c btree.c btree.h
	gcc ${CFLAGS} -o tree-create2 tree-create2.c btree.c

tree-search: tree-search.c btree.c btree.h
	gcc ${CFLAGS} -o tree-search tree-search.c btree.c

tree-search2: tree-search2.c btree.c btree.h
	gcc ${CFLAGS} -o tree-search2 tree-search2.c btree.c

tree-search3: tree-search3.c btree.c btree.h
	gcc ${CFLAGS} -o tree-search3 tree-search3.c btree.c
