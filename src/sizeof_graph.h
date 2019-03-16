#ifndef __SIZEOF_GRAPH_H__
#define __SIZEOF_GRAPH_H__

typedef struct {
	unsigned nl;       // number of links
	unsigned nn;       // number of nodes
	unsigned *in;      // in degrees array
	unsigned *out;     // out degrees array
#ifdef MAPNODES
	unsigned *map;     // map to actual node ids array
	unsigned *revmap;  // map actual ids to virtual ids
#endif
} ginfo_t;


unsigned sizeof_graph(ginfo_t* ginfo);

#endif
