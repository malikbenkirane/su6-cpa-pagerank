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
	unsigned *map, *revmap;
	unsigned mi, mi_max; // map index, map max index
	unsigned ri, ri_max; // max node index, revmap max index
	unsigned *out;
	FILE *fd;

	out = (unsigned *)malloc(sizeof(*out) * MIN_ALLOC);
	map = (unsigned *)malloc(sizeof(*map) * MIN_ALLOC);
	revmap = (unsigned *)malloc(sizeof(*revmap) * MIN_ALLOC);
	if (out == NULL || map == NULL || revmap == NULL)
		return ERR_ALLOC;
	for (unsigned i = 0; i < MIN_ALLOC; i++) {
		out[i] = 0;
		map[i] = 0;
		revmap[i] = 0;
	}
	mi_max = MIN_ALLOC - 1;
	ri_max = MIN_ALLOC - 1;
	ri = 0;
	mi = 0;
	nl = 0;
	nn = 0;
	if ((fd = fopen(FILENAME, "r")) == NULL)
		return ERR_FILE_OPEN;
	while (fscanf(fd, "%u %u", &u, &v) == 2) {
		if (mi > mi_max) {
			mi_max += MIN_ALLOC;
			out = realloc(out, sizeof(*out) * (mi_max+1));
			map = realloc(map, sizeof(*map) * (mi_max+1));
		}
		if (u > ri) ri = u;
		if (v > ri) ri = v;
		if (ri > ri_max) {
			ri_max = ri;
			revmap = realloc(revmap, sizeof(*revmap) * (ri_max+1));
		}
		// count nodes
		if (revmap[u] == 0) nn++;
		if (revmap[v] == 0) nn++;
		// register u
		map[mi] = u;
		out[mi]++;
		revmap[u] = mi++;
		// register v (out degree 0)
		map[mi] = v;
		revmap[v] = mi++;
		// count link
		nl++;
	}
	ginfo->nl = nl;
	ginfo->nn = nn;
	ginfo->map = map;
	ginfo->out_degrees = out;
	return (0);
}

#ifdef TEST_SIZEOF
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
#endif
