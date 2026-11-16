#include "plsc.h"
#include <stddef.h>
#include <stdlib.h>

int_matrix* int_matrix_create(size_t m, size_t n) {
    int_matrix* matrix = (int_matrix*) malloc(sizeof(int_matrix));
    if (matrix == NULL) return NULL;
    matrix->m = m;
    matrix->n = n;
    matrix->data = (int**) malloc(sizeof(int*) * m);
    for (size_t i = 0; i<m; i++) {
        matrix->data[i] = (int*) malloc(sizeof(int) * n);
        for (size_t j = 0; j<n; j++) {
            matrix->data[i][j] = 0;
        }
    }
    return matrix;
}

void int_matrix_free(int_matrix *matrix) {
    for (size_t i = 0; i<matrix->m; i++) {
        free(matrix->data[i]);
    }
    free(matrix->data);
    free(matrix);
}
