#include "graphstreamer.h"
#include "scc_structures.h"
#include <assert.h>

void read_from(char *file) {

    	size_t from, to;
    	size_t nr_nodes, nr_edges;

    	FILE *fp;
	fp = fopen(file, "r");
	//list **m_nodes;

	fscanf(fp, "%zu %zu\n", &nr_nodes, &nr_edges);
	m_nodes = (list **)malloc(nr_nodes*sizeof(list *));
	
	for(size_t i = 0; i < nr_nodes; i++) {
    		m_nodes[i] = (list *)malloc(sizeof(list));
	}
	m_nr_nodes = nr_nodes;

	while(!feof(fp)) {
		fscanf(fp, "%zu %zu\n", &from, &to);
        	add_arc(from-1, to-1);
    	}
	fclose(fp);
}

