#include "plsc.h"
#include "tokenize.h"
#include "unity_internals.h"

#define REF2 "assets/test_ref2.txt"
#define SRC2 "assets/test_src2.txt"

void int_matrix_create_free_test() {
    int_matrix* mat = int_matrix_create(10, 10);
    int_matrix_free(mat);
}

void plsc_lengths_test() {
    text_analysis* t = text_analysis_create(REF2, SRC2);
    int_matrix* mat = plsc_lengths(t->ref_tokens, t->src_tokens);

    // int_matrix_display(mat);
    int_matrix_free(mat);
    text_analysis_free(t);
}

void plsc_algo_test() {
    text_analysis* t = text_analysis_create(REF2, SRC2);
    token_list* list = plsc(t->ref_tokens, t->src_tokens);

    token_list_display(list);

    token_list_free(list);
    text_analysis_free(t);
}

void diff_test() {
    text_analysis* t = text_analysis_create(REF2, SRC2);
    token_list* list = diff(t->ref_tokens, t->src_tokens);

    token_list_display(list);

    token_list_free(list);
    text_analysis_free(t);
}

void plsc_test() {
    RUN_TEST(int_matrix_create_free_test);
    RUN_TEST(plsc_lengths_test);
    RUN_TEST(plsc_algo_test);
    RUN_TEST(diff_test);
}
