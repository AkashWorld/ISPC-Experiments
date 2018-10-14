clang++ tasksys.cpp -O2 -m64 -c -o objs/tasksys.o
ispc -O2 -g --target=avx2 brute_force_ispc.cpp -o objs/brute_force_ispc.o -h objs/brute_force_ispc.h 
g++ brute_force.cpp -Iobjs/ -O3 -g -Wall -c -o objs/brute_force.o -no-pie
g++ -Iobjs/ -O3 -g -Wall -lpthread -o brute_forcer tasksys.o objs/brute_force.o objs/brute_force_ispc.o -no-pie