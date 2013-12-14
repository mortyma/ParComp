./efl2f90 "test.in" < test.efl > test.f90
gfortran -o test test.f90
