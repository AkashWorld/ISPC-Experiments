Build prerequesites: gcc(c11), ispc, and python3.
Build instruction:
  make
To run the main progam, execute compute_square_root.run

Test instruction:
  make test
To run the test program, execute test.run

genrandom.py: Responsible for ceating a file of 20 million floating point values called 20m_square_root.txt.
Also creates the square root counterpart of each digit on another file called res_20m_square_root.txt for testing
purposes.

