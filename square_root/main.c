#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <square_root.h>

int profile_sequential(const float *twenty_mil_fpn, float *output){
    clock_t start, end;
    if(twenty_mil_fpn == NULL || output == NULL){
        fprintf(stderr, "Could not allocate %ld bytes on line %d.\n", sizeof(float) * FLOAT_COUNT, __LINE__);
        return -1;
    }
    start = clock();
    compute_all_square_roots_normal(twenty_mil_fpn, FLOAT_COUNT, output);
    end = clock();
    printf("Non vectorized computation of square root of %d " 
           "floating point numbers took %ld cycles.\n", FLOAT_COUNT, end - start);
    return 1;
}

int profile_ispc(const float *twenty_mil_fpn, float *output){
    clock_t start, end;
    if(twenty_mil_fpn == NULL || output == NULL){
        fprintf(stderr, "Could not allocate %ld bytes on line %d.\n", sizeof(float) * FLOAT_COUNT, __LINE__);
        return -1;
    }
    start = clock();
    compute_square_root_ispc_tasks(twenty_mil_fpn, FLOAT_COUNT, output, 1);
    end = clock();
    printf("ISPC driven computation of square root of %d "
           "floating point numbers took %ld cycles.\n", FLOAT_COUNT, end - start);
    return 1;
}

int profile_avx(const float *twenty_mil_fpn, float *output){
    clock_t start, end;
    if(twenty_mil_fpn == NULL || output == NULL){
        fprintf(stderr, "Could not allocate %ld bytes on line %d.\n", sizeof(float) * FLOAT_COUNT, __LINE__);
        return -1;
    }
    start = clock();
    compute_square_root_avx(twenty_mil_fpn, FLOAT_COUNT, output);
    end = clock();
    printf("AVX driven computation of square root of %d "
           "floating point numbers took %ld cycles.\n", FLOAT_COUNT, end - start);
    return 1;
}

int main(int argc, char **argv){
    float* twenty_mil_fpn = (float*)malloc(sizeof(float) * FLOAT_COUNT);
    if(twenty_mil_fpn == NULL){
        fprintf(stderr, "Could not allocate %ld bytes on line %d.\n", sizeof(float) * FLOAT_COUNT, __LINE__);
        return -1;
    }
    if(!load_floating_numbers(twenty_mil_fpn, FLOAT_COUNT, "20m_square_root.txt")){
        fprintf(stderr, "Error loading floating point numbers.\n");
        return -1;
    }
    float *output = (float*)malloc(sizeof(float) * FLOAT_COUNT);
    if(output == NULL){
        fprintf(stderr, "Error allocating memory for output.\n");
        free(twenty_mil_fpn);
        return -1;
    }
    profile_sequential(twenty_mil_fpn, output);
    profile_ispc(twenty_mil_fpn, output);
    profile_avx(twenty_mil_fpn, output);
    free(output);
    free(twenty_mil_fpn);
    return 1;
}