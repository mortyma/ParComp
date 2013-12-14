#ifndef _SCC_H
#define _SCC_H

#include "scc_structures.h"

void init_scc(char *file);

vector_list *get_SCC(list *stmts, int lv);

#endif /* _SCC_H */
