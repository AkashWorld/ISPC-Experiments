CONC_DIR = ../concurrency

CFLAGS = -g -I${CURDIR}
LFLAGS = -pthread
OPTFLAG = -O3

all: tasksys.o

debug: set_debug_opt_flag all
	
set_debug_opt_flag: 
	$(eval OPTFLAG = -O0)

tasks.o: tasks.c
	${CC} ${OPTFLAG} ${CFLAGS} -o tasks.o -c tasks.c ${LFLAGS}

tasksys.o: tasksys.cpp
	${CXX} ${OPTFLAG} ${CFLAGS} -o tasksys.o -c tasksys.cpp ${LFLAGS}

clean: 
	rm *.run *.o
