/*185.A64 Compilers for Parallel Systems WS 2013/14 H.Moritsch
   EFL to Fortran 90
*/


#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "scc/scc.h"
#include "efl_ir.h"

#define MAX 100
int idepth  = 0;
size_t lvl = 0;
N_ASSIGN *assigments[MAX+1];

void indent(int d) {
    int i;
    for (i=0; i<4*d; i++) 
        printf(" ");
    }

void gen_decls(ENTRY ** sym) {
	ENTRY * e;
    int i;
    for (e = sym[FIRST]; e != NULL; e = e->next) {
        switch (e->data_type) {
            case _INT:      printf("integer");  break;
            case _FLOAT:    printf("real");     break;
            }
        if (e->dim_type == _ARR) {
            printf(", dimension(");
            for (i=1; i<= e->size->rank; i++) {
                if (i>1) printf(",");
                printf("%d:%d",e->size->lb[i],e->size->ub[i]);
                }
            printf(")");
            }
        printf(" :: %s\n",e->id);
        }
    }

N_EXPR *copy_expr(N_EXPR *expr, N_ITER *innermost_iter, int lvl, bool upper, bool *changed) {
	if(expr == NULL) {
		return NULL;
	}
	N_EXPR *expr_cp = (N_EXPR *) malloc(sizeof(N_EXPR));
	expr_cp->next = NULL;
	expr_cp->typ = expr->typ;
	if(expr->typ == _FLOATNUM) {
		expr_cp->me.float_number = expr->me.float_number;
	} else if(expr->typ == _INTNUM) {
		expr_cp->me.int_number = expr->me.int_number;
	} else if(expr->typ == _OP) {
		expr_cp->me.op.oper = expr->me.op.oper;
		N_EXPR *op1expr_cp = copy_expr(expr->me.op.op1expr, innermost_iter, lvl, upper, changed);
		N_EXPR *op2expr_cp = copy_expr(expr->me.op.op2expr, innermost_iter, lvl, upper, changed);
		expr_cp->me.op.op1expr = op1expr_cp;
		expr_cp->me.op.op2expr = op2expr_cp;
	} else if (expr->typ == _VAR) {
		N_ITER *tmp = innermost_iter;
		bool done = false;
		while(tmp && tmp->lvl >= lvl) {
			if(expr->me.var_ref->entry->id == tmp->tn_for->loopvar->id) {
				expr_cp->typ = _OP;
				expr_cp->me.op.oper = NO_OP;
				if(upper) {
					expr_cp->me.op.op1expr = tmp->tn_for->ub;
				} else {
					expr_cp->me.op.op1expr = tmp->tn_for->lb;
				}
				done = true;
				*changed = true;
				break;
			}
			tmp = tmp->prev;
		}
		if(!done) {
			//assign old var_ref. no need to copy
			expr_cp->typ = _VAR;
			expr_cp->me.var_ref = expr->me.var_ref;
		}
	}
	return expr_cp;
}

void gen_expr(N_EXPR * ex, N_ITER *innermost_iter, int lvl);

void gen_exprlist(N_EXPRLIST * exl, N_ITER *innermost_iter, int lvl) {
    N_EXPR * ex;
    for (ex = exl->first; ex != NULL; ex = ex->next) {
        if (ex != exl->first) printf(",");
		bool changed = false;
		//gen_expr(ex, innermost_iter, lvl);
		N_EXPR *replaced = copy_expr(ex, innermost_iter, lvl, false, &changed);
		gen_expr(replaced, innermost_iter, lvl);
		if(changed) {
			printf(":");
			replaced = copy_expr(ex, innermost_iter, lvl, true, &changed);
			gen_expr(replaced, innermost_iter, lvl);
        }
    }
}


/*N_EXPRLIST *copy_exprlist(N_EXPRLIST *explist, N_ITER *innermost_iter, int lvl, bool upper) {
	N_EXPRLIST *explist_cp = (N_EXPRLIST *) malloc(sizeof(N_EXPRLIST));
	explist_cp->first = NULL;
	explist_cp->last = NULL;
	N_EXPR *tmp;
	N_EXPR *cp;
	for(tmp = explist->first; tmp!=explist->last; tmp = tmp->next) {
		cp = copy_expr(tmp, innermost_iter, lvl, upper);
		if(explist_cp->last)
			explist_cp->last->next=cp;
		if(tmp == explist->first) {
			explist_cp->first = cp;
		}
		if(tmp == explist->last) {
			explist_cp->last = cp;
		}
	}
	return explist_cp;
}*/

void gen_var_ref(N_VAR * v, N_ITER *innermost_iter, int lvl) {
	ENTRY * e;
	e = v->entry;
	printf("%s",e->id);
	if (e->dim_type == _SCAL) {
		if (v->index != NULL)
			printf("{no index expression allowed}\n");
		}
	if (e->dim_type == _ARR) {
		if (v->index == NULL)
			printf("{index expression missing}\n");
		printf("(");
		gen_exprlist(v->index, innermost_iter, lvl);
		printf(")");
		}
	}

void gen_oper(int o) {
  switch(o) {
  case EQ_OP:       printf("==");    break;
  case NE_OP:       printf("<>");   break;
  case GT_OP:       printf(">");    break;
  case GE_OP:       printf(">=");   break;
  case LT_OP:       printf("<");    break;
  case LE_OP:       printf("<=");   break;
  case PLUS_OP:     printf("+");    break;
  case MINUS_OP:    printf("-");    break;
  case MULT_OP:     printf("*");    break;
  case DIV_OP:      printf("/");    break;
  case AND_OP:      printf(".and."); break;
  case OR_OP:       printf(".or.");  break;
  case NOT_OP:      printf(".not."); break;
  case VECT_OP:     printf(":"); break;
  }
}

