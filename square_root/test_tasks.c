#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <time.h>
#include <square_root.h>
#include <errno.h>

#define CALC_COUNT 20000000

void execute_arbitrary_computation(const float* input_fp, float *output_fp, const size_t task_count)
{
    time_point<Clock> start, end;
    long elapsed_time = 0;    
    start = Clock::now();
    test_compute_arbitrary_computation(input_fp, output_fp, CALC_COUNT, task_count);
    end = Clock::now();
    elapsed_time = duration_cast<milliseconds>(end-start).count();
    printf("[Arbitrary] comptuation of %d task(s) took %ld ms.\n", task_count, elapsed_time);
    return;
}

int main(int argc, char **argv)
{
    srand((unsigned int)time(NULL));
    float *input_fp;
    if(posix_memalign((void**)&input_fp, sizeof(float) * 8, CALC_COUNT*sizeof(float)) == 1
        || input_fp == NULL){
        perror("Allocation of " "20000000" " bytes unsuccessful.");
    }
    for(size_t i = 0; i < CALC_COUNT; ++i){
        *(input_fp + i) = ((float)rand())/RAND_MAX;
    }
    float *output_fp;
    if(posix_memalign((void**)&output_fp, sizeof(float) * 8, CALC_COUNT*sizeof(float)) == 1
        || output_fp == NULL){
        perror("Allocation of " "20000000" " bytes unsuccessful.");
    }

    execute_arbitrary_computation(input_fp, output_fp, 1);
    execute_arbitrary_computation(input_fp, output_fp, 2);
    execute_arbitrary_computation(input_fp, output_fp, 3);
    execute_arbitrary_computation(input_fp, output_fp, 4);
    execute_arbitrary_computation(input_fp, output_fp, 5);
    execute_arbitrary_computation(input_fp, output_fp, 6);
    execute_arbitrary_computation(input_fp, output_fp, 7);
    execute_arbitrary_computation(input_fp, output_fp, 8);
    execute_arbitrary_computation(input_fp, output_fp, 9);
    execute_arbitrary_computation(input_fp, output_fp, 10);


    return 0;
}