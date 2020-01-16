#ifndef COND_MASK_MATRIX
#define COND_MASK_MATRIX

#include <stdlib.h>
#include <stdio.h>

// Globals
#define COND_MATRIX_N 8
#define COND_MATRIX_M 6

#define MASK_MATRIX_N 8
#define MASK_MATRIX_M 6

// 2D matrix containing the conditions for a given operation
// See "Logikkstyring"-slides, TTK4235 Jan. 16 2020, slide 36
static const int cond_matrix[COND_MATRIX_N][COND_MATRIX_M] = {
    1, 1, 0, 0, 0, 1,
    1, 1, 0, 0, 1, 0,
    0, 1, 0, 0, 0, 0,
    1, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0,
    0, 0, 1, 0, 0, 0,
    0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0
};

// 2D matrix: the mask for cond_matrix 
// See "Logikkstyring"-slides, TTK4235 Jan. 16 2020, slide 36
static const int mask_matrix[MASK_MATRIX_N * MASK_MATRIX_M] = {
    1, 1, 0, 1, 1, 1,
    1, 1, 0, 1, 1, 1,
    0, 1, 0, 0, 0, 0,
    1, 1, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0,
    1, 1, 1, 0, 0, 1,
    0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0
};

/*!
 * @struct      matrix_s
 * @abstract    Contains all chicken's properties.
 * @discussion  All properties of chickens (weight, color, ...)
 *              are stored in this structure.
 * @field       color     Hexadecimal color value.
 * @field       weight    Weight of the chicken in kg.
 * @field       age       How old the chicken is.
*/
typedef struct {
    int rows;
    int cols;
    int data[];
} matrix_s;

matrix_s* create_matrix(int rows, int cols, const int values[rows*cols]);
void matrix_AND_data(matrix_s* m, const int input_data[m->cols]);
void print_matrix(matrix_s* m);

#endif // COND_MASK_MATRIX