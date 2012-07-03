#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <math.h>
#include <fcntl.h>

#include "btree.h"
#include "set.h"

#define NODE_COUNT(nr_of_items, order)  (((2 * nr_of_items) / order) + 1)
#define BTREE_FREELIST_SIZE(nr_of_items) ((unsigned int) (ceil(nr_of_items / 4096.0) * 4096))
#define BTREE_DATA_EXTRA (1 + sizeof(size_t) + sizeof(time_t))

/* Forwards declarations */
static void btree_dump_node(btree_tree *t, btree_node *node);
static btree_node* btree_find_branch(btree_tree *t, btree_node *node, uint64_t key, uint32_t *i);
static void *btree_get_data_location(btree_tree *t, uint32_t idx);

#define LOCK_DEBUG 1

/* Locking */
#define BT_LOCK btree_admin_lock(t)
#define BT_UNLOCK btree_admin_unlock(t)
#define BT_LOCK_DATA_R(idx) btree_data_lockr(t, (idx))
#define BT_LOCK_DATA_W(idx) btree_data_lockw(t, (idx))
#define BT_UNLOCK_DATA(idx) btree_data_unlock(t, (idx))

static int btree_admin_lock(btree_tree *t)
{
	struct flock fls;

	fls.l_type   = F_WRLCK;
	fls.l_whence = SEEK_SET;
	fls.l_start  = 0;
	fls.l_len    = t->data - t->mmap;

#if LOCK_DEBUG
	printf("LOCKW   %4x - %4x\n", fls.l_start, fls.l_len);
#endif

	if (fcntl(t->fd, F_SETLKW, &fls) == -1) {
		return 0;
	}
	return 1;
}

static int btree_admin_unlock(btree_tree *t)
{
	struct flock fls;

	fls.l_type   = F_UNLCK;
	fls.l_whence = SEEK_SET;
	fls.l_start  = 0;
	fls.l_len    = t->data - t->mmap;

#if LOCK_DEBUG
	printf("UNLOCK  %4x - %4x", fls.l_start, fls.l_len);
#endif

	if (fcntl(t->fd, F_SETLKW, &fls) == -1) {
#if LOCK_DEBUG
		printf(" X: %d\n", errno);
#endif
		return 0;
	}
#if LOCK_DEBUG
	printf(" V\n");
#endif
	return 1;
}

inline static int btree_data_lock_helper(btree_tree *t, uint32_t idx, short type)
{
	struct flock fls;

	fls.l_type   = type;
	fls.l_whence = SEEK_SET;
	fls.l_start  = btree_get_data_location(t, idx) - t->mmap;
	fls.l_len    = t->header->item_size + BTREE_DATA_EXTRA;

#if LOCK_DEBUG
	printf("%7s %4x - %4x", type == F_RDLCK ? "DLOCKR" : (type == F_WRLCK ? "DLOCKW" : "UNLOCKD" ), fls.l_start, fls.l_len);
#endif

	if (fcntl(t->fd, F_SETLKW, &fls) == -1) {
#if LOCK_DEBUG
		printf(" X: %d\n", errno);
#endif
		return 0;
	}
#if LOCK_DEBUG
	printf(" V\n");
#endif
	return 1;
}

static int btree_data_lockr(btree_tree *t, uint32_t idx)
{
	return btree_data_lock_helper(t, idx, F_RDLCK);
}

static int btree_data_lockw(btree_tree *t, uint32_t idx)
{
	return btree_data_lock_helper(t, idx, F_WRLCK);
}

int btree_data_unlock(btree_tree *t, uint32_t idx)
{
	return btree_data_lock_helper(t, idx, F_UNLCK);
}


/* Allocations */
static btree_node *btree_get_node(btree_tree *t, uint32_t idx)
{
	return (btree_node*) (t->nodes + (idx * 4096));
}

