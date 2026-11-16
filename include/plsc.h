#pragma once
#include <stddef.h>


typedef struct {
    int** data;
    size_t m;
    size_t n;
} int_matrix;

/**
 * Construit une nouvelle matrice m x n et initialise tous les coefficients à 0.
 */
int_matrix* int_matrix_create(size_t m, size_t n);

/**
 * Libère une matrice.
 */
void int_matrix_free(int_matrix* matrix);
