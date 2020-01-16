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


// TODO: Add failsafe to matrix operations
// Currently assuming all correct dimensions

/*!
 *
 * 
*/
void matrix_AND_data(matrix_s* m, const matrix_s* input_vector) {
    for(int i = 0; i < m->rows * m->cols; i++) {
        m->data[i] &= input_vector->data[i / m->cols];  // The floor-division i / m->cols ensures that the correct values are ANDed
    }
    
}

// Compare two matrices, column by column
// changes the  with 1's in the index of match column
// and 0 for all the columns with one or more mismatches
// returns a column vector, which is really just a matrix with 
// rows = 1
matrix_s* matrix_COMP_matrix(matrix_s* m1, matrix_s* m2) {
    matrix_s* m_out = malloc(1 * m2->cols * sizeof(int) + sizeof(matrix_s));
    m_out->rows = 1;
    m_out->cols = m1->cols;

    for(int i = 0; i < m1->cols; i++) {
        for(int j = 0; j < m1->cols; j++) {
            // Assume columns equal
            m_out->data[i] = 1;

            // If not, immediatly mark column and go to next column
            if(m1->data[j*m1->cols + i] != m2->data[j*m2->cols + i]) {
                m_out->data[i] = 0;
                break;
            }
        }
    }

    return m_out;
}

void print_matrix(const matrix_s* m) {
    for(int i = 0; i < m->rows; i++) {
        for(int j = 0; j < m->cols; j++) {
            printf("%d ", m->data[i*m->cols + j]);  // [i*m + j] is the equivalent of element [i][j], think "base + offset"
            printf(" ");
        }
        printf("\n");
    }
}

