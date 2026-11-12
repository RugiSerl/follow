#include "list.h"
#include "strhash.h"
#include "unity.h"
#include "unity_internals.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



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

void strhash_add_test() {
    strhash* s = strhash_create(5);
    char s1[]="test", s2[]="super", s3[]="cool";
    char* added = strhash_add(s, s1);
    TEST_ASSERT_NOT_EQUAL_PTR(NULL, added);
    TEST_ASSERT_NOT_EQUAL_PTR(s1, added);
    TEST_ASSERT_EQUAL_INT(0, strcmp(added, s1));
    TEST_ASSERT(strhash_contains(s, s1));
    TEST_ASSERT(!strhash_contains(s, s2));

    added = strhash_add(s, s2);
    TEST_ASSERT_NOT_EQUAL_PTR(NULL, added);
    TEST_ASSERT_NOT_EQUAL_PTR(s2, added);
    TEST_ASSERT_EQUAL_INT(0, strcmp(added, s2));
    TEST_ASSERT(strhash_contains(s, s1));
    TEST_ASSERT(strhash_contains(s, s2));
    TEST_ASSERT(!strhash_contains(s, s3));

    char* added2 = strhash_add(s, s2);
    TEST_ASSERT_NOT_EQUAL_PTR(NULL, added);
    TEST_ASSERT_EQUAL_PTR(added, added2);
    TEST_ASSERT(strhash_contains(s, s1));
    TEST_ASSERT(strhash_contains(s, s2));
    TEST_ASSERT(!strhash_contains(s, s3));

    strhash_free(s);
}

void strhash_remove_test() {
    strhash *s = strhash_create(5), *tmp;
    char s1[]="test", s2[]="super", s3[]="cool";

    strhash_add(s, s1);
    strhash_add(s, s2);
    strhash_add(s, s3);

    tmp = strhash_from_string("test|super|cool", 5);
    TEST_ASSERT(strhash_equal(s, tmp));
    strhash_free(tmp);

    TEST_ASSERT_EQUAL_INT(1, strhash_remove(s, s1));

    tmp = strhash_from_string("super|cool", 5);
    TEST_ASSERT(strhash_equal(s, tmp));
    strhash_free(tmp);

    TEST_ASSERT_EQUAL_INT(0, strhash_remove(s, "pasdanslaliste"));

    tmp = strhash_from_string("super|cool", 5);
    TEST_ASSERT(strhash_equal(s, tmp));
    strhash_free(tmp);

    strhash_add(s, "rara");
    strhash_add(s, "toto");

    TEST_ASSERT_EQUAL_INT(1, strhash_remove(s, "super"));

    tmp = strhash_from_string("cool|rara|toto", 5);
    TEST_ASSERT(strhash_equal(s, tmp));
    strhash_free(tmp);

    TEST_ASSERT_EQUAL_INT(0, strhash_remove(s, "super"));

    tmp = strhash_from_string("cool|rara|toto", 5);
    TEST_ASSERT(strhash_equal(s, tmp));
    strhash_free(tmp);

    TEST_ASSERT_EQUAL_INT(1, strhash_remove(s, "toto"));

    tmp = strhash_from_string("cool|rara", 5);
    TEST_ASSERT(strhash_equal(s, tmp));
    strhash_free(tmp);

    strhash_free(s);
}

void strhash_equal_test() {
    strhash* s1 = strhash_create(10);
    strhash* s2 = strhash_create(10);
    char c1[]="test", c2[]="super", c3[]="cool", c4[]="génial";
    TEST_ASSERT(strhash_equal(s1, s2));
    TEST_ASSERT(strhash_equal(s1, s1));
    strhash_add(s1, c1);
    strhash_add(s1, c2);
    strhash_add(s1, c3);
    strhash_add(s1, c4);
    TEST_ASSERT(!strhash_equal(s1, s2));
    strhash_add(s2, c1);
    strhash_add(s2, c2);
    strhash_add(s2, c3);
    strhash_add(s2, c4);
    TEST_ASSERT(strhash_equal(s1, s2));
    TEST_ASSERT(strhash_equal(s1, s1));
    strhash_free(s1);
    strhash_free(s2);

    s1 = strhash_create(10);
    s2 = strhash_create(2);
    TEST_ASSERT(strhash_equal(s1, s2));
    strhash_add(s1, c1);
    strhash_add(s1, c2);
    strhash_add(s1, c3);
    strhash_add(s1, c4);
    TEST_ASSERT(!strhash_equal(s1, s2));
    strhash_add(s2, c1);
    strhash_add(s2, c2);
    strhash_add(s2, c3);
    strhash_add(s2, c4);
    TEST_ASSERT(strhash_equal(s1, s2));
    strhash_free(s1);
    strhash_free(s2);
}

void strhash_from_string_test() {
    strhash* s1 = strhash_from_string("a|b|c|d|e|banane|tomate|x|x|u", 2);
    strhash* s2 = strhash_from_string("banane|d|c|b|e|e|tomate|a|x|u", 10);
    TEST_ASSERT(strhash_equal(s1, s2));
    strhash_free(s1);
    strhash_free(s2);

    char a[]="a", b[]="b", c[]="c";
    strhash* s3 = strhash_from_string("a|b|c", 3);
    strhash* s4 = strhash_create(3);
    TEST_ASSERT(!strhash_equal(s3, s4));
    strhash_add(s4, a);
    strhash_add(s4, b);
    strhash_add(s4, c);
    TEST_ASSERT(strhash_equal(s3, s4));
    strhash_free(s3);
    strhash_free(s4);
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
    RUN_TEST(strhash_create_free_test);
    RUN_TEST(strhash_contains_test);
    RUN_TEST(strhash_add_test);
    RUN_TEST(strhash_remove_test);
    RUN_TEST(strhash_equal_test);
    RUN_TEST(strhash_from_string_test);
    RUN_TEST(strhash_get_stats_test);
}
