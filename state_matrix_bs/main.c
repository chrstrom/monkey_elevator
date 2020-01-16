#include <stdio.h>

#include "headers/cond_mask_matrix.h"

// Data vector given by example
// See "Logikkstyring"-slides, TTK4235 Jan. 16 2020, slide 36
static const int data_vec[MASK_MATRIX_N] = {1, 1, 1, 0, 0, 0, 0, 0};

int main(){

    matrix_s* operative_matrix = create_matrix(MASK_MATRIX_N,MASK_MATRIX_M, mask_matrix); 
    matrix_s* conditional_matrix = create_matrix(COND_MATRIX_N,COND_MATRIX_M, cond_matrix);
    matrix_s* data_matrix = create_matrix(MASK_MATRIX_N, 1, data_vec);

    printf("\nData:\n");
    for(int i = 0; i < sizeof(data_vec)/sizeof(data_vec[0]); i++) {
        printf("%d ", data_vec[i]);
        printf("\n");
    }

    printf("\n\nBefore ANDing:\n");
    print_matrix(operative_matrix);

    matrix_AND_data(operative_matrix, data_matrix);

    printf("\n\nAfter ANDing:\n");
    print_matrix(operative_matrix);

    matrix_s* rule_vector = matrix_COMP_matrix(operative_matrix, conditional_matrix);
    printf("\n\nAfter row-wise comparison:\n");
    print_matrix(rule_vector);

    return 0;
}