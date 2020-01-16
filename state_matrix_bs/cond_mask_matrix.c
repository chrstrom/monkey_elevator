#include "headers/cond_mask_matrix.h"

// Below is a proposed method of commenting on functions
// If wanted, this can be done for every single function

/* !
 * @function    create_matrix
 * @abstract    creates a matrix of a given size
 * @param   rows    amount of rows
 * @param   cols    amount of columns
 * @param   values  an array with the values that the matrix is to be filled with
 * @result      a pointer to a matrix struct with all 0's
*/
matrix_s* create_matrix(int rows, int cols, const int values[rows*cols]) {
    // Allocate memory for a matrix of size rows x cols
    matrix_s* m = malloc(rows * cols * sizeof(int) + sizeof(matrix_s));

     // Set member variables of the matrix
    m->rows = rows;
    m->cols = cols;

    for(int i = 0; i < rows*cols; i++) {
        m->data[i] = values[i];
    }
    
    return m;
}

void matrix_AND_data(matrix_s* m, const int input_data[m->cols]) {
    for(int i = 0; i < m->rows * m->cols; i++) {
        m->data[i] = m->data[i] & input_data [i / m->cols];  // The floor-division i / m->cols ensures that the correct values are ANDed
    }
}

void print_matrix(matrix_s* m) {
    for(int i = 0; i < m->rows; i++) {
        for(int j = 0; j < m->cols; j++) {
            printf("%d ", m->data[i*m->cols + j]);  // [i*m + j] is the equivalent of element [i][j], think "base + offset"
            printf(" ");
        }
        printf("\n");
    }
}

