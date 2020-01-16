#ifndef COND_MASK_MATRIX
#define COND_MASK_MATRIX

#include <stdlib.h>
#include <stdio.h>

#define COND_MATRIX_N 8
#define COND_MATRIX_M 6

#define MASK_MATRIX_N 8
#define MASK_MATRIX_M 6

#define VEC_LEN(x) (sizeof(x) / sizeof(x)[0])

// 2D matrix containing the conditions for a given operation
// See "Logikkstyring"-slides, TTK4235 Jan. 16 2020, slide 36
static const int cond_matrix[COND_MATRIX_N][COND_MATRIX_M] = {
    {1, 1, 0, 0, 0, 1},
    {1, 1, 0, 0, 1, 0},
    {0, 1, 0, 0, 0, 0},
    {1, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0},
    {0, 0, 1, 0, 0, 0},
    {0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0}
};

// 2D matrix: the mask for cond_matrix 
// See "Logikkstyring"-slides, TTK4235 Jan. 16 2020, slide 36
static const int mask_matrix[MASK_MATRIX_N][MASK_MATRIX_M] = {
    {1, 1, 0, 1, 1, 1},
    {1, 1, 0, 1, 1, 1},
    {0, 1, 0, 0, 0, 0},
    {1, 1, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0},
    {1, 1, 1, 0, 0, 1},
    {0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0}
};

// Data vector given by example
// See "Logikkstyring"-slides, TTK4235 Jan. 16 2020, slide 36
static const int data_vec[MASK_MATRIX_N] = {1, 1, 1, 0, 0, 0, 0, 0};

void print_mask_matrix();
int** colwise_AND();

#endif // COND_MASK_MATRIX