GCC = gcc
GPP = g++

CONC_DIR = ../concurrency

CFLAGS = -g -I${CURDIR} -mavx
LFLAGS = -pthread
OPTFLAG = -O3

all: tasks.o

debug: set_debug_opt_flag tasks.o
	
set_debug_opt_flag: 
	$(eval OPTFLAG = -O0)

tasks.o: tasks.c
	${GCC} ${OPTFLAG} ${CFLAGS} ${LFLAGS} -o tasks.o -c tasks.c

tasksys.o: tasksys.cpp
	${GPP} ${OPTFLAG} ${CFLAGS} ${LFLAGS} -o tasksys.o -c tasksys.cpp


clean: 
	rm *.run *.o