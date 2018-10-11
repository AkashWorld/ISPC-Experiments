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
