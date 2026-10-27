#include "strhash.h"
#include "unity.h"
#include "unity_internals.h"
#include <stdio.h>
#include <stdlib.h>


void hash_key_test() {
    char maChaine[] = "Ceci est une chaîne";
    char maChaine2[] = "Ceci est une autre chaîne";
    unsigned int hash1 = _hash_key(maChaine);
    unsigned int hash2 = _hash_key(maChaine2);
    TEST_ASSERT_NOT_EQUAL_INT(hash1, hash2);
    TEST_ASSERT_EQUAL_INT(85867233, hash1);
    TEST_ASSERT_EQUAL_INT(1200510689, hash2);
}

void strhash_create_free_test() {
    strhash* s = strhash_create(10);
    TEST_ASSERT_EQUAL_INT(10, s->capacity);
    free(s->entries);
    free(s->entry_sizes);
    free(s);
    s = strhash_create(50);
    TEST_ASSERT_EQUAL_INT(50, s->capacity);
    strhash_free(s);
}

void strhash_test() {
    RUN_TEST(hash_key_test);
    RUN_TEST(strhash_create_free_test);
}
