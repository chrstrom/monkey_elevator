#ifndef COND_MASK_MATRIX
#define COND_MASK_MATRIX

#include <stdlib.h>
#include <stdio.h>

// Globals
#define COND_MATRIX_N 8
#define COND_MATRIX_M 6

#define MASK_MATRIX_N 8
#define MASK_MATRIX_M 6

// The 2D matrix containing the conditions (rows) for a given operation (cols)
// See "Logikkstyring"-slides, TTK4235 Jan. 16 2020, slide 36
static const int cond_matrix[COND_MATRIX_N * COND_MATRIX_M] = {
    1, 1, 0, 0, 0, 1,
    1, 1, 0, 0, 1, 0,
    0, 1, 0, 0, 0, 0,
    1, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0,
    0, 0, 1, 0, 0, 0,
    0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0
};

// The 2D mask matrix used for specify values of importance in cond_matrix
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
 * @abstract    Contains the main properties of a matrix
 * @discussion  data will contain rows*cols elements, and makes up the values
 * in the matrix. Note that the data is stored in a 1D array, so any subsequent
 * operations on one single element in a matrix where rows && cols > 1 (2D matrix)
 * must be done with caution to ensure the correct indexation.
 * Example: mat[i][j] ==> [i*mat->cols + j]
 *  Note also that by specfying rows = 1, you are practically making a 1-d vector.
 * @field       rows    The amount of rows in the matrix
 * @field       cols    The amount of columns in the matrix
 * @field       data[]  The data contained by the matrix. Is rows*cols long.
*/
typedef struct {
    int rows;
    int cols;
    int data[];
} matrix_s;

/*!
 *@function create_matrix
 *@discussion the function creates a matrix, with rows and columns
 *as specified by the @param values array
 *@param rows amt. of rows, MUST correspond to amt. of rows in the values-array
 *@param cols amt. of columns, MUST correspond to amt. of columns in values-array 
 * 
*/
matrix_s* create_matrix(int rows, int cols, const int values[rows*cols]);

void matrix_AND_data(matrix_s* m, const matrix_s* input_vector);
matrix_s* matrix_COMP_matrix(matrix_s* m1, matrix_s* m2);

void print_matrix(const matrix_s* m);

#endif // COND_MASK_MATRIX