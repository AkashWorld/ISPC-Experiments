#Check if required executables are available in system path
EXECUTABLES = python3 ispc ${CC} ${CXX}
K := $(foreach exec,$(EXECUTABLES),\
        $(if $(shell which $(exec)),some string,$(error "No $(exec) in PATH)))

SQRT = ./square_root
CONC = ./concurrency
all: ${SQRT}/Makefile ${CONC}/Makefile
	cd ${SQRT} && make

clean:
	cd square_root && make clean