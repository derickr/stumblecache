#define BTREE_T    102
#define BTREE_T2   204

typedef struct {
	uint64_t key;
	uint32_t idx;       /* index into data portion */
	uint32_t expire_ts; /* if 0, then not in use */
} btree_key; /* 16 bytes */

typedef struct {
	uint32_t      idx;        /* the node's index into the data store */
	uint32_t      branch[BTREE_T2];
	btree_key     keys[BTREE_T2 - 1];
	char          leaf;       /* whether it's a leaf node or not */
	uint16_t      nr_of_keys; /* number of keys in use */
} btree_node; /* 204*4 + 203*16 + 1 + 2 = 4067, which fits nicely in a page */

typedef struct {
	btree_node *root;
} btree;
