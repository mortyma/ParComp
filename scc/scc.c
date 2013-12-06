#include <stdbool.h>
#include "scc_structures.h"
#include "graphstreamer.h"
#include <assert.h>

#define MAX 100

#define min(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a < _b ? _a : _b; })

list *stack;
bool visited[MAX+1];
bool on_stack[MAX+1];
size_t idx[MAX+1];
size_t low_link[MAX+1];
size_t cnt = 0;

void scc();

void SCC() {
	size_t nr_nodes = size();
	stack = malloc(sizeof(list));
	components = malloc(sizeof(vector_list));
	components->head = NULL;
	components->tail = NULL;
    	for(size_t i = 0; i < nr_nodes; i++) {
        	if(!visited[i]) {
            		scc(i);
        	}
    	}
}

void scc(size_t ct) {
    	push(stack, ct);
    	on_stack[ct] = true;
    	visited[ct] = true;
    	idx[ct] = cnt;
    	low_link[ct] = cnt++;
	
	node *neighbours_tmp = neighbours(ct);
	while(neighbours_tmp != NULL) {
		size_t neighbour = neighbours_tmp->node_ct;
        	if(!visited[neighbour]) {
            		scc(neighbour);
            		low_link[ct] = min(low_link[ct], low_link[neighbour]);
        	} else if(on_stack[neighbour]) {
           		low_link[ct] = min(low_link[ct], low_link[neighbour]);
        	}
		neighbours_tmp = neighbours_tmp->next;
    	}

    	if(low_link[ct] == idx[ct]) {
        	size_t tmp;
        	list* component = malloc(sizeof(list));
		component->head = NULL;
		component->tail = NULL;
        	do {
            		tmp = top(stack);
	            	push_back(component, tmp);
            		on_stack[tmp] = false;
            		pop(stack);
        	} while(tmp != ct);
        	push_back_v(components, component);
    	}
}

/*int main(void) {
	read_from("../scc.in");
	SCC();
	vector_node *components_tmp = components->head;
    	while(components_tmp != NULL) {
		node *component_tmp = components_tmp->list->head;
        	while(component_tmp != NULL) {
            		printf("%zu\n", component_tmp->node_ct+1);
			component_tmp = component_tmp->next;
        	}
        	printf("\n");
		components_tmp = components_tmp->next;
    	}

        return 0;
}*/
