#include <stdio.h>

#include "headers/cond_mask_matrix.h"

int main(){
    int** testANDED = colwise_AND();

    printf("Before ANDing\n");
    print_mask_matrix();
    printf("\n\n\nAfter ANDing\n");

    for(int i = 0; i < MASK_MATRIX_N; i++) {
        for(int j = 0; j < MASK_MATRIX_M; j++) {

            printf("%d ", testANDED[i][j]);
            printf("\t");
        }
        printf("\n");
    }

    return 0;
}