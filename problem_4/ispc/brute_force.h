// This is start of the header guard.  ADD_H can be any unique name.  By convention, we use the name of the header file.
#ifndef _BRUTE_FORCE_H
#define _BRUTE_FORCE_H

#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <cstring>
#include <iostream>
#include <algorithm>
#include <math.h>
#include <pthread.h>
#include <time.h>
#include <stdint.h>
#include <openssl/md5.h>
#include <sstream>
#include "objs/brute_force_ispc.h"

using namespace std; 

void gen_strs(const char*,string,const int, const int);

void gen_arr(char *);

void md5_to_str(unsigned char*);

void str_to_md5(const char*, unsigned char  *);

int load_hashes(char *, unsigned char *, unsigned char *,const size_t, const char*);

void compare_hashes_serial(const int8_t * ,const uint64_t * ,const uint64_t * , const uint64_t * ,uint8_t * ,size_t, int);

#endif
