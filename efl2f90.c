/* 185.A64 Compilers for Parallel Systems WS 2013/14 H.Moritsch
   EFL to Fortran 90
*/


#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "scc/scc.h"
#include "efl_ir.h"



int idepth  = 0;
size_t lvl = 0;
vector_list **scc;

void indent(int d) {
    int i;
    for (i=0; i<4*d; i++) 
        printf(" ");
    }

void gen_decls(ENTRY ** sym) {
	get_scc("");

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

void gen_expr(N_EXPR * ex);

void gen_exprlist(N_EXPRLIST * exl) {
    N_EXPR * ex;
    for (ex = exl->first; ex != NULL; ex = ex->next) {
        if (ex != exl->first) printf(",");
        gen_expr(ex);
        }
    }

void gen_var_ref(N_VAR * v) {
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
        gen_exprlist(v->index);
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
  }
}

void gen_expr(N_EXPR * ex) {
    switch(ex->typ) {
        case _FLOATNUM:
            printf("%f",ex->me.float_number);
        break;
        case _INTNUM:
            printf("%d",ex->me.int_number);
        break;
        case _VAR:
            gen_var_ref(ex->me.var_ref);
        break;
        case _OP:
            if (ex->me.op.oper == NO_OP) {
                printf("(");
                gen_expr(ex->me.op.op1expr);
                printf(")");
                }
            else if (ex->me.op.op2expr == NULL) {
                gen_oper(ex->me.op.oper);
                gen_expr(ex->me.op.op1expr);
                }
            else {
                gen_expr(ex->me.op.op1expr);
                gen_oper(ex->me.op.oper);
                gen_expr(ex->me.op.op2expr);
                }
        }
    }

void gen_assign(N_ASSIGN * s, int nr) {
	printf("%03d ",nr);
    indent(idepth);
    if (s->var_ref != NULL) {
    	gen_var_ref(s->var_ref);
    	printf(" = ");
		}
	else {
    	printf("write(*,*) ");
		}
    gen_expr(s->expr);
    printf("\n");
    }

void gen_stmts(N_STMTLIST * stmts);

void gen_if(N_IF * s, int nr) {
	printf("%03d ",nr);
    indent(idepth);
    printf("if (");
    gen_expr(s->expr);
    printf(") then\n");
    idepth++;
    gen_stmts(s->then_part);
    if (s->else_part != NULL) {
        indent(--idepth);
        printf("    else\n");
        idepth++;
        gen_stmts(s->else_part);
        }
    indent(--idepth);
    printf("    end if");
    printf("\n");
    }

void v_start_gen_for(N_FOR* s, int nr);




void get_stmts(N_FOR *for_s) {
	N_STMTLIST *stmts = for_s->body;
	N_STMT *s;
	for (s = stmts->first; s != NULL; s = s->next) {
		switch(s->typ) {
			case _ASSIGN:
				//TODO
				//s->nr
			break;
			case _IF:
				//TODO
				assert(0);
			break;
			case _FOR:
				//TODO
				get_stmts(s->me.s_for);
			break;
			}
		}

}

void gen_for(N_FOR * s, int nr) {
	++lvl;
     /*TODO: if scc is acyclic
     vectorize_for(s, int nr)
     else generate loop
     */

	get_stmts(s);
		v_start_gen_for(s, nr);
		return;

     
	printf("%03d ",nr);
    indent(idepth);
    
    printf("do %s = ",s->loopvar->id);
    gen_expr(s->lb);
    printf(", ");
    gen_expr(s->ub);
    if (s->step!=NULL) {
        printf(", ");
        gen_expr(s->step);
        }
    printf("\n");
    idepth++;
    gen_stmts(s->body);
    indent(--idepth);
    printf("    end do\n");
	--lvl;
    }
    
    
/* Iteration variable
*/
typedef struct tN_ITER {
    ENTRY  * loopvar;         	/* loop variable */
    N_EXPR * lb;     		  	/* lower bound */
    N_EXPR * ub;     		  	/* upper bound */
    N_EXPR * step;     		  	/* step */
    struct tN_ITER * next;   /* list */
} N_ITER;
 
    /* List of statements
*/
typedef struct tN_ITER_LIST {
    N_ITER * first;
    N_ITER * last;
} N_ITER_LIST;

