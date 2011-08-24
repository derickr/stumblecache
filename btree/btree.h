int btree_search(btree_node *node, uint64_t key, uint32_t *idx);
btree_tree *btree_allocate(uint32_t nr_of_items, uint32_t data_size);
btree_tree *btree_create(void);
