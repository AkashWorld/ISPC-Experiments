#include <stdlib.h>
/*
 * Reference for computational logic:
 * https://www.math.upenn.edu/~kazdan/202F09/sqrt.pdf
 */
float compute_square_root_normal(const float a){
    if(a < 0)
        return 0;
    const float difference = .0001; // 10^-4
    float previous_approx = a*2;
    float final_approx = 0;
    float current_difference = 1;
    while(current_difference > difference){
        final_approx = (1/2)*(previous_approx + (a/previous_approx));
        current_difference = abs(final_approx - previous_approx);
        previous_approx = final_approx;
    }
    return final_approx;
}