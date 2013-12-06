#include <stdlib.h>
#include <assert.h>
#include "scc_structures.h"

list **m_nodes;

size_t top(list *stack) {
	if(stack->head == NULL) {
		return -1;
	}
	return stack->head->node_ct;
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

void push(list *stack, size_t ct) {
	if(stack->head == NULL) {
		stack->head = malloc(sizeof(node));
		stack->head->node_ct = ct;
		stack->tail = stack->head;
		stack->head->next = NULL;
	}
	else {
		node *tmp;
		tmp = malloc(sizeof(node));
		tmp->node_ct = ct;
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
	}
	else {
		components->tail->next = malloc(sizeof(vector_node));
		components->tail->next->list = component;
		components->tail = components->tail->next;
		components->tail->next = NULL;
	}
}

list** nodes() {
	return m_nodes;
}

void add_arc(size_t from, size_t to) {
	assert(from < m_nr_nodes);
    	assert(to < m_nr_nodes);
    	push_back(m_nodes[from], to);
}

node* neighbours(size_t ct) {
	assert(ct < m_nr_nodes);
    	return (m_nodes[ct])->head;
}
size_t size() {
    return m_nr_nodes;
}