static int btree_allocate(char *path, uint32_t order, uint32_t nr_of_items, size_t data_size)
{
	int fd;
	uint64_t bytes;
	char buffer[4096];
	uint32_t node_count = 0;

	/**
	 * Header:   4096
	 * Freelist: ceil((nr_of_items + 7) / 8)
	 * Nodes:    4096 * (nr_of_items / order)
	 * Data:     nr_of_items * (length-marker + ts + data_size + '\0' delimiter)
	 */
	node_count = NODE_COUNT(nr_of_items, order);
	bytes = BTREE_HEADER_SIZE +
		BTREE_FREELIST_SIZE(nr_of_items) +
		(node_count * 4096) +
		(nr_of_items * (BTREE_DATA_EXTRA + data_size));

	fd = open(path, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);

	memset(buffer, 0, 4096);
	/* FIXME: Check for signals */
	if (posix_fallocate(fd, 0, bytes)) {
		close(fd);
		unlink(path);
		return 0;
	}
	close(fd);
	return 1;
}

static btree_node *btree_allocate_node(btree_tree *t)
{
	btree_node *tmp_node;

	tmp_node = t->nodes + (t->header->next_node_idx * 4096);

	tmp_node->marker[0] = 'N';

	tmp_node->idx = t->header->next_node_idx;
	t->header->next_node_idx++;

	return tmp_node;
}

