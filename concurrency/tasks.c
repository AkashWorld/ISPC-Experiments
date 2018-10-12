#include <pthread.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <assert.h>

struct _task_args{
    void* data;
    //Unused data that should be passed to thread
    //TODO: Figure out how to pass them on (refer to ISPC documentation and tasksys.cpp)
    /*
    int threadIndex; 
    int threadCount;
    int taskIndex; 
    int taskCount;
    int taskIndex0; 
    int taskIndex1; 
    int taskIndex2; 
    int taskCount0; 
    int taskCount1; 
    int taskCount2;
    */
} typedef task_args;

struct _task_id{
    pthread_t *thread_list;
    int thread_count;
    struct _task_args *arg_struct;
} typedef task_id;

void* allocate_task_structure(void **handlePtr){
    *handlePtr = aligned_alloc(sizeof(task_id), sizeof(task_id));
    if(*handlePtr == NULL){
        fprintf(stderr,"Error allocating task argument structure.\n");
        return NULL;
    }
    task_id* new_task = (task_id*)*handlePtr;
    new_task->thread_list = NULL;
    new_task->thread_count = 0;
    new_task->arg_struct = aligned_alloc(sizeof(task_args), sizeof(task_args));
    if(new_task->arg_struct == NULL){
        fprintf(stderr, "Error memory allocating arguement struct for tasks. \n");
        return NULL;
    }
    handlePtr = (void**)&new_task;
    return (void*)new_task;
}

void *ISPCAlloc(void **handlePtr, int64_t size, int32_t alignment){
    assert(size%alignment == 0);
    task_id* task;
    if(size % alignment != 0)
        return NULL;
    task = (task_id*)allocate_task_structure(handlePtr);
    if(posix_memalign(&(task->arg_struct->data), alignment, size) != 0){
        fprintf(stderr,"Error allocating aligned memory for ISPC tasks. %s, %d", __FILE__, __LINE__);
    }
    return task->arg_struct->data;
}

void ISPCLaunch(void **handlePtr, void *f, void *data, int count0, int count1, int count2){
    if(handlePtr == NULL)
        allocate_task_structure(handlePtr);
    task_id *task = (task_id*)*handlePtr;
    task->thread_count = count0 * count1 * count2;
    task->thread_list = aligned_alloc(task->thread_count * sizeof(pthread_t), sizeof(pthread_t));
    if(task->thread_list == NULL){
        fprintf(stderr,"Error allocating aligned memory for ISPC tasks. %s, %d", __FILE__, __LINE__);
        return;
    }
    for(int i = 0; i < task->thread_count; ++i){
        pthread_create(task->thread_list + i, NULL, f, (void*)task->arg_struct->data);
    }
    return;
}

void ISPCSync(void *handle){
    task_id* task = (task_id*)handle;
    for(int i = 0; i < task->thread_count; ++i){
        pthread_join(*(task->thread_list + i), NULL);
    }
    free(task->arg_struct->data);
    free(task->arg_struct);
    free(task->thread_list);
    free(handle);
}