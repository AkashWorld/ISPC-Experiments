CONC_DIR = ../concurrency

CFLAGS = -g -I${CURDIR} -I${CURDIR}/..${CONC_DIR} -mavx -std=c11
LFLAGS = -pthread
OPTFLAG = -O3

#check if floating point numbers were generated

all: compute_square_root

debug: set_debug_opt_flag compute_square_root
	
set_debug_opt_flag: 
	$(eval OPTFLAG = -O0)

compute_square_root: main.c ${CONC_DIR}/tasksys.o square_root_ispc.o square_root_avx.o generate_fp_numbers
	${CXX} ${CFLAGS} ${LFLAGS} ${OPTFLAG} -o compute_square_root.run main.c ${CONC_DIR}/tasksys.o square_root_ispc.o square_root_avx.o

square_root_ispc.o: square_root_ispc.c
	ispc -g ${OPTFLAG} -o square_root_ispc.o square_root_ispc.c --pic --target=avx2-i32x8

square_root_avx.o: square_root_avx.c
	${CC} ${CFLAGS} ${OPTFLAG} -o square_root_avx.o -c square_root_avx.c

test: test.c square_root_ispc.o square_root_avx.o generate_fp_numbers
	${CC} ${OPTFLAG} ${CFLAGS} ${LFLAGS} -o test.run test.c square_root_ispc.o square_root_avx.o 

tasksys.o: ${CONC_DIR}/concurrency.mk
	make tasksys.o

generate_fp_numbers:
ifeq (,$(wildcard ./20m_square_root.txt))
	python3 genrandom.py
endif

clean: 
	rm *.run *.o
	rm ${CONC_DIR}/*.o