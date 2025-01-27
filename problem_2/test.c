#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <square_root.h>
#include <errno.h>
/*
#define SIMPLE
#define BRUTE_FORCE
*/
#define AVX_TEST
#define ISPC_TEST
#define ALL
int main(int argc, char **argv){
    //Compute Square Root Normal
#if defined(SIMPLE) || defined(ALL)
    assert(abs(3.16227766 - _compute_square_root_normal(10)) < .0001);
    assert(abs(0.7071067811865476 - _compute_square_root_normal(.5)) < .0001);
    assert(abs(1.4142135623730951 - _compute_square_root_normal(2)) < .0001);
    assert(abs(1.8260767958933162 - _compute_square_root_normal(3.3345564645)) < .0001);
    assert(abs(2.7102463624179998 - _compute_square_root_normal(7.345435345)) < .0001);
    assert(abs(1.118033988749895 - _compute_square_root_normal(1.25)) < .0001);
    assert(abs(1.004987562112089 - _compute_square_root_normal(1.01)) < .0001);
    fprintf(stderr, "Simple normal square root calculations passed.\n");
#endif
#if defined(AVX_TEST) || defined(ISPC_TEST)
    float new_test[8] __attribute__ ((aligned(8 * sizeof(float))));
    float output[8] __attribute__ ((aligned(8 * sizeof(float)))); 
    new_test[0] = 1; new_test[1] = 2; new_test[2] = 3; new_test[3] = 4;
    new_test[4] = 5; new_test[5] = 6; new_test[6] = 7; new_test[7] = 8;
#endif
#if defined(ISPC_TEST)
    fprintf(stderr, "Currently computing ISPC: %f %f %f %f %f %f %f %f\n", *(new_test + 0), *(new_test + 1)
    , *(new_test + 2), *(new_test + 3), *(new_test + 4), *(new_test + 5), *(new_test + 6)
    , *(new_test + 7));
    compute_square_root_ispc(new_test, 8, 0, output);
    fprintf(stderr, "Final ISPC Results: %f %f %f %f %f %f %f %f\n", *(output + 0), *(output + 1)
    , *(output + 2), *(output + 3), *(output + 4), *(output + 5), *(output + 6)
    , *(output + 7));
#endif
#if defined(AVX_TEST)
    fprintf(stderr, "Currently computing AVX: %f %f %f %f %f %f %f %f\n", *(new_test + 0), *(new_test + 1)
    , *(new_test + 2), *(new_test + 3), *(new_test + 4), *(new_test + 5), *(new_test + 6)
    , *(new_test + 7));
    compute_square_root_avx(new_test, 8, output);
    fprintf(stderr, "Final AVX Results: %f %f %f %f %f %f %f %f\n", *(output + 0), *(output + 1)
    , *(output + 2), *(output + 3), *(output + 4), *(output + 5), *(output + 6)
    , *(output + 7));
#endif
#if defined(BRUTE_FORCE) || defined(ALL)
    float *twenty_mil_fp; //= (float*)aligned_alloc(sizeof(float), sizeof(float) * FLOAT_COUNT);
    if(posix_memalign((void**)&twenty_mil_fp, sizeof(float) * 8, FLOAT_COUNT*sizeof(float)) == 1
        || twenty_mil_fp == NULL){
        perror("Allocation of %ld bytes unsuccessful.");
    }
    float *twenty_mil_sqrtfp = (float*)malloc(sizeof(float) * FLOAT_COUNT);
    if(twenty_mil_fp == NULL || twenty_mil_sqrtfp == NULL ||
        !load_floating_numbers(twenty_mil_fp, FLOAT_COUNT, "20m_square_root.txt") ||
        !load_floating_numbers(twenty_mil_sqrtfp, FLOAT_COUNT, "res_20m_square_root.txt")
    ){
        fprintf(stderr, "Could not allocate memory and/or could not load files requested.\n");
        return -1;
    }

    for(size_t i = 0; i < FLOAT_COUNT; ++i){
        float computed_root = _compute_square_root_normal(*(twenty_mil_fp + i));
        assert(abs(computed_root - *(twenty_mil_sqrtfp + i)) < .0001f);
    }
    fprintf(stderr, "Brute forced normal calculation comparisons with %d values test passed.\n", FLOAT_COUNT);
    float *output_ispc;// = (float*)aligned_alloc(sizeof(float), sizeof(float) * FLOAT_COUNT);
    if(posix_memalign((void**)&output_ispc, sizeof(float) * 8, FLOAT_COUNT*sizeof(float)) == 1
        || output_ispc == NULL){
        perror("Allocation of %ld bytes unsuccessful.");
    }
    compute_square_root_ispc(twenty_mil_fp, FLOAT_COUNT, 0, output_ispc);
    for(size_t i = 0; i < FLOAT_COUNT; ++i){
        assert(abs(*(output_ispc + i) - *(twenty_mil_sqrtfp + i)) < .0001f);
    }
    free(output_ispc);
    fprintf(stderr, "Brute forced ISPC calculation comparisons with %d values test passed.\n", FLOAT_COUNT);
    float *output_avx;// = (float*)aligned_alloc(sizeof(float), sizeof(float) * FLOAT_COUNT);
    if(posix_memalign((void**)&output_avx, sizeof(float) * 8, FLOAT_COUNT*sizeof(float)) == 1
        || output_avx == NULL){
        perror("Allocation of %ld bytes unsuccessful.");
    }
    compute_square_root_avx(twenty_mil_fp, FLOAT_COUNT, output_avx);
    for(size_t i = 0; i < FLOAT_COUNT; ++i){
        assert(abs(*(output_avx + i) - *(twenty_mil_sqrtfp + i)) < .0001f);
    }
    fprintf(stderr, "Brute forced AVX calculation comparisons with %d values test passed.\n", FLOAT_COUNT);
    free(output_avx);
    free(twenty_mil_fp);
    free(twenty_mil_sqrtfp);
#endif
    return 1;
}