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

void int_matrix_display(int_matrix *matrix) {
    for (size_t i = 0; i<matrix->m; i++) {
        for (size_t j = 0; j<matrix->n; j++) {
            printf("%5d", matrix->data[i][j]);
        }
        printf("\n");
    }
}

void plsc_aux(int_matrix* matrix, token_list* ref_tokens, token_list* src_tokens, int i, int j) {
    if (i == 0 || j == 0) {
        matrix->data[i][j] = 0;
    } else if (matrix->data[i][j] == -1) {
        if (token_equals(ref_tokens->tokens[i], src_tokens->tokens[j])) {
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
    int_matrix* res = int_matrix_create(ref_tokens->length, src_tokens->length);
    for (int i = 0; i<res->m; i++) {
        for (int j = 0; j<res->n; j++) {
            res->data[i][j] = -1;
        }
    }

    for (int i = 0; i<res->m; i++) {
        for (int j = 0; j<res->n; j++) {
            if (res->data[i][j] == -1) {
                plsc_aux(res, ref_tokens, src_tokens, i, j);
            }
        }
    }

    return res;
}


token_list* plsc(token_list* ref_tokens, token_list* src_tokens){
    int_matrix* m=plsc_lengths(ref_tokens, src_tokens);
    token_list* res=token_list_create(m->data[m->m][m->n]);
    int i=m->m;
    int j=m->n;
    int x=0;
    while(i!=0||j!=0){
        if(token_equals(ref_tokens->tokens[i],ref_tokens->tokens[j])){
            i--;
            j--;
            res->tokens[res->length-1-(x++)]=ref_tokens->tokens[i];
        }
        else{
            if(m->data[i-1][j]>=m->data[i][j-1]){
                i--;
            } else {
                j--;
            }
        }
    }
    return res;
}