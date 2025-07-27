#include <stdlib.h>
#include <stdio.h>
#include "globals.h"

int** allocate_matrix() {
    int** matrix = (int**)malloc(numRows * sizeof(int*));
    for (int i = 0; i < numRows; i++) {
        matrix[i] = (int*)malloc(numCols * sizeof(int));
    }
    return matrix;
}

void free_matrix(int** matrix) {
    for (int i = 0; i < numRows; i++) {
        free(matrix[i]);
    }
    free(matrix);
}

int grid() {
    int** A = allocate_matrix();
    for (int i = 0; i < numRows; i++) {
        for (int j = 0; j < numCols; j++) {
            A[i][j] = 0;

        }
    }
}