static int btree_open_file(btree_tree *t, char *path)
{
	int fd;
	struct stat fileinfo;

	stat(path, &fileinfo);
	fd = open(path, O_RDWR);
	if (fd == -1) {
		return errno;
	}
	t->fd = fd;
	t->mmap = mmap(NULL, fileinfo.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	t->file_size = fileinfo.st_size;
	t->header = (btree_header*) t->mmap;

	return 0;
}

btree_tree *btree_open(char *path)
{
	btree_tree *t;

	t = malloc(sizeof(btree_tree));
	if (btree_open_file(t, path) != 0) {
		free(t);
		return NULL;
	}

	t->freelist.size = t->header->max_items;
	t->freelist.setinfo = t->mmap + BTREE_HEADER_SIZE;

	t->nodes = t->mmap +
		BTREE_HEADER_SIZE +
		BTREE_FREELIST_SIZE(t->header->max_items);

	t->data = t->mmap +
		BTREE_HEADER_SIZE +
		BTREE_FREELIST_SIZE(t->header->max_items) +
		(t->header->node_count * 4096);

	t->root = btree_get_node(t, t->header->root_node_idx);

	return t;
}

static void btree_init(btree_tree *t)
{
	btree_node *tmp_node;

	t->header->version = 1;
	t->header->next_node_idx = 0;
	t->header->node_count = NODE_COUNT(t->header->max_items, t->header->order);

	t->freelist.size = t->header->max_items;
	t->freelist.setinfo = t->mmap + BTREE_HEADER_SIZE;
	dr_set_init(&(t->freelist));
	
	t->nodes = t->mmap +
		BTREE_HEADER_SIZE +
		BTREE_FREELIST_SIZE(t->header->max_items);

	t->data = t->mmap +
		BTREE_HEADER_SIZE +
		BTREE_FREELIST_SIZE(t->header->max_items) +
		(t->header->node_count * 4096);

	tmp_node = btree_allocate_node(t);
	tmp_node->leaf = 1;
	tmp_node->nr_of_keys = 0;

	t->root = tmp_node;
}

btree_tree *btree_create(char *path, uint32_t order, uint32_t nr_of_items, size_t data_size)
{
	btree_tree *tmp_tree;

	if (order > BTREE_MAX_ORDER) {
		order = BTREE_MAX_ORDER;
	}
	btree_allocate(path, order, nr_of_items, data_size);

	tmp_tree = malloc(sizeof(btree_tree));
	if (btree_open_file(tmp_tree, path) != 0) {
		free(tmp_tree);
		return NULL;
	}

	tmp_tree->path = path;
	tmp_tree->header->order = order;
	tmp_tree->header->max_items = nr_of_items;
	tmp_tree->header->item_size = data_size;

	btree_init(tmp_tree);

	return tmp_tree;
}

int btree_close(btree_tree *t)
{
	close(t->fd);
	return munmap(t->mmap, t->file_size) == 0;
}

void btree_empty(btree_tree *t)
{
	BT_LOCK;
	btree_init(t);
	BT_UNLOCK;
}

void btree_free(btree_tree *t)
{
	free(t);
}

inline static void *btree_get_data_location(btree_tree *t, uint32_t idx)
{
	return t->data + (idx * (t->header->item_size + BTREE_DATA_EXTRA));
}

void *btree_get_data(btree_tree *t, uint32_t idx, size_t *data_size, time_t *ts)
{
	void *location;

	location = btree_get_data_location(t, idx);
	*data_size = *((size_t*)location);
	*ts = *((time_t*) (location + sizeof(size_t)));
	return location + sizeof(size_t) + sizeof(time_t);
}

int btree_set_data(btree_tree *t, uint32_t idx, void *data, size_t data_size, time_t ts)
{
	void *location;

	if (data_size > t->header->item_size) {
		return 0;
	}

	BT_LOCK;
	location = btree_get_data_location(t, idx);
	*((size_t*)location) = data_size;
	*(time_t*) (location + sizeof(size_t)) = ts;
	memcpy(location + sizeof(size_t) + sizeof(time_t), data, data_size);
	BT_UNLOCK;
	return 1;
}

void btree_get_data_ptr(btree_tree *t, uint32_t idx, void **data, size_t **data_size, time_t **ts)
{
	void *location;

	location = btree_get_data_location(t, idx);
	*data = location + sizeof(size_t) + sizeof(time_t);
	*data_size = ((size_t*)location);
	*ts = (time_t*) (location + sizeof(size_t));
}

int btree_search_internal(btree_tree *t, btree_node *node, uint64_t key, uint32_t *idx)
{
	int i = 0;
	while (i < node->nr_of_keys && key > node->keys[i].key) {
		i++;
	}

	if (i < node->nr_of_keys && key == node->keys[i].key) {
		if (idx) {
			*idx = node->keys[i].idx;
		}
		return 1;
	}

	if (node->leaf) {
		return 0;
	} else {
		btree_node *tmp_node = btree_get_node(t, node->branch[i]);
		return btree_search_internal(t, tmp_node, key, idx);
	}
}

/* This function will lock the data index for writing FIXME: Should use R for normal finds */
int btree_search(btree_tree *t, btree_node *node, uint64_t key, uint32_t *idx)
{
	int retval;

	BT_LOCK;
	if (1 == (retval = btree_search_internal(t, node, key, idx))) {
		BT_LOCK_DATA_R(*idx);
	}
	BT_UNLOCK;
	return retval;
}

static void btree_split_child(btree_tree *t, btree_node *parent, uint32_t key_nr, btree_node *child)
{
	uint32_t j;

	btree_node *tmp_node = btree_allocate_node(t);
	tmp_node->leaf = child->leaf;
	tmp_node->nr_of_keys = BTREE_T(t) - 1;

	for (j = 0; j < BTREE_T(t) - 1; j++) {
		tmp_node->keys[j] = child->keys[j + BTREE_T(t)];
	}
	if (!child->leaf) {
		for (j = 0; j < BTREE_T(t); j++) {
			tmp_node->branch[j] = child->branch[j + BTREE_T(t)];
		}
	}
	child->nr_of_keys = BTREE_T(t) - 1;

	for (j = parent->nr_of_keys + 1; j > key_nr; j--) {
		parent->branch[j] = parent->branch[j - 1];
	}
	parent->branch[key_nr + 1] = tmp_node->idx;

	for (j = parent->nr_of_keys; j > key_nr; j--) {
		parent->keys[j] = parent->keys[j - 1];
	}
	parent->keys[key_nr] = child->keys[BTREE_T(t) - 1];
	parent->nr_of_keys++;
/*
	for (j = BTREE_T(t) - 1; j < child->nr_of_keys; j++) {
		child->keys[j].key = 0;
	}
*/
}

static void btree_insert_non_full(btree_tree *t, btree_node *node, uint64_t key, uint32_t data_idx)
{
	uint32_t i;
	btree_node *tmp_node;

	i = node->nr_of_keys;
	if (node->leaf) {
		while (i > 0 && key < node->keys[i - 1].key) {
			node->keys[i] = node->keys[i - 1];
			i--;
		}
		node->keys[i].key = key;
		node->nr_of_keys++;

		/* Fetch data index, and set it to the idx element here too */
		node->keys[i].idx = data_idx;

		/* Do administrative jobs */
		dr_set_add(&(t->freelist), data_idx);
		t->header->item_count++;
	} else {
		tmp_node = btree_find_branch(t, node, key, &i);
		if (tmp_node->nr_of_keys == BTREE_T2(t) - 1) {
			btree_split_child(t, node, i, tmp_node);
			if (key > node->keys[i].key) {
				i++;
			}
		}
		btree_insert_non_full(t, btree_get_node(t, node->branch[i]), key, data_idx);
	}
}

static btree_node* btree_find_branch(btree_tree *t, btree_node *node, uint64_t key, uint32_t *i)
{
	*i = node->nr_of_keys;
	while (*i > 0 && key < node->keys[(*i) - 1].key) {
		(*i)--;
	}
	return btree_get_node(t, node->branch[*i]);
}

static void btree_insert_internal(btree_tree *t, uint64_t key, uint32_t data_idx)
{
	btree_node *r = t->root;

	if (r->nr_of_keys == BTREE_T2(t) - 1) {
		btree_node *tmp_node;

		tmp_node = btree_allocate_node(t);
		t->root = tmp_node;
		t->header->root_node_idx = tmp_node->idx;
		tmp_node->leaf = 0;
		tmp_node->nr_of_keys = 0;
		tmp_node->branch[0] = r->idx;
		btree_split_child(t, tmp_node, 0, r);
		btree_insert_non_full(t, tmp_node, key, data_idx);
	} else {
		btree_insert_non_full(t, r, key, data_idx);
	}
}

/* This function will lock the data index for writing */
int btree_insert(btree_tree *t, uint64_t key, uint32_t *data_idx)
{
	btree_node *r = t->root;
	unsigned int tmp_data_idx;

	BT_LOCK;
	if (t->header->item_count >= t->header->max_items) {
		BT_UNLOCK;
		return 0;
	}
	if (btree_search_internal(t, r, key, NULL)) {
		BT_UNLOCK;
		return 0;
	}
	if (!dr_set_find_first(&(t->freelist), &tmp_data_idx)) {
		BT_UNLOCK;
		return 0;
	}
	btree_insert_internal(t, key, tmp_data_idx);
	*data_idx = tmp_data_idx;
	BT_LOCK_DATA_W(tmp_data_idx);
	BT_UNLOCK;
	return 1;
}

static btree_node *btree_find_smallest(btree_tree *t, btree_node *node)
{
	btree_node *ptr = node;
	while (!ptr->leaf) {
		ptr = btree_get_node(t, ptr->branch[0]);
	}
	return ptr;
}

static btree_node *btree_find_greatest(btree_tree *t, btree_node *node)
{
	btree_node *ptr = node;
	while (!ptr->leaf) {
		ptr = btree_get_node(t, ptr->branch[ptr->nr_of_keys]);
	}
	return ptr;
}

static unsigned int btree_check_key_in_node(btree_node *node, uint64_t key, uint32_t *idx, uint32_t *data_idx)
{
	int i = 0;
	while (i < node->nr_of_keys && key > node->keys[i].key) {
		i++;
	}
	if (i < node->nr_of_keys && key == node->keys[i].key) {
		*idx = i;
		*data_idx = node->keys[i].idx;
		return 1;
	}
	return 0;
}

/**
 * Removes the key/branch with index "node_idx" and shifts all other keys
 */
static void btree_delete_key_idx_from_node(btree_node *node, uint64_t idx)
{
	int i;

	for (i = idx; i < node->nr_of_keys - 1; i++) {
		node->keys[i] = node->keys[i + 1];
	}
	node->nr_of_keys--;
}

static void btree_delete_branch_idx_from_node(btree_node *node, uint64_t idx)
{
	int i;

	if (!node->leaf) {
		for (i = idx; i <= node->nr_of_keys; i++) {
			node->branch[i] = node->branch[i + 1];
		}
	}
}

static void btree_merge(btree_tree *t, btree_node *a, btree_node *x, uint32_t idx, btree_node *b)
{
	int i = a->nr_of_keys, j, k = a->nr_of_keys;

	a->keys[i] = x->keys[idx];
	a->nr_of_keys++;
	for (j = 0; j < b->nr_of_keys; j++) {
		i++;
		a->keys[i] = b->keys[j];
	}
	a->nr_of_keys += b->nr_of_keys;

	if (!x->leaf) {
		for (j = 0; j <= b->nr_of_keys; j++) {
			k++;
			a->branch[k] = b->branch[j];
		}
	}
}

static void btree_node_insert_key(btree_tree *t, btree_node *node, uint32_t pos, btree_key key)
{
	uint32_t i = node->nr_of_keys;

	while (i > pos) {
		node->keys[i] = node->keys[i-1];
		i--;
	}
	node->keys[pos] = key;
	node->nr_of_keys++;
}

static int btree_delete_internal(btree_tree *t, btree_node *node, uint64_t key_to_delete, uint64_t key, uint32_t *data_idx_to_free)
{
	uint32_t idx;
	uint32_t data_idx;

	/* if x is a leaf then
	 *   if k is in x then
	 *     delete k from x and return true
	 *   else return false //k is not in subtree
	 */
	if (node->leaf) {
		if (btree_check_key_in_node(node, key, &idx, &data_idx)) {
			btree_delete_key_idx_from_node(node, idx);
			btree_delete_branch_idx_from_node(node, idx);
			if (key == key_to_delete) {
				*data_idx_to_free = data_idx;
			}
			return 1;
		} else {
			return 0;
		}
	} else {
		/* if k is in x then */
		if (btree_check_key_in_node(node, key, &idx, &data_idx)) {
			btree_node *y, *node_with_prev_key;
			/* Record the data_idx for this key */
			if (key == key_to_delete) {
				*data_idx_to_free = data_idx;
			}
			/*   y = the child of x that precedes k
			 *   if y has at least t keys then
			 *     k' = the predecessor of k (use B-Tree-FindLargest)
			 *     Copy k' over k //i.e., replace k with k'
			 *     B-Tree-Delete(y, k') //Note: recursive call
			 */
			y = btree_get_node(t, node->branch[idx]);
			if (y->nr_of_keys >= BTREE_T(t)) {
				node_with_prev_key = btree_find_greatest(t, y);
				node->keys[idx] = node_with_prev_key->keys[y->nr_of_keys-1];
//				node->branch[idx] = node_with_prev_key->branch[y->nr_of_keys-1];
				return btree_delete_internal(t, y, key_to_delete, node_with_prev_key->keys[y->nr_of_keys-1].key, data_idx_to_free);
			} else {
				btree_node *z, *node_with_next_key;
			/*   else //y has t-1 keys
			 *     z = the child of x that follows k
			 *     if z has at least t keys then
			 *       k' = the successor of k
			 *       Copy k' over k //i.e., replace k with k'
			 *       B-Tree-Delete(z, k') //Note: recursive call
			 */
				z = btree_get_node(t, node->branch[idx + 1]);
				if (z->nr_of_keys >= BTREE_T(t)) {
					node_with_next_key = btree_find_smallest(t, z);
					node->keys[idx] = node_with_next_key->keys[0];
//					node->branch[idx] = node_with_next_key->branch[0];
					btree_delete_internal(t, z, key_to_delete, node_with_next_key->keys[0].key, data_idx_to_free);
				} else {
			/*     else //both y and z have t-1 keys
			 *       merge k and all of z into y //y now contains 2t-1 keys.
			 *       //k and the pointer to z will be deleted from x.
			 *       B-Tree-Delete(y, k) //Note: recursive call
			 */
					btree_merge(t, y, node, idx, z);
					btree_delete_key_idx_from_node(node, idx);
					btree_delete_branch_idx_from_node(node, idx + 1);
					btree_delete_internal(t, y, key_to_delete, key, data_idx_to_free);
					if (t->root->nr_of_keys == 0 && !t->root->leaf) {
						t->root = btree_get_node(t, t->root->branch[0]);
					}
				}
			}
		} else {
			btree_node *c;
			uint32_t i;

			c = btree_find_branch(t, node, key, &i);
			if (c->nr_of_keys <= BTREE_T(t) - 1) {
				btree_node *z, *node_with_prev_key, *node_with_next_key;
				btree_key tmp_key;

				/* Is there a left sibling with T or more keys? */
				if (i > 0) { /* otherwise there is no left sibling */
					z = btree_get_node(t, node->branch[i - 1]);
					if (z->nr_of_keys > BTREE_T(t) - 1) {
						node_with_prev_key = btree_find_greatest(t, z);
						btree_node_insert_key(t, c, 0, node_with_prev_key->keys[z->nr_of_keys-1]);
						btree_delete_internal(t, z, key_to_delete, node_with_prev_key->keys[z->nr_of_keys-1].key, data_idx_to_free);

						/* Swap parent and first key in C */
						tmp_key = node->keys[i-1];
						node->keys[i-1] = c->keys[0];
						c->keys[0] = tmp_key;
						goto proceed;
					}
				}

				/* Is there a left sibling with T or more keys? */
				if (i < node->nr_of_keys) { /* otherwise there is no right sibling */
					z = btree_get_node(t, node->branch[i + 1]);
					if (z->nr_of_keys > BTREE_T(t) - 1) {
						node_with_next_key = btree_find_smallest(t, z);
						btree_node_insert_key(t, c, c->nr_of_keys, node_with_next_key->keys[0]);
						btree_delete_internal(t, z, key_to_delete, node_with_next_key->keys[0].key, data_idx_to_free);

						/* Swap parent and last key in C */
						tmp_key = node->keys[i];
						node->keys[i] = c->keys[c->nr_of_keys - 1];
						c->keys[c->nr_of_keys - 1] = tmp_key;
						goto proceed;
					}
				}

				/* No siblings, so we need to merge. */
				/* Is there a left sibling? */
				if (i > 0) { /* otherwise there is no left sibling */
					z = btree_get_node(t, node->branch[i - 1]);
					btree_merge(t, z, node, i - 1, c);
					btree_delete_branch_idx_from_node(node, i);
					btree_delete_key_idx_from_node(node, i - 1);
					if (t->root->nr_of_keys == 0 && !t->root->leaf) {
						t->root = btree_get_node(t, t->root->branch[0]);
					}
					return btree_delete_internal(t, z, key_to_delete, key, data_idx_to_free);
				}
				/* Is there a right sibling? */
				if (i < node->nr_of_keys) { /* otherwise there is no right sibling */
					z = btree_get_node(t, node->branch[i + 1]);
					btree_merge(t, c, node, i, z);
					btree_delete_branch_idx_from_node(node, i + 1);
					btree_delete_key_idx_from_node(node, i);
					if (t->root->nr_of_keys == 0 && !t->root->leaf) {
						t->root = btree_get_node(t, t->root->branch[0]);
					}
					return btree_delete_internal(t, c, key_to_delete, key, data_idx_to_free);
				}
			}
proceed:
			return btree_delete_internal(t, c, key_to_delete, key, data_idx_to_free);
		}
	}
	return 0;
}

int btree_delete(btree_tree *t, uint64_t key)
{
	btree_node *n = t->root;
	uint32_t data_idx = 99999999; // the one to free

	BT_LOCK;
	if (btree_delete_internal(t, n, key, key, &data_idx)) {
		/* Do administrative jobs */
		t->header->item_count--;
		dr_set_remove(&(t->freelist), data_idx);
		BT_UNLOCK;
		return 1;
	}
	BT_UNLOCK;
	return 0;
}

static void btree_dump_node_dot(btree_tree *t, btree_node *node)
{
	int i;

	printf("\n\"IDX%d\" [\nlabel=\"{{", node->idx);
	for (i = 0; i < node->nr_of_keys; i++) {
		printf("%s%lu", i ? " | " : "", node->keys[i].key);
	}
	printf("} ");
	if (!node->leaf) {
		printf("| {");
		for (i = 0; i < node->nr_of_keys + 1; i++) {
			printf("%s<n%d>%d", i ? " | " : "", i, node->branch[i]);
		}
		printf("}}\"\n];\n");
		for (i = 0; i < node->nr_of_keys + 1; i++) {
			printf("\"IDX%d\":n%d->\"IDX%d\";\n", node->idx, i, node->branch[i]);
		}
		for (i = 0; i < node->nr_of_keys + 1; i++) {
			btree_dump_node_dot(t, btree_get_node(t, node->branch[i]));
		}
	} else {
		printf("}\"\n];\n");
	}
}

void btree_dump_dot(btree_tree *t)
{
	BT_LOCK;
	printf("digraph g {\ngraph [ rankdir = \"TB\" ];\nnode [ fontsize = \"16\" shape = \"record\" ];\n");
	btree_dump_node_dot(t, t->root);
	printf("}\n");
	BT_UNLOCK;
}


static void btree_dump_node_test(btree_tree *t, btree_node *node, int level)
{
	int i;

	printf("\n%*sIDX: %d: ", level * 2, "", node->idx);
	if (!node->leaf) {
		printf("(%d) ", node->branch[0]);
	}
	for (i = 0; i < node->nr_of_keys; i++) {
		printf("%lu ", node->keys[i].key);
		if (!node->leaf) {
			printf("(%d) ", node->branch[i+1]);
		}
	}
	if (!node->leaf) {
		for (i = 0; i < node->nr_of_keys + 1; i++) {
			btree_dump_node_test(t, btree_get_node(t, node->branch[i]), level + 1);
		}
	}
}

void btree_dump_test(btree_tree *t)
{
	BT_LOCK;
	btree_dump_node_test(t, t->root, 0);
	printf("\n");
	BT_UNLOCK;
}


static void btree_dump_node(btree_tree *t, btree_node *node)
{
	int i;

	printf("\nIDX: %d\n", node->idx);
	for (i = 0; i < node->nr_of_keys; i++) {
		printf("%9lu ", node->keys[i].key);
	}
	if (!node->leaf) {
		printf("\n");
		for (i = 0; i < node->nr_of_keys + 1; i++) {
			printf("%9d ", node->branch[i]);
		}
		for (i = 0; i < node->nr_of_keys + 1; i++) {
			btree_dump_node(t, btree_get_node(t, node->branch[i]));
		}
	}
}

void btree_dump(btree_tree *t)
{
	BT_LOCK;
	printf("-------\n");
	btree_dump_node(t, t->root);
	printf("\n-------\n");
	BT_UNLOCK;
}
