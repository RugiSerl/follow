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
    TEST_ASSERT_EQUAL_INT(0, s->size);
    free(s->entries);
    free(s->entry_sizes);
    free(s);
    s = strhash_create(50);
    TEST_ASSERT_EQUAL_INT(50, s->capacity);
    TEST_ASSERT_EQUAL_INT(0, s->size);
    strhash_free(s);
}

void strhash_contains_test() {
    strhash* s = strhash_create(10);
    TEST_ASSERT_EQUAL_INT(10, s->capacity);
    TEST_ASSERT_EQUAL_INT(0, s->size);
    char s1[]="test", s2[]="super", s3[]="cool", s4[]="génial", s5[]="génie";
    strhash_add(s, s1);
    strhash_add(s, s2);
    strhash_add(s, s3);
    strhash_add(s, s4);
    strhash_add(s, s5);
    TEST_ASSERT_EQUAL_INT(10, s->capacity);
    TEST_ASSERT_EQUAL_INT(5, s->size);
    TEST_ASSERT(strhash_contains(s, "super"));
    TEST_ASSERT(!strhash_contains(s, "er"));
    TEST_ASSERT(!strhash_contains(s, "superieur"));
    TEST_ASSERT(!strhash_contains(s, "géni"));
    TEST_ASSERT(strhash_contains(s, "génial"));
    TEST_ASSERT(strhash_contains(s, "génie"));
    TEST_ASSERT(strhash_contains(s, "test"));
    strhash_free(s);
}

void strhash_get_stats_test() {
    strhash* s = strhash_create(10);
    TEST_ASSERT_EQUAL_INT(10, s->capacity);
    TEST_ASSERT_EQUAL_INT(0, s->size);
    char s1[]="test", s2[]="super", s3[]="cool", s4[]="génial", s5[]="génie";
    strhash_add(s, s1);
    strhash_add(s, s2);
    strhash_add(s, s3);
    strhash_add(s, s4);
    strhash_add(s, s5);
    TEST_ASSERT_EQUAL_INT(10, s->capacity);
    TEST_ASSERT_EQUAL_INT(5, s->size);
    strhash_stats stats = strhash_get_stats(s);
    TEST_ASSERT_EQUAL_INT(5, stats.size);
    TEST_ASSERT_EQUAL_INT(0, stats.entry_min);
    TEST_ASSERT_EQUAL_INT(2, stats.entry_max);
    TEST_ASSERT_DOUBLE_WITHIN(0.001, 0.5, stats.entry_avg);
    TEST_ASSERT_DOUBLE_WITHIN(0.001, 0.670820393, stats.entry_stdev);
    strhash_free(s);
}

void strhash_test() {
    RUN_TEST(hash_key_test);
    RUN_TEST(strhash_create_free_test);
    RUN_TEST(strhash_contains_test);
    RUN_TEST(strhash_get_stats_test);
}
