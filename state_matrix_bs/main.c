#include <stdio.h>

#include "headers/cond_mask_matrix.h"

// Data vector given by example
// See "Logikkstyring"-slides, TTK4235 Jan. 16 2020, slide 36
static const int data_vec[MASK_MATRIX_N] = {1, 1, 1, 0, 0, 0, 0, 0};

int main(){

    matrix_s* m = create_matrix(MASK_MATRIX_N,MASK_MATRIX_M, mask_matrix); 
    printf("\nData:\n");
    for(int i = 0; i < sizeof(data_vec)/sizeof(data_vec[0]); i++) {
        printf("%d ", data_vec[i]);
        printf("\n");
    }

    printf("\n\nBefore ANDing:\n");
    print_matrix(m);

    printf("\n\nAfter ANDing:\n");
    matrix_AND_data(m, data_vec);
    print_matrix(m);

    return 0;
}