void v_gen_var_ref(N_VAR * v, N_ITER_LIST* iters);

void v_gen_expr(N_EXPR * ex, N_ITER_LIST* iters) {
    switch(ex->typ) {
        case _FLOATNUM:
            printf("%f",ex->me.float_number);
        break;
        case _INTNUM:
            printf("%d",ex->me.int_number);
        break;
        case _VAR:
            v_gen_var_ref(ex->me.var_ref, iters);
		break;
        case _OP:
        	assert(0);
            if (ex->me.op.oper == NO_OP) {
                printf("(");
                gen_expr(ex->me.op.op1expr);
                printf(")");
                }
            else if (ex->me.op.op2expr == NULL) {
                gen_oper(ex->me.op.oper);
                gen_expr(ex->me.op.op1expr);
                }
            else {
                gen_expr(ex->me.op.op1expr);
                gen_oper(ex->me.op.oper);
                gen_expr(ex->me.op.op2expr);
                }
        }
    }


void v_gen_exprlist(N_EXPRLIST * exl, N_ITER_LIST* iters) {
    N_EXPR * ex;
    for (ex = exl->first; ex != NULL; ex = ex->next) {
        if (ex != exl->first) printf(",");
        v_gen_expr(ex, iters);
        }
    }



void v_gen_var_ref(N_VAR * v, N_ITER_LIST* iters) {
    ENTRY * e;
    e = v->entry;
    
    //TODO: need to do this somewhere else, since var may not be part of a vectorized statement
    N_ITER* it;
    for (it = iters->first; it != NULL; it = it->next) {
    	if(it->loopvar->id == e->id) {
    		v_gen_expr(it->lb, iters);
    		printf(":");
    		v_gen_expr(it->ub, iters);
    		return;
    	}
    }
    
    printf("%s",e->id);
    
    if (e->dim_type == _SCAL) {
        if (v->index != NULL) 
            printf("{no index expression allowed}\n");
        }
    if (e->dim_type == _ARR) {
        if (v->index == NULL) 
            printf("{index expression missing}\n");
        printf("(");
        v_gen_exprlist(v->index, iters);
        printf(")");
        }
    }

void v_gen_assign(N_ASSIGN * s, int nr, N_ITER_LIST* iters) {
	printf("%03d ",nr);
    indent(idepth);
    if (s->var_ref != NULL) {
    	v_gen_var_ref(s->var_ref, iters);
    	printf(" = ");
		}
	else {
    	printf("write(*,*) ");
		}
    v_gen_expr(s->expr, iters);
    printf("\n");


}

void v_gen_stmts(N_STMTLIST * stmts, N_ITER_LIST* iters);

void v_start_gen_for(N_FOR* s, int nr) {
	++lvl;
	N_ITER it = {s->loopvar, s->lb, s->ub, s->step};
    N_ITER_LIST its = {&it, &it};
    v_gen_stmts(s->body, &its);
	--lvl;
}

void v_gen_for(N_FOR* s, int nr, N_ITER_LIST* iters) {
	++lvl;
	N_ITER it = {s->loopvar, s->lb, s->ub, s->step};
	iters->last->next = &it;
	iters->last = iters->last->next;
	v_gen_stmts(s->body, iters);
	--lvl;
}

void v_gen_stmts(N_STMTLIST * stmts, N_ITER_LIST* iters) {
	N_STMT * s;
	for (s = stmts->first; s != NULL; s = s->next) {
		switch(s->typ) {
			case _ASSIGN:
				v_gen_assign(s->me.s_assign,s->nr, iters);
			break;
			case _IF:
				//TODO
				assert(0);
				gen_if(s->me.s_if,s->nr);
			break;
			case _FOR:
				v_gen_for(s->me.s_for,s->nr, iters);
			break;
			}
		}
    }

void gen_stmts(N_STMTLIST * stmts) {
    N_STMT * s;
    for (s = stmts->first; s != NULL; s = s->next) {
        switch(s->typ) {
            case _ASSIGN:   
                gen_assign(s->me.s_assign,s->nr);
            break;
            case _IF:       
                gen_if(s->me.s_if,s->nr);
            break;
            case _FOR:      
                gen_for(s->me.s_for,s->nr);
                //vectorize_for(s->me.s_for,s->nr);
            break;
            }
        }
    }

