#ifndef SCC_STRUCTURES_H
#define SCC_STRUCTURES_H

#include <stdio.h>
#include <stdbool.h>

struct _linked_node {
	size_t node_ct;
	struct _linked_node *next;
};

struct _linked_list {
	struct _linked_node *head;
	struct _linked_node *tail;
};

struct _linked_list_vector_node {
	bool is_cyclic;
	struct _linked_list *list;
	struct _linked_list_vector_node *next;
};

struct _linked_list_vector {
	struct _linked_list_vector_node *head;
	struct _linked_list_vector_node *tail;
};

typedef struct _linked_node node;
typedef struct _linked_list_vector_node vector_node;
typedef struct _linked_list_vector vector_list;
typedef struct _linked_list list;

void pop(list *);
size_t top(list *);
void push(list *, size_t);
void push_back(list *, size_t);
void push_back_v(vector_list *, list *);
list **nodes();
void add_arc(size_t, size_t, size_t);
node *neighbours(size_t, size_t);
size_t size();
size_t levels();

list ***m_nodes;
size_t m_nr_nodes;
size_t m_nr_levels;
vector_list **components;

#endif /* SCC_STRUCTURES_H */
