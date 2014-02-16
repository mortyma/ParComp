#include "graphstreamer.h"
#include "scc.h"
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

void scc(size_t lv, node *stmt, vector_list *components, list *stmts) {
    	push(stack, stmt);
    	on_stack[stmt->node_ct] = true;
    	visited[stmt->node_ct] = true;
    	idx[stmt->node_ct] = cnt;
    	low_link[stmt->node_ct] = cnt++;
	bool self_loop = false;
	int not_found = 1;
	list_node *neighbours_tmp = neighbours(stmt->node_ct, lv);
	while(neighbours_tmp != NULL) {
		node *tmp = stmts->head;
		while(tmp != NULL) {
			if(tmp->node_ct == neighbours_tmp->node->node_ct) {
				not_found = 0;
				break;
			}
			tmp = tmp->next;
		}
		if(not_found) {
			neighbours_tmp = neighbours_tmp->next;
			continue;
		}
		//printf("%zu %zu\n", stmt->node_ct+1, neighbours_tmp->node_ct+1);
		if(neighbours_tmp->node->node_ct == stmt->node_ct) {
		//printf("self_loop %zu\n", neighbours_tmp->node->node_ct+1);
			self_loop = true;
		}
        	if(!visited[neighbours_tmp->node->node_ct]) {
            		scc(lv, neighbours_tmp->node, components, stmts);
            		low_link[stmt->node_ct] = min(low_link[stmt->node_ct], low_link[neighbours_tmp->node->node_ct]);
        	} else if(on_stack[neighbours_tmp->node->node_ct]) {
           		low_link[stmt->node_ct] = min(low_link[stmt->node_ct], low_link[neighbours_tmp->node->node_ct]);
        	}
		neighbours_tmp = neighbours_tmp->next;
    	}

    	if(low_link[stmt->node_ct] == idx[stmt->node_ct]) {
		//printf("in %zu\n", stmt->node_ct+1);
        	list* component = malloc(sizeof(list));
		component->head = NULL;
		component->tail = NULL;
		int count = 0;
        	node *tmp_stmt;
		size_t tmp_stmt_ct;
		do {
			count++;
            		tmp_stmt = top(stack);
			tmp_stmt_ct = tmp_stmt->node_ct;
	            	push_back_m(component, tmp_stmt);
			//printf("%zu\n", tmp_stmt->node_ct+1);
            		on_stack[tmp_stmt->node_ct] = false;
            		pop(stack);
		} while(tmp_stmt_ct != stmt->node_ct);
		//printf("component\n");
        	push_back_v(components, component);
		if(count > 1 || self_loop) {
			components->tail->is_cyclic = true;
		}
		else {
			components->tail->is_cyclic = false;	
		}
	}
}

vector_list *get_SCC(list *stmts, int lv) {
	
	node *tmp = stmts->tail;
	cnt = 0;
	stack = malloc(sizeof(list));
	vector_list *components = (vector_list *)malloc(sizeof(vector_list));
	components->head = NULL;
	components->tail = NULL;
	components->nr_nodes = 0;
	
	for(size_t i = 0; i < MAX+1; i++) {
		visited[i] = false;
		on_stack[i] = false;
		idx[i] = 0;
		low_link[i] = 0;
	}
	while(tmp != NULL) {
        	if(!visited[tmp->node_ct]) {
        		scc(lv, tmp, components, stmts);
        	}
		tmp = tmp->prev;
    	}
	free(stack);
	return components;
}

void init_scc(char *file) {
    read_from(file);
}
