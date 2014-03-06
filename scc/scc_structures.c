#include <stdlib.h>
#include <assert.h>
#include "scc_structures.h"

list_lists ***m_nodes;

node* top(list *stack) {
	if(stack->head == NULL) {
		return NULL;
	}
	return stack->head;
}

void pop(list *stack) {
	node *tmp;

	tmp = stack->head;
	stack->head = stack->head->next;
	if(stack->head == NULL) {
		stack->tail = NULL;
	}
	free(tmp);
}

void push(list *stack, node * stack_node) {
	if(stack->head == NULL) {
		stack->head = malloc(sizeof(node));
		stack->head->node_ct = stack_node->node_ct;
		stack->head->loop = stack_node->loop;
		stack->head->assign = stack_node->assign;
		stack->tail = stack->head;
		stack->head->next = NULL;
	}
	else {
		node *tmp;
		tmp = malloc(sizeof(node));
		tmp->node_ct = stack_node->node_ct;
		tmp->loop = stack_node->loop;
		tmp->assign = stack_node->assign;
		tmp->next = stack->head;
		stack->head = tmp;
	}
}

void push_back(list *vector, size_t ct) {
	if(vector->head == NULL) {
		vector->head = malloc(sizeof(node));
		vector->head->node_ct = ct;
		vector->tail = vector->head;
		vector->head->next = NULL;
	}
	else {
		vector->tail->next = malloc(sizeof(node));
		vector->tail->next->node_ct = ct;
		vector->tail = vector->tail->next;
		vector->tail->next = NULL;
	}
}

void push_back_v(vector_list *components, list *component) {
	if(components->head == NULL) {
		components->head = malloc(sizeof(vector_node));
		components->head->list = component;
		components->tail = components->head;
		components->head->next = NULL;
		components->head->prev = NULL;
	}
	else {
		components->tail->next = malloc(sizeof(vector_node));
		components->tail->next->list = component;
		components->tail->next->prev = components->tail;
		components->tail = components->tail->next;
		components->tail->next = NULL;
	}
	components->nr_nodes++;
}

void push_back_t(list *vector, size_t ct, N_ITER* loop, N_ASSIGN *assign) {
	if(vector->head == NULL) {
		vector->head = malloc(sizeof(node));
		vector->head->node_ct = ct;
		vector->head->loop = loop;
		vector->head->assign = assign;
		vector->tail = vector->head;
		vector->head->next = NULL;
		vector->head->prev = NULL;
	}
	else {
		vector->tail->next = malloc(sizeof(node));
		vector->tail->next->node_ct = ct;
		vector->tail->next->prev = vector->tail;
		vector->tail->next->loop = loop;
		vector->tail->next->assign = assign;
		vector->tail = vector->tail->next;
		vector->tail->next = NULL;
	}
}

void push_back_m(list *vector, node *vector_node) {
	if(vector->head == NULL) {
		vector->head = malloc(sizeof(node));
		vector->head->node_ct = vector_node->node_ct;
		vector->head->loop = vector_node->loop;
		vector->head->assign = vector_node->assign;
		vector->tail = vector->head;
		vector->head->next = NULL;
		vector->head->prev = NULL;
	}
	else {
		/*vector->tail->next = malloc(sizeof(node));
		vector->tail->next->node_ct = vector_node->node_ct;
		vector->tail->next->prev = vector->tail;
		vector->tail->next->loop = vector_node->loop;
		vector->tail->next->assign = vector_node->assign;
		vector->tail = vector->tail->next;
		vector->tail->next = NULL;*/
		vector->head->prev = malloc(sizeof(node));
		vector->head->prev->node_ct = vector_node->node_ct;
		vector->head->prev->next = vector->head;
		vector->head = vector->head->prev;
		vector->head->loop = vector_node->loop;
		vector->head->assign = vector_node->assign;
		vector->head->prev = NULL;
	}
}

void push_back_k(list_lists *vector, node *vector_node) {
	if(vector->head == NULL) {
		vector->head = malloc(sizeof(list_node));
		vector->head->node = vector_node;
		vector->tail = vector->head;
		vector->head->next = NULL;
	}
	else {
		vector->tail->next = malloc(sizeof(list_node));
		vector->tail->next->node = vector_node;
		vector->tail = vector->tail->next;
		vector->tail->next = NULL;

	}
}
list_lists** nodes(size_t level) {
	return m_nodes[level];
}

void add_arc(size_t from, size_t to, size_t level) {
	assert(from < m_nr_nodes);
    	assert(to < m_nr_nodes);
	if(stmt_array[from] == NULL) {
		stmt_array[from] = malloc(sizeof(node));
		stmt_array[from]->node_ct = from;
		stmt_array[from]->loop = NULL;
		stmt_array[from]->assign = NULL;
		stmt_array[from]->next = NULL;
	}
	if(stmt_array[to] == NULL) {
		stmt_array[to] = malloc(sizeof(node));
		stmt_array[to]->node_ct = to;
		stmt_array[to]->loop = NULL;
		stmt_array[to]->assign = NULL;
		stmt_array[to]->next = NULL;
	}
	//for(size_t i = 1; i <= level; i++) {
    		push_back_k(m_nodes[level][from], stmt_array[to]);
	//}
}

void update_nodes(list* stmts) {
	node *tmp = stmts->head;
	while(tmp != NULL) {
		if(stmt_array[tmp->node_ct] != NULL) {
			stmt_array[tmp->node_ct]->loop = tmp->loop;
			stmt_array[tmp->node_ct]->assign = tmp->assign;
		}
		tmp = tmp->next;
	}
}

list_node* neighbours(size_t ct, size_t level) {
    	if(level > m_nr_levels) {
		return NULL; 
	}
	return (m_nodes[level][ct])->head;
}
size_t size() {
    return m_nr_nodes;
}
size_t levels() {
    return m_nr_levels;
}
