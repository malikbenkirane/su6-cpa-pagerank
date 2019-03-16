#include <stdlib.h>
#include <stdio.h>

#include "targets.h"
#include "sizeof_graph.h"

#define MIN_ALLOC 2048
#define ERR_ALLOC 100
#define ERR_FILE_OPEN 101

unsigned
sizeof_graph(ginfo_t *ginfo) {
	unsigned u, v;
	unsigned nl, nn;
	unsigned map_alloc;
	unsigned map_index;
	unsigned *map;
	unsigned *douts;
	FILE *fd;

	douts = (unsigned *)malloc(sizeof(*douts)*MIN_ALLOC);
	map = (unsigned *)malloc(sizeof(*map)*MIN_ALLOC);
	if (douts == NULL || map == NULL)
		return ERR_ALLOC;
	map_alloc = MIN_ALLOC;
	map_index = 0;
	nl = 0;
	nn = 0;
	for (unsigned i = 0; i < map_alloc; i++)
		douts[i] = 0;
	if ((fd = fopen(FILENAME, "r")) == NULL)
		return ERR_FILE_OPEN;
	while (fscanf(fd, "%u %u", &u, &v) == 2) {
		printf("read %u - %u", u, v);
		if (map_index > map_alloc - 1) {
			map_alloc += MIN_ALLOC;
			douts = realloc(douts, sizeof(*douts)*map_alloc);
			map = realloc(map, sizeof(*map)*map_alloc);
		}
		map[map_index] = u;
		if (douts[map_index++]++ > 0) nn++;
		map[map_index++] = v;
		nl++;
	}
	ginfo->nl = nl;
	ginfo->nn = nn;
	ginfo->map = map;
	ginfo->out_degrees = douts;
	return (0);
}

int
main(void)
{
	ginfo_t ginfo;
	int err;

	if ((err = sizeof_graph(&ginfo)) != 0)
		return err;
	printf("nl:%u\n", ginfo.nl);
	printf("nn:%u\n", ginfo.nn);
	return (0);
}
