#include <x86intrin.h>
#include <stdlib.h>
#include <stdio.h>
void compute_square_root_avx(const float *numbers, const size_t count, float* output)
{
    #if defined(__AVX__) || defined(__AVX2__)
    const __m256 _of_neg_zero = _mm256_set1_ps(-0.0);
    const __m256 _difference = _mm256_set1_ps(.0001f);
    const __m256 _half = _mm256_set1_ps(.5f);
    //compute variables
    __m256 _a_div_prev, _final_approx, _comparisons;
    int boolean;
    for(size_t i = 0; i < count; i += (256/32)){
        const __m256 _a = _mm256_load_ps(numbers + i);
        __m256 _previous_approx = _mm256_set1_ps(2.0f);
               _previous_approx = _mm256_mul_ps(_a, _previous_approx);
        __m256 _current_difference = _mm256_set1_ps(1.0f);
    compute:
        _a_div_prev = _mm256_div_ps(_a, _previous_approx);
        _final_approx = _mm256_add_ps(_previous_approx, _a_div_prev);
        _final_approx = _mm256_mul_ps(_final_approx, _half);
        _current_difference = _mm256_sub_ps(_previous_approx, _final_approx);
        _current_difference = _mm256_andnot_ps(_of_neg_zero, _current_difference); //abs
        _comparisons = _mm256_cmp_ps(_current_difference, _difference, _CMP_LT_OS);
        boolean = _mm256_movemask_ps(_comparisons);
        if(boolean != 0b11111111){
            _previous_approx = _final_approx;
            goto compute;
        } else{
            _mm256_store_ps(output + i, _final_approx);
            continue;
        }
    }
    #else
    fprintf(stderr, "AVX instructions are not available on this machine.\n");
    #endif
}