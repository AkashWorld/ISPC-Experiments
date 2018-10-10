#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
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

int profile_sequential(const float *twenty_mil_fpn, float *output){
    clock_t start, end;
    output = malloc(sizeof(float)*FLOAT_COUNT);
    start = clock();
    compute_all_square_roots_normal(twenty_mil_fpn, FLOAT_COUNT, output);
    end = clock();
    printf("Non vectorized computation of square root of %d " 
           "floating point numbers took %ld cycles.\n", FLOAT_COUNT, end - start);
    free(output);
    return 1;
}

int profile_ispc(const float *twenty_mil_fpn, float *output){
    clock_t start, end;output = malloc(sizeof(float)*FLOAT_COUNT);
    if(output == NULL){
        fprintf(stderr, "Could not allocate %ld bytes on line %d.\n", sizeof(float) * FLOAT_COUNT, __LINE__);
        return -1;
    }
    start = clock();
    compute_square_root_ispc(twenty_mil_fpn, 0, FLOAT_COUNT - 1, output);
    end = clock();
    printf("ISPC driven computation of square root of %d "
           "floating point numbers took %ld cycles.\n", FLOAT_COUNT, end - start);
    free(output);
    return 1;
}

int profile_avx(const float *twenty_mil_fpn, float *output){
    clock_t start, end;
    output = malloc(sizeof(float)*FLOAT_COUNT);
    start = clock();
    compute_square_root_avx(twenty_mil_fpn, FLOAT_COUNT, output);
    end = clock();
    printf("AVX driven computation of square root of %d "
           "floating point numbers took %ld cycles.\n", FLOAT_COUNT, end - start);
    free(output);
    return 1;
}

int main(int argc, char **argv){
    float* twenty_mil_fpn = malloc(sizeof(float) * FLOAT_COUNT);
    if(twenty_mil_fpn == NULL){
        fprintf(stderr, "Could not allocate %ld bytes on line %d.\n", sizeof(float) * FLOAT_COUNT, __LINE__);
        return -1;
    }
    if(!load_floating_numbers(twenty_mil_fpn, FLOAT_COUNT, "20m_square_root.txt")){
        fprintf(stderr, "Error loading floating point numbers.\n");
        return -1;
    }
    float *output;
    profile_sequential(twenty_mil_fpn, output);
    //profile_ispc(twenty_mil_fpn, output);
    profile_avx(twenty_mil_fpn, output);
    free(twenty_mil_fpn);
    return 1;
}