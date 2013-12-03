# 185.A64 Compilers for Parallel Systems WS 2013/14 H.Moritsch
# build EFL

bison -d efl.y
flex efl.l
clang -std=c90 -g -o efl2f90 efl.tab.c lex.yy.c efl2f90.c -lfl

