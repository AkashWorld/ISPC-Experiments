#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <square_root.h>
#include <pthread.h>


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

//Arguments to be passed to pthreads for ISPC function call
struct _ISPC_args{
    size_t data_count;
    size_t first_index;
    const float* twenty_mil_fpn;
    float* output;
} typedef ISPC_args;

//Wrapper for ISPC function call to adjust for thread arguments
void* launch_ispc_task(void* args){
    ISPC_args *ispc_args = (ISPC_args*)args;
    compute_square_root_ispc(ispc_args->twenty_mil_fpn, ispc_args->data_count, 
                             ispc_args->first_index, ispc_args->output);
    return NULL;
}

//Launches {count} threads evenly distributed for ISPC function calls
//Also handles remainder calculation via a remainder thread
int profile_ispc_pthreads(const float *twenty_mil_fpn, float *output, const size_t count){
    clock_t start, end;
    if(twenty_mil_fpn == NULL || output == NULL){
        fprintf(stderr, "Could not allocate %ld bytes on line %d.\n", sizeof(float) * FLOAT_COUNT, __LINE__);
        return -1;
    }
    const size_t count_per_thread = FLOAT_COUNT/count;
    const size_t remainder_count = FLOAT_COUNT/count;
    pthread_t threads[count];
    ISPC_args args_arr[count];
    start = clock();
    for(size_t i = 0; i < count; ++i){
        args_arr[i].data_count = count_per_thread;
        args_arr[i].first_index = count_per_thread * i;
        args_arr[i].twenty_mil_fpn = twenty_mil_fpn;
        args_arr[i].output = output;
        if(pthread_create(&threads[i], NULL, launch_ispc_task, &(args_arr[i])) != 0){
            perror("Error creating ISPC threads.");
            return -1;
        }
    }
    //Remainder thread
    ISPC_args remainder_args;
    remainder_args.data_count = remainder_count;
    remainder_args.first_index = count_per_thread * count;
    remainder_args.twenty_mil_fpn = twenty_mil_fpn;
    remainder_args.output = output;
    pthread_t remainder_thread;
    if(pthread_create(&remainder_thread, NULL, launch_ispc_task, &(remainder_args)) != 0){
            perror("Error creating ISPC threads.");
            return -1;
    }
    for(size_t i = 0; i < count; ++i){
        pthread_join(threads[i], NULL);
    }
    pthread_join(remainder_thread, NULL);
    end = clock();
    printf("ISPC %ld POSIX THREAD(S) driven computation of square root of %d "
           "floating point numbers took %ld cycles.\n", count, FLOAT_COUNT, end - start);
    return 1;
}

int profile_ispc_tasks(const float *input_numbers, float* output, const size_t task_count){
    clock_t start, end;
    start = clock();
    compute_square_root_ispc_tasks(input_numbers, FLOAT_COUNT, output, task_count);
    end = clock();
    printf("ISPC %ld TASK(S) driven computation of square root of %d "
        "floating point numbers took %ld cycles.\n", task_count, FLOAT_COUNT, end - start);
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
    fprintf(stderr,"This may take some time...\n");
    assert(sizeof(float)*FLOAT_COUNT % sizeof(float) == 0);
    float *twenty_mil_fpn; 
    if(posix_memalign((void**)&twenty_mil_fpn, sizeof(float) * 8, FLOAT_COUNT*sizeof(float)) == 1
        || twenty_mil_fpn == NULL){
        fprintf(stderr, "Could not allocate %ld bytes on line %d.\n", sizeof(float) * FLOAT_COUNT, __LINE__);
        return -1;
    }
    if(!load_floating_numbers(twenty_mil_fpn, FLOAT_COUNT, "20m_square_root.txt")){
        fprintf(stderr, "Error loading floating point numbers.\n");
        return -1;
    }
    float *output; 
    if(posix_memalign((void**)&output, sizeof(float) * 8, FLOAT_COUNT*sizeof(float)) == 1
        || output == NULL){
        free(twenty_mil_fpn);
        fprintf(stderr, "Could not allocate %ld bytes on line %d.\n", sizeof(float) * FLOAT_COUNT, __LINE__);
        return -1;
    }
    profile_sequential(twenty_mil_fpn, output);
    profile_ispc_pthreads(twenty_mil_fpn, output, 1);
    profile_ispc_pthreads(twenty_mil_fpn, output, 2);
    profile_ispc_pthreads(twenty_mil_fpn, output, 3);
    profile_ispc_pthreads(twenty_mil_fpn, output, 4);
    profile_ispc_pthreads(twenty_mil_fpn, output, 5);
    profile_ispc_pthreads(twenty_mil_fpn, output, 6);
    profile_ispc_pthreads(twenty_mil_fpn, output, 7);
    profile_ispc_pthreads(twenty_mil_fpn, output, 8);
    profile_ispc_pthreads(twenty_mil_fpn, output, 9);
    profile_ispc_tasks(twenty_mil_fpn, output, 1);
    profile_ispc_tasks(twenty_mil_fpn, output, 2);
    profile_ispc_tasks(twenty_mil_fpn, output, 3);
    profile_ispc_tasks(twenty_mil_fpn, output, 4);
    profile_ispc_tasks(twenty_mil_fpn, output, 5);
    profile_ispc_tasks(twenty_mil_fpn, output, 6);
    profile_ispc_tasks(twenty_mil_fpn, output, 7);
    profile_ispc_tasks(twenty_mil_fpn, output, 8);
    profile_ispc_tasks(twenty_mil_fpn, output, 9);
    profile_avx(twenty_mil_fpn, output);
    free(output);
    free(twenty_mil_fpn);
    return 1;
}