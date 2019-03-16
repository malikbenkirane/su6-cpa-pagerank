#ifndef __SIZEOF_GRAPH_H__
#define __SIZEOF_GRAPH_H__

typedef struct {
	unsigned nl;
	unsigned nn;
	unsigned *out_degrees;
	unsigned *map;
} ginfo_t;


unsigned sizeof_graph(ginfo_t* ginfo);

#endif
