#include "../set.h"
#include <stdio.h>

int main(void)
{
	dr_set *set;

	set = dr_set_create(33);

	printf("IN SET: %s\n", dr_set_in(set, 4) ? "YES" : "NO");
	dr_set_add(set, 4);
	printf("IN SET: %s\n", dr_set_in(set, 4) ? "YES" : "NO");
	dr_set_remove(set, 4);
	printf("IN SET: %s\n", dr_set_in(set, 4) ? "YES" : "NO");

	dr_set_dump(set);
	dr_set_free(set);

	return 0;
}
