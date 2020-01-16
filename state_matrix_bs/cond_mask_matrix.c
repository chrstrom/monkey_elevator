#include "headers/cond_mask_matrix.h"



void print_mask_matrix() {
    for(int n = 0; n < MASK_MATRIX_N; n++) {
        for(int m = 0; m < MASK_MATRIX_M; m++) {
            printf("%d ", mask_matrix[n][m]);
            printf("\t");
        }
        printf("\n");
    }
}

int** colwise_AND() {
    // We first allocate memory space for the double pointer
    // that will be the return value
    int** result_matrix = (int**) calloc(MASK_MATRIX_N, sizeof(int*));
    for (int i  = 0; i < MASK_MATRIX_N; i++) {
        result_matrix[i] = (int*) calloc( MASK_MATRIX_M, sizeof(int));
    }

    // Copy values into the allocated memory from mask_matrix in cond_mask_matrix.c
    for(int n = 0; n < MASK_MATRIX_N; n++) {
        for(int m = 0; m < MASK_MATRIX_M; m++) {
            result_matrix[n][m] = mask_matrix[n][m];
        }
    }

    // Column-wise AND operation
    for(int m = 0; m < MASK_MATRIX_M; m++) {
        // result_row and mask_row should be the current row of their respective matrices
        for(int n = 0; n < MASK_MATRIX_N; n++) {
            // Set the n-th element of the current result_row to be the ANDed values of the n-th element in data_vec and current mask_row
            result_matrix[n][m] = data_vec[n] & mask_matrix[n][m];
            
        }
    }

    return result_matrix;
}

