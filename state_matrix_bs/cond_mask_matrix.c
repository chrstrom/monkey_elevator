#include "headers/cond_mask_matrix.h"


/* !
 * @function    create_empty_matrix
 * @abstract    creates a 0'd matrix of a given size
 * @param   rows    amount of rows
 * @param   cols    amount of columns
 * @result      a pointer to a matrix struct with data as all 0's
*/
matrix_s* create_empty_matrix(int rows, int cols) {
     // Allocate memory for a matrix of size rows x cols
    matrix_s* m = malloc(rows * cols * sizeof(int) + sizeof(matrix_s));

     // Set member variables of the matrix
    m->rows = rows;
    m->cols = cols;
    for(int i = 0; i < rows*cols; i++) {
        m->data[i] = 0;
    }
    return m;
}

/* !
 * @function    create_matrix
 * @abstract    creates a matrix of a given size
 * @param   rows    amount of rows
 * @param   cols    amount of columns
 * @param   values  an array with the values that the matrix is to be filled with
 * @result      a pointer to a matrix struct with data equal to values' data
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

/* !
 * @function    matrix_AND_data
 * @abstract    Element-wise AND of row-vector and matrix
 * @discussion  All the inputs will be containers of 1's and 0's, and the
 * resulting matrix will be one with identical dimensions as @param m, but with
 * the operation completed
 * @param   m   Matrix to be used for the operation
 * @param   input_vector    Column-vector to be used for the operation
 * @result  a new matrix that is the result of the element-wise AND-operation
*/
matrix_s* matrix_AND_data(const matrix_s* m, const matrix_s* input_vector) {
    // Create a new matrix that will be the output of this operation
    matrix_s* m_out = create_empty_matrix(m->rows, m->cols);

    for(int i = 0; i < m->rows * m->cols; i++) {
        m_out->data[i] = m->data[i] & input_vector->data[i / m->cols];  // The floor-division i / m->cols ensures that the correct values are ANDed
    }

    return m_out;
}

/* !
 * @function    matrix_COMP_matrix
 * @abstract    Column-wise comparison of two matrices
 * @discussion  Both the matrices m1 and m2 must be of the same dimensions for 
 * the operation to make sense, as we are comparing column by column in them
 * and checking if they are equal or not.
 * @param   m1  Matrix to be used for the operation
 * @param   m2  Matrix to be used for the operation
 * @result  a 1D vector (type matrix_s) with 1's in all indices where m1
 * and m2 have equal columns
*/
matrix_s* matrix_COMP_matrix(const matrix_s* m1, const matrix_s* m2) {
    // Create output matrix and set values for rows and cols
    matrix_s* m_out = malloc(1 * m2->cols * sizeof(int) + sizeof(matrix_s));
    m_out->rows = 1;
    m_out->cols = m1->cols;

    // Determine what m_out->data[] will contain
    // This depends on the columns of m1 and m2
    for(int i = 0; i < m1->cols; i++) {
        for(int j = 0; j < m1->cols; j++) {
            // Assume current columns in m1 and m2 equal
            m_out->data[i] = 1;
            // If not, immediatly mark column (0 in m_out) and go to next column
            if(m1->data[j*m1->cols + i] != m2->data[j*m2->cols + i]) {
                m_out->data[i] = 0;
                break;
            }
        }
    }

    return m_out;
}

/* !
 * @function    print_matrix
 * @abstract    Print a matrix_s with pretty formatting
 * @param   m   Matrix to be printed
*/
void print_matrix(const matrix_s* m) {
    for(int i = 0; i < m->rows; i++) {
        for(int j = 0; j < m->cols; j++) {
            printf("%d ", m->data[i*m->cols + j]);  // [i*m + j] is the equivalent of element [i][j], think "base + offset"
            printf(" ");
        }
        printf("\n");
    }
}

