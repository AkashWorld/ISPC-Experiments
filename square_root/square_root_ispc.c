task void compute_square_root_ispc(uniform const float float_numbers[], 
                                     uniform const size_t count,
                                     uniform float output[]){
    uniform const float difference = .0001; // 10^-4
    uniform const size_t mod = count/taskCount;
    uniform const size_t split_count = count/taskCount;
    uniform const size_t first_index = taskIndex*split_count;
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
    launch[task_count] compute_square_root_ispc(float_numbers, count, output);
    return;
}
