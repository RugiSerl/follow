#include "hashtable.h"
#include "unity.h"
#include "unity_internals.h"
#include <stdio.h>


void hash_key_test() {
    char maChaine[] = "Ceci est une chaîne";
    char maChaine2[] = "Ceci est une autre chaîne";
    int hash1 = _hash_key(maChaine);
    int hash2 = _hash_key(maChaine2);
    TEST_ASSERT_NOT_EQUAL_INT(hash1, hash2);
    TEST_ASSERT_EQUAL_INT(85864161, hash1);
    TEST_ASSERT_EQUAL_INT(1200507617, hash2);
}

void hashtable_test() {
    RUN_TEST(hash_key_test);
}
