CC = gcc
CFLAGS = -g -I${CURDIR} -std=c11 -mavx
#LFLAGS = -L${CURDIR}
#check if floating point numbers were generated


all: generate_fp_numbers compute_square_root

generate_fp_numbers:
ifeq (,$(wildcard ./20m_square_root.txt))
	python3 genrandom.py
endif

compute_square_root: main.c square_root_ispc.o
	gcc ${CFLAGS} ${LFLAGS} -o compute_square_root.run main.c square_root_ispc.o

square_root_ispc.o: square_root_ispc.c
	ispc -o square_root_ispc.o square_root_ispc.c

test: test.c square_root_ispc.o
	gcc ${CFLAGS} -o test.run test.c square_root_ispc.o

clean: 
	rm *.run *.o