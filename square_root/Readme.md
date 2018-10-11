Build prerequesites: g++, gcc, ispc, and python3. Ensure that all three of these are in the PATH environment variable<br />
Build instruction:<br />
  make<br />
To run the main progam, execute compute_square_root.run<br />

Test instruction:<br />
  make test<br />
To run the test program, execute test.run<br />

genrandom.py: Responsible for ceating a file of 20 million floating point values called 20m_square_root.txt.<br />
Also creates the square root counterpart of each digit on another file called res_20m_square_root.txt for testing<br />
purposes.<br />

