CONC_DIR = ../concurrency

CFLAGS = -g -I${CURDIR} -mavx
LFLAGS = -pthread
OPTFLAG = -O3

all: tasks.o

debug: set_debug_opt_flag tasks.o
	
set_debug_opt_flag: 
	$(eval OPTFLAG = -O0)

tasks.o: tasks.c
	${CC} ${OPTFLAG} ${CFLAGS} ${LFLAGS} -o tasks.o -c tasks.c

tasksys.o: tasksys.cpp
	${CXX} ${OPTFLAG} ${CFLAGS} ${LFLAGS} -o tasksys.o -c tasksys.cpp -D ISPC_USE_PTHREADS_FULLY_SUBSCRIBED


clean: 
	rm *.run *.o