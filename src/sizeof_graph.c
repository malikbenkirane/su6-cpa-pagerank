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
#ifdef MAPNODES
	unsigned *map, *revmap;
	unsigned mi_max; // map index, map max index
	unsigned ri, ri_max; // max node index, revmap max index
#else
	unsigned imax;
	unsigned maxi;
#endif
	unsigned *out;
	unsigned *in;
	FILE *fd;

	out = (unsigned *)malloc(sizeof(*out) * MIN_ALLOC);
	in = (unsigned *)malloc(sizeof(*in) * MIN_ALLOC);
#ifdef MAPNODES
	map = (unsigned *)malloc(sizeof(*map) * MIN_ALLOC);
	revmap = (unsigned *)malloc(sizeof(*revmap) * MIN_ALLOC);
	if (out == NULL || in == NULL || map == NULL || revmap == NULL)
		return ERR_ALLOC;
#else
	if (out == NULL || in == NULL)
		return ERR_ALLOC;
#endif
	for (unsigned i = 0; i < MIN_ALLOC; i++) {
		out[i] = 0;
		in[i] = 0;
#ifdef MAPNODES
		map[i] = 0;
		revmap[i] = 0;
#endif
	}
#ifdef MAPNODES
	mi_max = MIN_ALLOC - 1;
	ri_max = MIN_ALLOC - 1;
	ri = 0;
#else
	maxi = MIN_ALLOC - 1;
	imax = 0;
#endif
	nl = 0;
	nn = 0;
	if ((fd = fopen(FILENAME, "r")) == NULL)
		return ERR_FILE_OPEN;
	while (fscanf(fd, "%u %u", &u, &v) == 2) {
#ifdef MAPNODES
		if (nn > mi_max) {
			mi_max += MIN_ALLOC;
			out = realloc(out, sizeof(*out) * (mi_max+1));
			in = realloc(in, sizeof(*in) * (mi_max+1));
			map = realloc(map, sizeof(*map) * (mi_max+1));
		}
		if (u > ri) ri = u;
		if (v > ri) ri = v;
		if (ri > ri_max) {
			ri_max = ri;
			revmap = realloc(revmap, sizeof(*revmap) * (ri_max+1));
		}
#else
		if (u > imax) imax = u;
		if (v > imax) imax = v;
		if (imax > maxi) {
			maxi = imax;
			out = realloc(out, sizeof(*out) * (maxi+1));
			in = realloc(in, sizeof(*in) * (maxi+1));
		}
#endif
#ifdef MAPNODES
		if (revmap[u] == 0) {
			map[nn] = u;
			revmap[u] = nn;
			nn++;
		}
		if (revmap[v] == 0) {
			map[nn] = u;
			revmap[v] = nn;
			nn++;
		}
		out[revmap[u]]++;
		in[revmap[v]]++;
#else
		if (in[u] == 0 && out[u] == 0) nn++;
		if (in[v] == 0 && out[v] == 0) nn++;
		out[u]++;
		in[v]++;
#endif
		nl++;
	}
	ginfo->nl = nl;
	ginfo->nn = nn;
#ifdef MAPNODES
	ginfo->map = map;
	ginfo->revmap = revmap;
#endif
	ginfo->in = in;
	ginfo->out = out;
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
	while (1);
	return (0);
}
#endif
