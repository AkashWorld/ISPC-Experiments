#include <stdlib.h>
/*
 * https://www.math.upenn.edu/~kazdan/202F09/sqrt.pdf
 */
float compute_square_root_normal(const float a){
    float seed = a*2;
    float final_approx = 0;
    const float difference = .0001; // 10^-4
    if(a < 0)
        goto ret;
calculate:
    final_approx = (1/2)*(seed + (a/seed));
    if(abs(final_approx - seed) < difference)
        goto ret;
    seed = final_approx;
    goto calculate;
ret:
    return final_approx;
}