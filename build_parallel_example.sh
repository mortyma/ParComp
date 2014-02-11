./efl2f90 "parallel_example.in" < parallel_example.efl
#gfortran -fopenmp -o parallel_example parallel_example.f90
