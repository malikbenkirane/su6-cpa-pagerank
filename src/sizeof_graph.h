#ifndef __SIZEOF_GRAPH_H__
#define __SIZEOF_GRAPH_H__

typedef struct {
	unsigned nl;     // number of links
	unsigned nn;     // number of nodes
	unsigned *exit;  // out degree array
	unsigned *map;   // map to actual node ids array
} ginfo_t;


unsigned sizeof_graph(ginfo_t* ginfo);

#endif
