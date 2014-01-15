./efl2f90 "jacobi.in" < jacobi.efl > jacobi.f90
gfortran -Wall -o jacobi jacobi.f90
