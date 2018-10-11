export void compute_square_root_ispc(uniform const float float_numbers[], 
                                     uniform const size_t count,
                                     uniform const size_t first_index,
                                     uniform float output[]){
    uniform const float difference = .0001; // 10^-4
    foreach(i = first_index ... count + first_index){
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
                                     uniform float output[],
                                     uniform const size_t _taskCount,
                                     uniform const size_t _taskIndex){
    uniform const float difference = .0001; // 10^-4
    uniform const size_t split_count = count/_taskCount;
    uniform const size_t first_index = _taskIndex*split_count;
    foreach(i = first_index ... first_index + split_count){
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

export void compute_square_root_ispc_tasks(uniform const float float_numbers[], 
                                           uniform const size_t count,
                                           uniform float output[],
                                           uniform const size_t task_count){
    for(uniform size_t i = 0; i < task_count; ++i){
        launch compute_square_root_ispc_task(float_numbers, count, output, task_count, i);
    }
    return;
}