bake:
	bison -d efl.y
	flex efl.l
	clang -std=c90 -g  scc/scc.c scc/scc_structures.c scc/graphstreamer.c  efl.tab.c lex.yy.c efl2f90.c -lfl -o efl2f90
	
	
clean:
	rm -fr build
	rm -f efl2f90 
	rm -f efl.tab.c
	rm -f efl.tab.h
	rm -f lex.yy.c
	rm -f jacobi.f90
	rm -f test.f90
	rm -f *~
