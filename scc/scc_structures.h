#ifndef SCC_STRUCTURES_H
#define SCC_STRUCTURES_H

#include <stdio.h>
#include <stdbool.h>
#include "../efl_ir.h"

struct _linked_node {
	size_t node_ct;
	N_ITER *loop;
	N_ASSIGN *assign;
	struct _linked_node *next;
	struct _linked_node *prev;
};

struct _linked_list {
	struct _linked_node *head;
	struct _linked_node *tail;
};

struct _linked_list_node {
	struct _linked_node *node;
	struct _linked_list_node *next;
};

struct _linked_list_lists {
	struct _linked_list_node *head;
	struct _linked_list_node *tail;
};

struct _linked_list_vector_node {
	bool is_cyclic;
	struct _linked_list *list;
	struct _linked_list_vector_node *next;
	struct _linked_list_vector_node *prev;
};

struct _linked_list_vector {
	int nr_nodes;
	bool has_cycles;
	struct _linked_list_vector_node *head;
	struct _linked_list_vector_node *tail;
};

typedef struct _linked_node node;
typedef struct _linked_list_vector_node vector_node;
typedef struct _linked_list_vector vector_list;
typedef struct _linked_list list;
typedef struct _linked_list_node list_node;
typedef struct _linked_list_lists list_lists;

void pop(list *);
node* top(list *);
void push(list *, node *);
void push_back(list *, size_t);
void push_back_v(vector_list *, list *);
void push_back_t(list *, size_t, N_ITER *, N_ASSIGN *);
void push_back_m(list *, node *);
void push_back_k(list_lists *, node *);
list_lists **nodes();
void add_arc(size_t, size_t, size_t);
list_node *neighbours(size_t, size_t);
void update_nodes(list *);
size_t size();
size_t levels();

list_lists ***m_nodes;
node **stmt_array;
size_t m_nr_nodes;
size_t m_nr_levels;

#endif /* SCC_STRUCTURES_H */
