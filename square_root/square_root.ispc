export void compute_square_root_ispc(uniform const float float_numbers[], 
                                     uniform const size_t first_index,
                                     uniform const size_t second_index,
                                     uniform float output[]){
    uniform const float difference = .0001; // 10^-4
    foreach(i = first_index ... second_index){
        const float a = float_numbers[i];
        float previous_approx = a*2;
        float final_approx = 0;
        float current_difference = 1;
        while(current_difference > difference){
            final_approx = (1/2)*(previous_approx + (a/previous_approx));
            current_difference = abs(final_approx - previous_approx);
            previous_approx = final_approx;
        }
        output[i] = final_approx;
    }
}
