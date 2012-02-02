#include "../set.h"

int main(void)
{
	dr_set *set;
	int i;

	for (i = 1; i < 33; i++) {
		set = dr_set_create(i);
		dr_set_dump(set);
		dr_set_free(set);
	}
}
