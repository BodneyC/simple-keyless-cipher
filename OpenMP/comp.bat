@echo on
g++ byte_func.cc -c -std=c++11
g++ benc_funcs.cc -c -std=c++11 -fopenmp
g++ ..\WIN_TIME\timefile.cc -c
g++ TEST.cc -c -std=c++11 -fopenmp -I ..\WIN_TIME
g++ TEST.o timefile.o byte_func.o benc_funcs.o -o TEST.exe -fopenmp -std=c++11 -I ..\WIN_TIME
