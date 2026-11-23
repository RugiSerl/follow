#include "plsc.h"
#include "strhash.h"
#include "tokenize.h"
#include "unity.h"
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

void diff_test() {
    text_analysis* t = text_analysis_create(REF2, SRC2);
    token_list* list = diff(t->ref_tokens, t->src_tokens);

    char buf[1000];
    token_list_to_string(list, buf);
    TEST_ASSERT_EQUAL_STRING("[INSERT:ceci][INSERT: ][INSERT:n'est][INSERT: ][REPLACE:pas] [REPLACE:un] texte génial,[ERASE: ][ERASE:au] [REPLACE:salut.]\n", buf);

    token_list_free(list);
    text_analysis_free(t);

    char ref[] = "Coucou ça va ?";
    char src[] = "Salut ça va.";
    token_list* ref_tokens = token_list_create(4);
    token_list* src_tokens = token_list_create(4);
    strhash* s = strhash_create(10);

    tokenize_text(ref, s, ref_tokens);
    tokenize_text(src, s, src_tokens);

    token_list* list_diff = diff(ref_tokens, src_tokens);

    token_list_to_string(list_diff, buf);
    TEST_ASSERT_EQUAL_STRING("[REPLACE:Salut] ça[ERASE: ][ERASE:va] [REPLACE:va.]", buf);

    token_list_free(list_diff);
    token_list_free(ref_tokens);
    token_list_free(src_tokens);
    strhash_free(s);
}

void plsc_test() {
    RUN_TEST(int_matrix_create_free_test);
    RUN_TEST(plsc_lengths_test);
    RUN_TEST(diff_test);
}
