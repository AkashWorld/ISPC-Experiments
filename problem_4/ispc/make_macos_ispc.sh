ispc -O2 --target=avx2 brute_force_ispc.cpp -o objs/brute_force_ispc.o -h objs/brute_force_ispc.h 
g++ brute_force.cpp -Iobjs/ -O3 -Wall -c -o objs/brute_force.o -no-pie
g++ -Iobjs/ -O3 -Wall -o brute_forcer objs/brute_force.o objs/brute_force_ispc.o -no-pie