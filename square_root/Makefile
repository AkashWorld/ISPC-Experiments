EXECUTABLES = python3 ispc ${CC} ${CXX}
K := $(foreach exec,$(EXECUTABLES),\
        $(if $(shell which $(exec)),some string,$(error "No $(exec) in PATH)))

CONC_DIR = ../concurrency

CFLAGS = -I${CURDIR} -I${CURDIR}/..${CONC_DIR} -mavx -Wall
ISPCFLAGS = --pic --target=avx2-i32x8
LFLAGS = -pthread
OPTFLAG = -O3

all: compute_square_root

debug: set_debug_opt_flag compute_square_root
	
set_debug_opt_flag: 
	$(eval OPTFLAG = -O0)
	$(eval CFLAGS += -g)
	$(eval ISPCFLAGS += -g)

compute_square_root: main.c tasks square_root_ispc.o square_root_avx.o generate_fp_numbers
	${CXX} ${CFLAGS} ${OPTFLAG} -o compute_square_root.run main.c ${CONC_DIR}/tasksys.o square_root_ispc.o square_root_avx.o ${LFLAGS}

square_root_ispc.o: square_root_ispc.c
	ispc  ${OPTFLAG} ${ISPCFLAGS} -o square_root_ispc.o square_root_ispc.c 

square_root_avx.o: square_root_avx.c
	${CC} ${CFLAGS} ${OPTFLAG} -o square_root_avx.o -c square_root_avx.c

test: test.c square_root_ispc.o square_root_avx.o generate_fp_numbers
	${CC} ${OPTFLAG} ${CFLAGS} ${LFLAGS} -o test.run test.c square_root_ispc.o square_root_avx.o 

tasks: ${CONC_DIR}/Makefile
	cd ${CONC_DIR} && make tasksys.o

#check if floating point numbers were generated
generate_fp_numbers:
ifeq (,$(wildcard ./20m_square_root.txt))
	python3 genrandom.py
endif

clean: 
	rm *.run *.o
	cd ${CONC_DIR} && make clean
