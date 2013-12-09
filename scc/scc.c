#include "scc.h"
#include <stdbool.h>
#include "graphstreamer.h"
#include <assert.h>

#define MAX 100

#define btoa(x) ((x)?"true":"false")

#define min(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a < _b ? _a : _b; })

list *stack;
bool visited[MAX+1][MAX+1];
bool on_stack[MAX+1][MAX+1];
size_t idx[MAX+1][MAX+1];
size_t low_link[MAX+1][MAX+1];
size_t cnt = 0;

void scc();


void SCC() {
	size_t nr_nodes = size();
	size_t nr_levels = levels();
	stack = malloc(sizeof(list));
	components = (vector_list **)malloc(nr_levels*sizeof(vector_list *));
	size_t i;
	for(i = 0; i < nr_levels; i++) {
		components[i] = malloc(sizeof(vector_list));
		components[i]->head = NULL;
		components[i]->tail = NULL;
	}
	for(i = 1; i < nr_levels; i++) {
			for(size_t j = 0; j < nr_nodes; j++) {
				if(!visited[i][j]) {
						scc(i, j);
				}
			}
	}
}

void scc(size_t lv, size_t ct) {
		push(stack, ct);
		on_stack[lv][ct] = true;
		visited[lv][ct] = true;
		idx[lv][ct] = cnt;
		low_link[lv][ct] = cnt++;

	node *neighbours_tmp = neighbours(ct, lv);
	while(neighbours_tmp != NULL) {
		size_t neighbour = neighbours_tmp->node_ct;
			if(!visited[lv][neighbour]) {
					scc(lv, neighbour);
					low_link[lv][ct] = min(low_link[lv][ct], low_link[lv][neighbour]);
			} else if(on_stack[lv][neighbour]) {
				low_link[lv][ct] = min(low_link[lv][ct], low_link[lv][neighbour]);
			}
		neighbours_tmp = neighbours_tmp->next;
		}

		if(low_link[lv][ct] == idx[lv][ct]) {
			size_t tmp;
			list* component = malloc(sizeof(list));
		component->head = NULL;
		component->tail = NULL;
		int count = 0;
			do {
			count++;
					tmp = top(stack);
					push_back(component, tmp);
					on_stack[lv][tmp] = false;
					pop(stack);
			} while(tmp != ct);
			push_back_v(components[lv], component);
		if(count > 1) {
			components[lv]->tail->is_cyclic = true;
		}
		else {
			components[lv]->tail->is_cyclic = false;
		}
		}
}

int get_scc(char *file) {
	printf("foobar\n");
	read_from("scc/scc.in");
	SCC();
	return 1; //components;
}

void print_scc() {
	size_t nr_levels = levels();
		for(size_t i = 0; i < nr_levels; i++) {
			printf("Level: %zu\n", i);
			vector_node *components_tmp = components[i]->head;
				while(components_tmp != NULL) {
				node *component_tmp = components_tmp->list->head;
					while(component_tmp != NULL) {
							printf("cyclic: %s %zu\n", btoa(components_tmp->is_cyclic), component_tmp->node_ct+1);
					component_tmp = component_tmp->next;
					}
					printf("\n");
				components_tmp = components_tmp->next;
				}
		}
}

bool is_cyclic(size_t lvl, size_t stmt_nr) {
	vector_node *components_tmp = components[lvl]->head;
	while(components_tmp != NULL) {
		node *component_tmp = components_tmp->list->head;
		while(component_tmp != NULL) {
			if(component_tmp->node_ct == stmt_nr) {
				return components_tmp->is_cyclic;
			}
			component_tmp = component_tmp->next;
		}
		components_tmp = components_tmp->next;
	}
	assert(0);
	return false;
}


/*int main(void) {
	read_from("../scc.in");
	SCC();

	size_t nr_levels = levels();
	for(size_t i = 0; i < nr_levels; i++) {
		printf("Level: %zu\n", i);
		vector_node *components_tmp = components[i]->head;
    		while(components_tmp != NULL) {
			node *component_tmp = components_tmp->list->head;
        		while(component_tmp != NULL) {
            			printf("cyclic: %s %zu\n", btoa(components_tmp->is_cyclic), component_tmp->node_ct+1);
				component_tmp = component_tmp->next;
        		}
        		printf("\n");
			components_tmp = components_tmp->next;
    		}
	}

        return 0;
}*/
