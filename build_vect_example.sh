./efl2f90 "vect_example.in" < vect_example.efl > vect_example.f90
gfortran -Wall -o vect_example vect_example.f90