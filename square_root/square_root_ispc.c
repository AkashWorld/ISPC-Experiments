export void compute_square_root_ispc(uniform const float float_numbers[], 
                                     uniform const size_t count,
                                     uniform const size_t first_index,
                                     uniform float output[]){
    uniform const float difference = .0001; // 10^-4
    foreach(i = first_index ... first_index + count){
        if(i < count){
            float a = float_numbers[i];
            float previous_approx = a*2;
            float current_difference = 1;
            while(current_difference > difference){
                output[i] = (.5)*(previous_approx + (a/previous_approx));
                current_difference = abs(output[i] - previous_approx);
                previous_approx = output[i];
            }
        }
    }
}


task void compute_square_root_ispc_task(uniform const float float_numbers[], 
                                     uniform const size_t count,
                                     uniform float output[]){
    uniform const float difference = .0001; // 10^-4
    uniform const size_t split_count = count/taskCount;
    uniform const size_t first_index = taskIndex*split_count;
    uniform const size_t end_index = min(first_index + split_count, count);
    foreach(i = first_index ... end_index){
            float a = float_numbers[i];
            float previous_approx = a*2;
            float current_difference = 1;
            while(current_difference > difference){
                output[i] = (.5)*(previous_approx + (a/previous_approx));
                current_difference = abs(output[i] - previous_approx);
                previous_approx = output[i];
            }
    }
}

export void compute_square_root_ispc_tasks(uniform const float float_numbers[], 
                                           uniform const size_t count,
                                           uniform float output[],
                                           uniform const size_t task_count){
    launch[task_count] compute_square_root_ispc_task(float_numbers, count, output);
    sync;
    return;
}

task void compute_arbitrary_computation(uniform const float input_numbers[],
                                          uniform float output_numbers[],
                                          uniform const size_t size)
{
    uniform int split_count = size/taskCount;
    uniform int first_index = taskIndex*split_count;
    uniform int end_index = min(first_index + split_count, size);
    foreach(i = first_index ... end_index){
        float a = input_numbers[i];
        a = a*3.5;
        a = a/2.93;
        a *= a;
        output_numbers[i] = a;
    }
}

export void test_compute_arbitrary_computation(uniform const float input_numbers[],
                                          uniform float output_numbers[],
                                          uniform size_t size,
                                          uniform const size_t task_count)
{
    launch[task_count] compute_arbitrary_computation(input_numbers, output_numbers, size);
    sync;
    return;
}