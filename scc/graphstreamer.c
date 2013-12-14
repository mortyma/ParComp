#include <assert.h>
#include "graphstreamer.h"
#include "scc_structures.h"

void read_from(char *file) {

    	size_t from, to, level, type;
    	size_t nr_nodes, nr_edges, nr_levels;

    	FILE *fp;
	fp = fopen(file, "r");

	fscanf(fp, "%zu %zu %zu\n", &nr_nodes, &nr_edges, &nr_levels);
	stmt_array = (node **)malloc(nr_nodes*sizeof(node *));

	m_nodes = (list_lists ***)malloc((nr_levels+1)*sizeof(list_lists **));
	
	for(size_t i = 0; i < nr_levels+1; i++) {
    		m_nodes[i] = (list_lists **)malloc(nr_nodes*sizeof(list_lists *));
		for(size_t j = 0; j < nr_nodes; j++) {
			m_nodes[i][j] = (list_lists *)malloc(sizeof(list_lists)); 
		}
	}
	m_nr_nodes = nr_nodes;
	m_nr_levels = nr_levels;

	while(!feof(fp)) {
		fscanf(fp, "%zu %zu %zu %zu\n", &from, &to, &level, &type);
        	add_arc(from-1, to-1, level);
    	}
	fclose(fp);
}

