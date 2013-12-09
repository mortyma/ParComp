bake:
	mkdir -p build
	#cd build; cmake -DCMAKE_BUILD_TYPE="Debug" ../; make -j6
	bison -d efl.y
	flex efl.l
	clang -std=c90 -g  scc/scc.c scc/scc_structures.c scc/graphstreamer.c  efl.tab.c lex.yy.c efl2f90.c -lfl -o efl2f90
	
	
clean:
	rm *~
	cd build; make clean
	rm -r build
	rm efl2f90
	rm efl.tab.c
	rm efl.tab.h
	rm lex.yy.c
