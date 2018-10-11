#ifndef _HEADER_SQUARE_ROOT_
#define _HEADER_SQUARE_ROOT_
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
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
        final_approx = (.5)*(previous_approx + (a/previous_approx));
        current_difference = abs(final_approx - previous_approx);
        previous_approx = final_approx;
    }
    return final_approx;
}

void compute_all_square_roots_normal(const float *float_numbers, 
                                     const size_t float_count, float* output){
    for(size_t i = 0; i < float_count; ++i){
        *(output + i) = _compute_square_root_normal(*(float_numbers + i));
    }
}


void compute_square_root_avx(const float *numbers, const size_t count, float* output);



#ifdef __cplusplus
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
    extern void compute_square_root_ispc(const float * float_numbers, const uint32_t count, const uint32_t first_index, float * output);
    extern void compute_square_root_ispc_tasks(const float * float_numbers, const uint32_t count, float * output, const uint32_t task_count);
#if defined(__cplusplus) && (! defined(__ISPC_NO_EXTERN_C) || !__ISPC_NO_EXTERN_C )
} /* end extern C */
#endif // __cplusplus


#ifdef __cplusplus
#endif // __cplusplus

static int load_floating_numbers(float* numbers, const size_t number_count, const char* filename){
    FILE *file = fopen(filename, "r");
    if(file == NULL){
        fprintf(stderr, "File %s could not be opened!\n", filename);
        fclose(file);
        return 0;
    }
    char float_line[256] __attribute__ ((aligned(256*sizeof(char))));
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

