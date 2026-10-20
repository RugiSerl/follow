#include "list.h"
#include "unity.h"
#include "unity_internals.h"
#include <stdio.h>
#include <stdlib.h>

void list_create_test() {
    s_node* list = list_create();
    TEST_ASSERT_EQUAL_PTR(NULL, list);
}

void list_insert_test() {
    s_node* list = list_create();
    TEST_ASSERT_EQUAL_PTR(NULL, list);
    double data = 5.0;
    list = list_insert(list, &data);
    TEST_ASSERT_NOT_EQUAL_PTR(NULL, list);
    TEST_ASSERT_EQUAL_PTR(NULL, list->next);
    TEST_ASSERT_EQUAL_PTR(&data, list->data);
    double data2 = 10.0;
    list = list_insert(list, &data2);
    TEST_ASSERT_NOT_EQUAL_PTR(NULL, list);
    TEST_ASSERT_NOT_EQUAL_PTR(NULL, list->next);
    TEST_ASSERT_EQUAL_PTR(NULL, list->next->next);
    TEST_ASSERT_EQUAL_PTR(&data2, list->data);

    free(list->next);
    free(list);
}

void list_get_data_test() {
    s_node* list = list_create();
    TEST_ASSERT_EQUAL_PTR(NULL, list);
    TEST_ASSERT_EQUAL_PTR(NULL, list_get_data(list));
    double data = 5.0;
    list = list_insert(list, &data);
    TEST_ASSERT_EQUAL_PTR(&data, list_get_data(list));
    double data2 = 40.0;
    list = list_insert(list, &data2);
    TEST_ASSERT_EQUAL_PTR(&data2, list_get_data(list));

    free(list->next);
    free(list);
}

void list_test() {
    RUN_TEST(list_create_test);
    RUN_TEST(list_insert_test);
    RUN_TEST(list_get_data_test);
}
