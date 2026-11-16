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

void tokenize_text_test() {
    strhash* s = strhash_create(10);
    char text[] = "Ceci est     une  phrase   ";
    token_list* t = token_list_create(10);
    tokenize_text(text, s, t);

    strhash* temp = strhash_from_string("Ceci|est|     |une|phrase", 10);
    TEST_ASSERT(strhash_equal(s, temp));
    strhash_free(temp);

    token_list_free(t);
    strhash_free(s);
}

void text_analysis_create_free_test() {
    text_analysis* t = text_analysis_create("fichierinexistant", "fichierinexistant");
    TEST_ASSERT_NULL(t);
    // Remarque : pas besoin de free si le chargement a échoué

    text_analysis* t2 = text_analysis_create("assets/test_ref.txt", "assets/test_src.txt");
    text_analysis_free(t2);
}

void text_analysis_test() {
    text_analysis* t = text_analysis_create("assets/test_ref.txt", "assets/test_src.txt");
    strhash* temp = strhash_from_string("ceci|n'est|pas|un|texte|génial,|salut.|est|bonjour,|super|au|revoir.", 10);
    TEST_ASSERT(strhash_equal(t->s, temp));
    strhash_free(temp);
    text_analysis_free(t);
}

void openfile_test(){
    char* test1 = openfile("assets/fichier_inexistant.txt");
    TEST_ASSERT_EQUAL_PTR(NULL,test1);
    char* test2 = openfile("assets/test_1_2_3.txt");
    TEST_ASSERT_EQUAL_STRING("test_1_2_3",test2);
    free(test2);
}


void token_list_add_test(){
    strhash* s = strhash_create(10);
    token_list* t_list = token_list_create(20);
    char text[] = "Ceci est     une  phrase   ";
    size_t offset = 0;

    token* t1 = token_create(text, &offset, s);
    token* t2 = token_create(text, &offset, s);


    TEST_ASSERT_EQUAL_INT(0,t_list->length);
    TEST_ASSERT_EQUAL_INT(1,token_list_add(t_list,t1));
    TEST_ASSERT_EQUAL_INT(1,t_list->length);
    TEST_ASSERT_EQUAL_INT(1,token_list_add(t_list,t2));
    TEST_ASSERT_EQUAL_INT(2,t_list->length);
    TEST_ASSERT_EQUAL_INT(1,token_list_add(t_list,NULL));
    TEST_ASSERT_EQUAL_INT(3,t_list->length);

    strhash_free(s);
    token_list_free(t_list);
}


void tokenize_test() {
    RUN_TEST(token_create_test);
    RUN_TEST(token_list_create_free_test);
    RUN_TEST(tokenize_text_test);
    RUN_TEST(text_analysis_create_free_test);
    RUN_TEST(text_analysis_test);
    RUN_TEST(openfile_test);
    RUN_TEST(token_list_add_test);
}
