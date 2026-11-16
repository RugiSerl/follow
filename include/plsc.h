#pragma once
#include "tokenize.h"
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

/**
 * Affiche la matrice dans stdout.
 */
void int_matrix_display(int_matrix* matrix);

/**
 * Créé la matrice où (c)_ij = "longueur des PLSCs entre ref_tokens[i] et ref_tokens[j]."
 */
int_matrix* plsc_lengths(token_list* ref_tokens, token_list* src_tokens);
