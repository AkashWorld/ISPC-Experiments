#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <square_root.h>

#define FLOAT_COUNT 20000000

int load_floating_numbers(float* numbers, const size_t number_count, const char* filename){
    FILE *file = fopen(filename, "r");
    if(file == NULL){
        fprintf(stderr, "File %s could not be opened!\n", filename);
        fclose(file);
        return 0;
    }
    char float_line[256] = {};
    for(size_t i = 0; i < number_count; ++i){
        memset(float_line, 0, 256);
        if(fgets(float_line, 256, file) == NULL && i + 1 != number_count){
            fprintf(stderr, "Could not read the expected number of floating point numbers."
                    "Expected: %ld, Read: %ld\n", number_count, i);
            fclose(file);
            return 0;
        }
        *(numbers + i) = atof(float_line);
    }
    fclose(file);
    return 1;
}

/*
#define SIMPLE
#define BRUTE_FORCE
*/
#define AVX_TEST

int main(int argc, char **argv){
    //Compute Square Root Normal
#if defined(SIMPLE) || defined(ALL)
    assert(abs(3.16227766 - _compute_square_root_normal(10)) < .0001);
    assert(abs(0.7071067811865476 - _compute_square_root_normal(.5)) < .0001);
    assert(abs(1.4142135623730951 - _compute_square_root_normal(2)) < .0001);
    assert(abs(1.8260767958933162 - _compute_square_root_normal(3.3345564645)) < .0001);
    assert(abs(2.7102463624179998 - _compute_square_root_normal(7.345435345)) < .0001);
    assert(abs(1.118033988749895 - _compute_square_root_normal(1.25)) < .0001);
    assert(abs(0.9997167268781693 - _compute_square_root_normal(0.9997167268781693)) < .0001);
    assert(abs(1.004987562112089 - _compute_square_root_normal(1.01)) < .0001);
    fprintf(stderr, "Simple normal square root calculations passed.\n");
#endif
#if defined(AVX_TEST)
    float new_test[8] = {2, 3.234353454, 2.5, 7.33, 1, 0.5, 3.1, 2.2};
    float output[8] = {};
    fprintf(stderr, "Currently computing %f %f %f %f %f %f %f %f\n", *(new_test + 0), *(new_test + 1)
    , *(new_test + 2), *(new_test + 3), *(new_test + 4), *(new_test + 5), *(new_test + 6)
    , *(new_test + 7));
    compute_square_root_avx(new_test, 8, output);
    fprintf(stderr, "Final computing %f %f %f %f %f %f %f %f\n", *(output + 0), *(output + 1)
    , *(output + 2), *(output + 3), *(output + 4), *(output + 5), *(output + 6)
    , *(output + 7));
#endif
#if defined(BRUTE_FORCE) || defined(ALL)
    float *twenty_mil_fp = malloc(sizeof(float) * FLOAT_COUNT);
    float *twenty_mil_sqrtfp = malloc(sizeof(float) * FLOAT_COUNT);
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

    float *output = malloc(sizeof(float)*FLOAT_COUNT);
    compute_square_root_avx(twenty_mil_fp, FLOAT_COUNT, output);
    for(size_t i = 0; i < FLOAT_COUNT; ++i){
        assert(abs(*(output + i) - *(twenty_mil_sqrtfp + i)) < .0001f);
    }
    fprintf(stderr, "Brute forced AVX calculation comparisons with %d values test passed.\n", FLOAT_COUNT);
    free(output);
    free(twenty_mil_fp);
    free(twenty_mil_sqrtfp);
#endif
    return 1;
}