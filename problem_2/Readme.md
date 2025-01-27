
# Problem 2 - Parallel Square Root Computation

Program that accelerates an iterative version (newtons method) of computing a square root
algorithm. Benchmarking is done with sequential, handwritten AVX code and compiled ISPC
code.

## Getting Started

These instructions will get you a copy of the project up and running on your local machine for development and testing purposes. See deployment for notes on how to deploy the project on a live system.

### Prerequisites

Posix compliant standard c library.  
Ensure that the environmental PATH has access to the following:
g++, gcc, python3, and ispc

### Installing

Simply run make all (or make) to build the program and get the output binary:  
compute_square_root.run

To make a sample of the program with 0 optimizations, run make debug  

For the program to run properly, a text file named 20m_square_root.txt must be in the
same directory as the program. These are 20 million floating point new line separated
values to run square root computations on. It is generated by the makefile but also
can be generated by running genrandom.py with python3.

## Running the tests

make test  
./test.run  
make task_test  
./task_test.run  

### Purpose of tests

Checks the validity of the square root algorithm in the three different
contexts: sequential, AVX, and ISPC compiled versions.


## Built With

* [ISPC](https://github.com/ispc/ispc) - The SPMD compiler used

## Authors

* **Khalid Akash**
* **Brandon Smith** 
* **Suva Shahria** 
* **Ryan Morey** 


## License

This project is licensed under the MIT License

