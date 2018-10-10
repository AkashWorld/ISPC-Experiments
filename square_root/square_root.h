#ifndef _HEADER_SQUARE_ROOT_
#define _HEADER_SQUARE_ROOT_
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <x86intrin.h>
#include <assert.h>

#define FLOAT_COUNT 20000000

/*
 * Reference for computational logic:
 * https://www.math.upenn.edu/~kazdan/202F09/sqrt.pdf
 */
__always_inline float _compute_square_root_normal(const float a){
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

void compute_all_square_roots_normal(const float *float_numbers, const size_t float_count,
                                     float* output){
    for(size_t i = 0; i < float_count; ++i){
        *(output + i) = _compute_square_root_normal(*(float_numbers + i));
    }
}

//TODO: Add boundry checks, lucky that 20,000,000 % (256/32) = 0
void compute_square_root_avx(const float *numbers, const size_t count, float* output)
{
    const __m256 _of_neg_zero = _mm256_set1_ps(-0.0);
    const __m256 _difference = _mm256_set1_ps(.0001f);
    const __m256 _half = _mm256_set1_ps(.5f);
    //compute variables
    __m256 _a_div_prev, _final_approx, _comparisons;
    int boolean;
    for(size_t i = 0; i < count; i += (256/32)){
        const __m256 _a = _mm256_loadu_ps(numbers + i);
        __m256 _previous_approx = _mm256_set1_ps(2.0f);
               _previous_approx = _mm256_mul_ps(_a, _previous_approx);
        __m256 _current_difference = _mm256_set1_ps(1.0f);
    compute:
        _a_div_prev = _mm256_div_ps(_a, _previous_approx);
        _final_approx = _mm256_add_ps(_previous_approx, _a_div_prev);
        _final_approx = _mm256_mul_ps(_final_approx, _half);
        _current_difference = _mm256_sub_ps(_previous_approx, _final_approx);
        _current_difference = _mm256_andnot_ps(_of_neg_zero, _current_difference); //abs
        _comparisons = _mm256_cmp_ps(_current_difference, _difference, _CMP_LT_OQ);
        boolean = _mm256_movemask_ps(_comparisons);
        if(boolean == 0){
            _previous_approx = _final_approx;
            goto compute;
        } else{
            _mm256_storeu_ps(output + i, _final_approx);
            continue;
        }
    }
}

#ifdef __cplusplus
namespace ispc { /* namespace */
#endif // __cplusplus

#ifndef __ISPC_ALIGN__
#if defined(__clang__) || !defined(_MSC_VER)
// Clang, GCC, ICC
#define __ISPC_ALIGN__(s) __attribute__((aligned(s)))
#define __ISPC_ALIGNED_STRUCT__(s) struct __ISPC_ALIGN__(s)
#else
// Visual Studio
#define __ISPC_ALIGN__(s) __declspec(align(s))
#define __ISPC_ALIGNED_STRUCT__(s) __ISPC_ALIGN__(s) struct
#endif
#endif


///////////////////////////////////////////////////////////////////////////
// Functions exported from ispc code
///////////////////////////////////////////////////////////////////////////
#if defined(__cplusplus) && (! defined(__ISPC_NO_EXTERN_C) || !__ISPC_NO_EXTERN_C )
extern "C" {
#endif // __cplusplus
    extern void compute_square_root_ispc(const float * float_numbers, const uint32_t first_index, const uint32_t second_index, float * output);
#if defined(__cplusplus) && (! defined(__ISPC_NO_EXTERN_C) || !__ISPC_NO_EXTERN_C )
} /* end extern C */
#endif // __cplusplus


#ifdef __cplusplus
} /* namespace */
#endif // __cplusplus

static int load_floating_numbers(float* numbers, const size_t number_count, const char* filename){
    FILE *file = fopen(filename, "r");
    if(file == NULL){
        fprintf(stderr, "File %s could not be opened!\n", filename);
        fclose(file);
        return 0;
    }
    char float_line[256] = {};
    for(size_t i = 0; i < number_count; ++i){
        memset(float_line, 0, 256);
        if(fgets(float_line, 256, file) == NULL && i + 1 != number_count){
            fprintf(stderr, "Could not read the expected number of floating point numbers."
                    "Expected: %ld, Read: %ld\n", number_count, i);
            fclose(file);
            return 0;
        }
        *(numbers + i) = atof(float_line);
    }
    fclose(file);
    return 1;
}

#endif

