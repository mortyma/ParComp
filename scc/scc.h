#ifndef SCC_H
#define SCC_H

#include "scc_structures.h"

int get_scc(char *file);

void print_scc();

bool is_cyclic(size_t lvl, size_t stmt_nr);

#endif /* SCC_H */