void gen_expr(N_EXPR * ex, N_ITER *innermost_iter, int lvl) {
    switch(ex->typ) {
        case _FLOATNUM:
            printf("%f",ex->me.float_number);
        break;
        case _INTNUM:
            printf("%d",ex->me.int_number);
        break;
        case _VAR: 
			gen_var_ref(ex->me.var_ref, innermost_iter, lvl);
		
        break;
        case _OP:
            if (ex->me.op.oper == NO_OP) {
                printf("(");
				gen_expr(ex->me.op.op1expr, innermost_iter, lvl);
                printf(")");
                }
            else if (ex->me.op.op2expr == NULL) {
                gen_oper(ex->me.op.oper);
				gen_expr(ex->me.op.op1expr, innermost_iter, lvl);
                }
            else {
				gen_expr(ex->me.op.op1expr, innermost_iter, lvl);
                gen_oper(ex->me.op.oper);
				gen_expr(ex->me.op.op2expr, innermost_iter, lvl);
                }
        }
    }

void gen_assign(N_ASSIGN * s, int nr, N_ITER *innermost_iter, int lvl) {
	//printf("%03d "gen_expr(replaced, innermost_iter, lvl);,nr);
    indent(idepth);
    if (s->var_ref != NULL) {
		gen_var_ref(s->var_ref, innermost_iter, lvl);
    	printf(" = ");
		}
	else {
    	printf("write(*,*) ");
		}
	gen_expr(s->expr, innermost_iter, lvl);
    printf("\n");
    }

void gen_stmts(N_STMTLIST * stmts);

void gen_if(N_IF * s, int nr) {
	//printf("%03d ",nr);
    indent(idepth);
    printf("if (");
	gen_expr(s->expr, NULL, 0);
    printf(") then\n");
    idepth++;
	gen_stmts(s->then_part);
    if (s->else_part != NULL) {
        indent(--idepth);
		printf("else\n");
        idepth++;
		gen_stmts(s->else_part);
        }
    indent(--idepth);
	printf("end if");
    printf("\n");
    }

void get_stmts(N_FOR *for_s, N_ITER* it, list *nrs) {
	N_STMTLIST *stmts = for_s->body;
	N_STMT *s;
	for (s = stmts->first; s != NULL; s = s->next) {
		switch(s->typ) {
			case _ASSIGN: {
				push_back_t(nrs, s->nr-1, it, s->me.s_assign);
				}
			break;
			case _IF:
				//TODO
				assert(0);
			break;
			case _FOR: {
				N_ITER *nest_it = malloc(sizeof(N_ITER));
				nest_it->tn_for = s->me.s_for;
				nest_it->prev = it;
				nest_it->lvl = ++lvl;
				get_stmts(s->me.s_for, nest_it, nrs);
				}
			break;
			}
		}
}

void vectorcode(list *nrs, int lvl) {
	vector_list * scc = get_SCC(nrs, lvl);
	vector_node *tmp;
	for(tmp = scc->tail; tmp != NULL; tmp = tmp->prev) {
		if(tmp->is_cyclic) {
			node *tmp_node = nrs->head;
			N_ITER * tmp_iter = tmp_node->loop;
			while(tmp_iter != NULL) {
				if(tmp_iter->lvl == lvl) {
					break;
				}
				tmp_iter = tmp_iter->prev;
			}
			N_FOR *s = tmp_iter->tn_for;

			indent(idepth);
			printf("do %s = ",s->loopvar->id);
			//gen_assign(assign, tmp->list->head->node_ct);
			gen_expr(s->lb, NULL, 0);
			printf(", ");
			gen_expr(s->ub, NULL, 0);
			if (s->step!=NULL) {
				printf(", ");
				gen_expr(s->step, NULL, 0);
				}
			printf("\n");
			idepth++;
			vectorcode(tmp->list, lvl+1);
			idepth--;
			indent(idepth);
			printf("end do\n");
		}
		else {
			N_ASSIGN *assign = tmp->list->head->assign;
			N_ITER *innermost_iter = tmp->list->head->loop;
			gen_assign(assign, tmp->list->head->node_ct, innermost_iter, lvl);
		}
	}
}

void gen_for(N_FOR * s, int nr) {
	lvl = 1;
	list *nrs = (list *) malloc(sizeof(list));
	nrs->head = NULL;
	nrs->tail = NULL;

	N_ITER *it = malloc(sizeof(N_ITER));
	it->tn_for = s;
	it->lvl = lvl;
	it->prev = NULL;

	get_stmts(s, it, nrs);
	update_nodes(nrs);
	vectorcode(nrs, 1);

	free(nrs);
	free(it);
}

void gen_stmts(N_STMTLIST * stmts) {
    N_STMT * s;
    for (s = stmts->first; s != NULL; s = s->next) {
        switch(s->typ) {
            case _ASSIGN:   
				gen_assign(s->me.s_assign,s->nr, NULL, 0);
            break;
            case _IF:       
				gen_if(s->me.s_if,s->nr);
            break;
            case _FOR:      
                gen_for(s->me.s_for,s->nr);
            break;
            }
        }
    }
