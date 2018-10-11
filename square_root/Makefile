GCC = gcc
GPP = g++
CFLAGS = -g -I${CURDIR} -mavx
LFLAGS = -pthread

#check if floating point numbers were generated

all: compute_square_root

compute_square_root: main.c square_root_ispc.o square_root_avx.o generate_fp_numbers
	${GCC} ${CFLAGS} ${LFLAGS} -o compute_square_root.run main.c square_root_ispc.o square_root_avx.o

square_root_ispc.o: square_root_ispc.c
	ispc -o square_root_ispc.o square_root_ispc.c --pic

square_root_avx.o: square_root_avx.c
	${GCC} ${CFLAGS} -o square_root_avx.o -c square_root_avx.c

test: test.c square_root_ispc.o square_root_avx.o generate_fp_numbers
	${GCC} ${CFLAGS} ${LFLAGS} -o test.run test.c square_root_ispc.o square_root_avx.o 

generate_fp_numbers:
ifeq (,$(wildcard ./20m_square_root.txt))
	python3 genrandom.py
endif

clean: 
	rm *.run *.o