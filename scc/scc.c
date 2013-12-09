#include "scc_structures.h"
#include "graphstreamer.h"
#include <assert.h>

#define MAX 100

#define btoa(x) ((x)?"true":"false")

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

void scc(size_t lv, size_t ct, vector_list *components) {
    	push(stack, ct);
    	on_stack[ct] = true;
    	visited[ct] = true;
    	idx[ct] = cnt;
    	low_link[ct] = cnt++;
	
	node *neighbours_tmp = neighbours(ct, lv);
	while(neighbours_tmp != NULL) {
		size_t neighbour = neighbours_tmp->node_ct;
        	if(!visited[neighbour]) {
            		scc(lv, neighbour, components);
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
		int count = 0;
        	do {
			count++;
            		tmp = top(stack);
	            	push_back(component, tmp);
            		on_stack[tmp] = false;
            		pop(stack);
        	} while(tmp != ct);
        	push_back_v(components, component);
		if(count > 1) {
			components->tail->is_cyclic = true;	
		}
		else {
			components->tail->is_cyclic = false;	
		}
    	}
}

vector_list *get_SCC(list *stmts, int lv) {
	
	size_t ct;
	node *tmp = stmts->head;
	stack = malloc(sizeof(list));
	vector_list *components = (vector_list *)malloc(sizeof(vector_list));
	components->head = NULL;
	components->tail = NULL;
	while(tmp != NULL) {
		ct = tmp->node_ct;
        	if(!visited[ct]) {
        		scc(lv, ct, components);
        	}
		tmp = tmp->next;
    	}
	free(stack);
	return components;
}

/*
int main(void) {
	read_from("../scc.in");
	list *stmts = malloc(sizeof(list));
	stmts->head = NULL;
	stmts->tail = NULL;
	push_back(stmts, 1);
	push_back(stmts, 2);
	push_back(stmts, 3);
	push_back(stmts, 4);
	vector_list *components = get_SCC(stmts, 1);

	size_t nr_levels = levels();
		vector_node *components_tmp = components->head;
    		while(components_tmp != NULL) {
			node *component_tmp = components_tmp->list->head;
        		while(component_tmp != NULL) {
            			printf("cyclic: %s %zu\n", btoa(components_tmp->is_cyclic), component_tmp->node_ct+1);
				component_tmp = component_tmp->next;
        		}
        		printf("\n");
			components_tmp = components_tmp->next;
    		}
        return 0;
}*/
