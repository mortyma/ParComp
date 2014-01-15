ParComp
=======

A simple parser, parsing efl (simple toy language) code to fortran 90 code.  If possible,  statemments are vectorized.

- To build the efl to fortran 90 parser, use 
  ./build_efl.sh

- To parse efl code to fortran 90 code, you need to provide the following:
  1) foo.efl
     The efl code to parse
  2) foo.in
     Specification of dependencies, matching with the efl code.
     Format:
        All values separated by space: 0x20
        First line contains: #nodes #edges #levels
        Each following line contains one dependency in the format:  
           #from #to #level #type 
           where #from and #to are node numbers, #
           level is the level of the dependency and 
           #type  is one out of {1,2,3}, where 1=true, 2=anti and 3=output dependency
      Example:
      3 3 2        // 3 nodes, 3 edges, 2 levels
      1 2 1 1      // true dependency from node 1 to 2 at level 1
      2 3 2 2      // anti dependency from node 2 to 3 at level 2
      3 3 2 1      // true dependency from node 3 to 3 at level 2
   3) To parse the efl to fortran code, run:
      ./efl2f90 foo.in < foo.efl > foo.f90
   WARNING: fortran 90 allows 132 characters per line only. If the parser generated longer lines, please 
   split them up using the continuation character "&" and newline.
   4) To compile the generated fortran code, run:
      gfortran -Wall -o foo foo.f90
  
