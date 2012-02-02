#ifndef __DR_SET_H__
#define __DR_SET_H__

typedef struct _dr_set {
	unsigned int size;
	unsigned char *setinfo;
} dr_set;

dr_set *dr_set_create(unsigned int size);
void dr_set_free(dr_set *set);

void dr_set_add(dr_set *set, unsigned int position);
void dr_set_remove(dr_set *set, unsigned int position);

int dr_set_in(dr_set *set, unsigned int position);
int dr_set_find_first(dr_set *set, unsigned int *position);

void dr_set_dump(dr_set *set);

#endif
