#include "plsc.h"
#include "strhash.h"
#include "tokenize.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int_matrix* int_matrix_create(size_t m, size_t n) {
    int_matrix* matrix = (int_matrix*) malloc(sizeof(int_matrix));
    if (matrix == NULL) return NULL;
    matrix->m = m;
    matrix->n = n;
    matrix->data = (int**) malloc(sizeof(int*) * m);
    if(matrix->data==NULL){
        free(matrix);
        return NULL;
    }
    for (size_t i = 0; i<m; i++) {
        matrix->data[i] = (int*) malloc(sizeof(int) * n);
        if(matrix->data[i]==NULL){
            int_matrix_free(matrix);
            return NULL;
        }
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

void int_matrix_display(int_matrix *matrix) {
    for (size_t i = 0; i<matrix->m; i++) {
        for (size_t j = 0; j<matrix->n; j++) {
            printf("%5d", matrix->data[i][j]);
        }
        printf("\n");
    }
}

void plsc_aux(int_matrix* matrix, token_list* ref_tokens, token_list* src_tokens, size_t i, size_t j) {
    if (i == 0 || j == 0) return;
    if (matrix->data[i][j] == -1) {
        if (token_equals(ref_tokens->tokens[i-1], src_tokens->tokens[j-1])) {
            plsc_aux(matrix, ref_tokens, src_tokens, i-1, j-1);
            matrix->data[i][j] = matrix->data[i-1][j-1] + 1;
        } else {
            plsc_aux(matrix, ref_tokens, src_tokens, i, j-1);
            plsc_aux(matrix, ref_tokens, src_tokens, i-1, j);
            matrix->data[i][j] = MAX(matrix->data[i][j-1], matrix->data[i-1][j]);
        }
    }
}

int_matrix* plsc_lengths(token_list* ref_tokens, token_list* src_tokens) {
    int_matrix* res = int_matrix_create(ref_tokens->length+1, src_tokens->length+1);
    for (size_t i = 0; i<res->m; i++) {
        for (size_t j = 0; j<res->n; j++) {
            if (i == 0 || j == 0) {
                res->data[i][j] = 0;
            } else {
                res->data[i][j] = -1;
            }
        }
    }

    for (size_t i = 1; i<res->m; i++) {
        for (size_t j = 1; j<res->n; j++) {
            if (res->data[i][j] == -1) {
                plsc_aux(res, ref_tokens, src_tokens, i, j);
            }
        }
    }

    return res;
}

// On va faire comme pour plsc mais cette fois ci en faisant gaffe au nouveau token avec ERASE, REPLACE, ...
token_list* diff(token_list* ref_tokens, token_list* src_tokens) {
    int_matrix* m = plsc_lengths(ref_tokens, src_tokens);
    token_list* res = token_list_create(m->data[m->m-1][m->n-1]);
    size_t i = m->m-1;
    size_t j = m->n-1;
    while (i > 0 && j > 0) {
        if (token_equals(ref_tokens->tokens[i-1], src_tokens->tokens[j-1])) {
            // Attention on ajoute à la fin alors qu'on devrait faire au début.
            // La liste va donc être à l'envers.
            token_list_add(res, token_duplicate(ref_tokens->tokens[i-1]));
            i--;
            j--;
        } else {
            if (m->data[i-1][j] > m->data[i][j-1]) {
                i--;
                token* t = token_empty();
                t->data.token_referenced = ref_tokens->tokens[i];
                t->type = ERASE;
                token_list_add(res, t);
            } else if(m->data[i][j-1] > m->data[i-1][j]){
                j--;
                token* t = token_empty();
                t->type = INSERT;
                t->data.token_referenced = src_tokens->tokens[j];
                token_list_add(res, t);
            }
            else{
                j--;
                i--;
                token* t = token_empty();
                t->type = REPLACE;
                t->data.token_referenced = src_tokens->tokens[j];
                token_list_add(res, t);
            }
        }
    }

    // On n'a plus besoin de la matrice maintenant.
    int_matrix_free(m);

    // Étant donné qu'on n'a pas forcément regardé en entier les deux listes de tokens,
    // on parcourt bien le reste des deux.
    while (i > 0) {
        i--;
        token* t = token_empty();
        t->data.token_referenced = ref_tokens->tokens[i];
        t->type = ERASE;
        token_list_add(res, t);
    }
    while (j > 0) {
        j--;
        token* t = token_empty();
        t->data.token_referenced = src_tokens->tokens[j];
        t->type = INSERT;
        token_list_add(res, t);
    }

    // On renverse la liste des tokens :
    for ( i = 0; i<res->length/2; i++) {
        token* temp = res->tokens[i];
        res->tokens[i] = res->tokens[res->length-1-i];
        res->tokens[res->length-1-i] = temp;
    }

    return res;
}
