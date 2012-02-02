#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include "set.h"

dr_set *dr_set_create(unsigned int size)
{
	dr_set *tmp;
	unsigned int bytes, i;

	if (size < 1) {
		return NULL;
	}

	tmp = calloc(1, sizeof(dr_set));
	tmp->size = size;
	bytes = ceil((size + 7) / 8);
	tmp->setinfo = calloc(1, bytes);

	// mass unset everything but the last byte
	memset(tmp->setinfo, 0xff, size / 8);

	// unset bits in the last byte
	for (i = 0; i < size % 8; i++) {
		dr_set_remove(tmp, i + (size / 8) * 8);
	}

	return tmp;
}

void dr_set_free(dr_set *set)
{
	free(set->setinfo);
	free(set);
}


void dr_set_dump(dr_set *set)
{
	unsigned int byte, bit; 

	printf("SIZE: %d\n", set->size);
	for (byte = 0; byte < ceil((set->size + 7) / 8); byte++) {
		printf("BYTE: %d (%0x)\n", byte, set->setinfo[byte]);
		for (bit = 0; bit < 8; bit++) {
			printf(" %c", set->setinfo[byte] & (1 << bit) ? '1' : '0');
		}
		printf("\n");
	}
	printf("\n");
}



inline void dr_set_add(dr_set *set, unsigned int position)
{
	unsigned char *byte;
	unsigned int   bit;

	if (position > set->size) {
		return;
	}

	byte = &(set->setinfo[position / 8]);
	bit  = position % 8;

	*byte = *byte & ~(1 << bit);
}

inline void dr_set_remove(dr_set *set, unsigned int position)
{
	unsigned char *byte;
	unsigned int   bit;

	if (position > set->size) {
		return;
	}

	byte = &(set->setinfo[position / 8]);
	bit  = position % 8;

	*byte = *byte | (1 << bit);
}


inline int dr_set_in(dr_set *set, unsigned int position)
{
	unsigned char *byte;
	unsigned int   bit;

	if (position > set->size) {
		return 0;
	}

	byte = &(set->setinfo[position / 8]);
	bit  = position % 8;

	return (*byte & (1 << bit));
}

inline int dr_set_find_first(dr_set *set, unsigned int *position)
{
	unsigned int i, j;

	for (i = 0; i < ((set->size + 7) / 8); i++) {
		if (set->setinfo[i] != 0x0) {
			for (j = 0; j < 8; j++) {
				if (dr_set_in(set, i * 8 + j)) {
					*position = i * 8 + j;
					return 1;
				}
			}
		}
	}

	return 0;
}
