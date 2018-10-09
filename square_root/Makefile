CC = gcc
CFLAGS = -g -I${CURDIR} -std=c11

#check if floating point numbers were generated


all: generate_fp_numbers compute_square_root

generate_fp_numbers:
ifeq (,$(wildcard ./20m_square_root.txt))
	python3 genrandom.py
endif

compute_square_root: main.c
	gcc ${CFLAGS} -o compute_square_root.run main.c 

test: test.c
	gcc ${CFLAGS} -o test.run test.c

clean: 
	rm compute_square_root.run test.run