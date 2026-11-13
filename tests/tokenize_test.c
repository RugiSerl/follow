#include "strhash.h"
#include "tokenize.h"
#include "unity.h"
#include "unity_internals.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void token_list_create_free_test() {
    token_list* l = token_list_create(20);
    token_list_free(l);
    l = token_list_create(0);
    token_list_free(l);
}

void token_create_test() {
    strhash* s = strhash_create(10);
    token_list* t = token_list_create(20);
    char text[] = "Ceci est     une  phrase   ";
    size_t offset = 0;

    t->tokens[0] = token_create(text, &offset, s);
    TEST_ASSERT_EQUAL_INT(WORD, t->tokens[0]->type);
    TEST_ASSERT(strcmp("Ceci", t->tokens[0]->data.word) == 0);

    t->tokens[1] = token_create(text, &offset, s);
    TEST_ASSERT_EQUAL_INT(SHORT_SPACE, t->tokens[1]->type);
    TEST_ASSERT(strncmp(" ", t->tokens[1]->data.space, 1) == 0);

    t->tokens[2] = token_create(text, &offset, s);
    TEST_ASSERT_EQUAL_INT(WORD, t->tokens[2]->type);
    TEST_ASSERT(strcmp("est", t->tokens[2]->data.word) == 0);

    t->tokens[3] = token_create(text, &offset, s);
    TEST_ASSERT_EQUAL_INT(SPACE, t->tokens[3]->type);
    TEST_ASSERT(strcmp("     ", t->tokens[3]->data.word) == 0);

    t->tokens[4] = token_create(text, &offset, s);
    TEST_ASSERT_EQUAL_INT(WORD, t->tokens[4]->type);
    TEST_ASSERT(strcmp("une", t->tokens[4]->data.word) == 0);

    t->tokens[5] = token_create(text, &offset, s);
    TEST_ASSERT_EQUAL_INT(SHORT_SPACE, t->tokens[5]->type);
    TEST_ASSERT(strncmp("  ", t->tokens[5]->data.space, 2) == 0);

    t->tokens[6] = token_create(text, &offset, s);
    TEST_ASSERT_EQUAL_INT(WORD, t->tokens[6]->type);
    TEST_ASSERT(strncmp("phrase", t->tokens[6]->data.word, 2) == 0);

    t->tokens[7] = token_create(text, &offset, s);
    TEST_ASSERT_EQUAL_INT(SHORT_SPACE, t->tokens[7]->type);
    TEST_ASSERT(strncmp("   ", t->tokens[7]->data.space, 2) == 0);

    TEST_ASSERT_EQUAL_INT(strlen(text), offset);

    token_list_free(t);
    strhash_free(s);
}

void tokenize_test() {
    RUN_TEST(token_list_create_free_test);
    RUN_TEST(token_create_test);
}
