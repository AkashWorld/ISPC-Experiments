#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
//#include <square_root_avx.h>
#include <square_root_normal.h>
//#include <square_root_ispc.h>

#define NORMAL

int main(int argc, char **argv){
    //Compute Square Root Normal
#if defined(NORMAL) || defined(ALL)
    assert(abs(3.16227766 - compute_square_root_normal(10)) < .0001);
    assert(abs(0.7071067811865476 - compute_square_root_normal(.5)) < .0001);
    assert(abs(1.4142135623730951 - compute_square_root_normal(2)) < .0001);
    assert(abs(1.8260767958933162 - compute_square_root_normal(3.3345564645)) < .0001);
    assert(abs(2.7102463624179998 - compute_square_root_normal(7.345435345)) < .0001);
    assert(abs(1.118033988749895 - compute_square_root_normal(1.25)) < .0001);
    assert(abs(0.9997167268781693 - compute_square_root_normal(0.9997167268781693)) < .0001);
    assert(abs(1.004987562112089 - compute_square_root_normal(1.01)) < .0001);
#endif
    return 1;
}