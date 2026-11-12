#include "strhash.h"
#include "tokenize.h"
#include "unity_internals.h"
#include <stddef.h>
#include <stdlib.h>

void token_list_create_free_test() {
    token_list* l = token_list_create(20);
    token_list_free(l);
    l = token_list_create(0);
    token_list_free(l);
}

void token_create_test() {
    strhash* s = strhash_create(10);
    token_list* t = token_list_create(20);
    size_t offset = 0;
    t->tokens[0] = token_create("Ceci est     une  phrase \t la la\n     la", &offset, s);
    token_list_free(t);
    strhash_free(s);
}

void tokenize_test() {
    RUN_TEST(token_list_create_free_test);
    RUN_TEST(token_create_test);
}
