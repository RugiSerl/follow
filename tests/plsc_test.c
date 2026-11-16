#include "plsc.h"
#include "tokenize.h"
#include "unity_internals.h"

void int_matrix_create_free_test() {
    int_matrix* mat = int_matrix_create(10, 10);
    int_matrix_free(mat);
}

void plsc_lengths_test() {
    text_analysis* t = text_analysis_create("assets/test_ref2.txt", "assets/test_src2.txt");
    int_matrix* mat = plsc_lengths(t->ref_tokens, t->src_tokens);

    int_matrix_display(mat);
    int_matrix_free(mat);
    text_analysis_free(t);
}

void plsc_test() {
    RUN_TEST(int_matrix_create_free_test);
    RUN_TEST(plsc_lengths_test);
}
