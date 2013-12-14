./efl2f90 "jacobi.in" < jacobi.efl > jacobi.f90
gfortran -o jacobi jacobi.f90
