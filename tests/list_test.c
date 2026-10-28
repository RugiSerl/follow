#include "list.h"
#include "unity.h"
#include "unity_internals.h"
#include <assert.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void list_create_test() {
    s_node* list = list_create();
    TEST_ASSERT_EQUAL_PTR(NULL, list);
    TEST_ASSERT_EQUAL_INT(1, check_list_state(list, node_int_to_string, "null"));
}

void list_insert_test() {
    s_node* list = list_create();
    TEST_ASSERT_EQUAL_PTR(NULL, list);
    TEST_ASSERT_EQUAL_INT(1, check_list_state(list, node_int_to_string, "null"));
    int data = 5;
    list = list_insert(list, &data);
    TEST_ASSERT_NOT_EQUAL_PTR(NULL, list);
    TEST_ASSERT_EQUAL_PTR(NULL, list->next);
    TEST_ASSERT_EQUAL_PTR(&data, list->data);
    TEST_ASSERT_EQUAL_INT(1, check_list_state(list, node_int_to_string, "5 -> null"));
    int data2 = 10;
    list = list_insert(list, &data2);
    TEST_ASSERT_NOT_EQUAL_PTR(NULL, list);
    TEST_ASSERT_NOT_EQUAL_PTR(NULL, list->next);
    TEST_ASSERT_EQUAL_PTR(NULL, list->next->next);
    TEST_ASSERT_EQUAL_PTR(&data2, list->data);
    TEST_ASSERT_EQUAL_INT(1, check_list_state(list, node_int_to_string, "10 -> 5 -> null"));
    free(list->next);
    free(list);
}

void list_get_data_test() {
    s_node* list = list_create();
    TEST_ASSERT_EQUAL_PTR(NULL, list);
    TEST_ASSERT_EQUAL_PTR(NULL, list_get_data(list));
    TEST_ASSERT_EQUAL_INT(1, check_list_state(list, node_int_to_string, "null"));
    int data = 5;
    list = list_insert(list, &data);
    TEST_ASSERT_EQUAL_PTR(&data, list_get_data(list));
    TEST_ASSERT_EQUAL_INT(1, check_list_state(list, node_int_to_string, "5 -> null"));

    int data2 = 40;
    list = list_insert(list, &data2);
    TEST_ASSERT_EQUAL_PTR(&data2, list_get_data(list));
    TEST_ASSERT_EQUAL_INT(1, check_list_state(list, node_int_to_string, "40 -> 5 -> null"));

    free(list->next);
    free(list);
}

void list_set_data_test() {
    s_node* list = list_create();
    TEST_ASSERT_EQUAL_PTR(NULL, list);
    TEST_ASSERT_EQUAL_INT(1, check_list_state(list, node_int_to_string, "null"));
    int data = 5;
    list_set_data(list, &data);
    TEST_ASSERT_EQUAL_PTR(NULL, list);
    TEST_ASSERT_EQUAL_INT(1, check_list_state(list, node_int_to_string, "null"));
    list = list_insert(list, &data);
    TEST_ASSERT_EQUAL_INT(1, check_list_state(list, node_int_to_string, "5 -> null"));
    int data2 = 7;
    list_set_data(list, &data2);
    TEST_ASSERT_EQUAL_INT(1, check_list_state(list, node_int_to_string, "7 -> null"));
    free(list);
}

void list_next_test() {
    s_node* list = list_create();
    int d1=0, d2=5, d3=6;
    list = list_insert(list, &d1);
    list = list_insert(list, &d2);
    list = list_insert(list, &d3);
    TEST_ASSERT_EQUAL_INT(1, check_list_state(list, node_int_to_string, "6 -> 5 -> 0 -> null"));

    s_node* l2 = list_next(list);
    TEST_ASSERT_EQUAL_INT(1, check_list_state(l2, node_int_to_string, "5 -> 0 -> null"));
    l2 = list_next(l2);
    TEST_ASSERT_EQUAL_INT(1, check_list_state(l2, node_int_to_string, "0 -> null"));
    l2 = list_next(l2);
    TEST_ASSERT_EQUAL_INT(1, check_list_state(l2, node_int_to_string, "null"));

    free(list->next->next);
    free(list->next);
    free(list);
}

void list_length_test() {
    s_node* l = list_create();
    int d = 5, e = 10, f = 20;
    TEST_ASSERT_EQUAL_INT(0, list_length(l));
    l = list_insert(l, &d);
    TEST_ASSERT_EQUAL_INT(1, list_length(l));
    l = list_insert(l, &e);
    TEST_ASSERT_EQUAL_INT(2, list_length(l));
    l = list_remove(l, &d);
    TEST_ASSERT_EQUAL_INT(1, list_length(l));
    l = list_insert(l, &f);
    TEST_ASSERT_EQUAL_INT(2, list_length(l));
    l = list_insert(l, &e);
    TEST_ASSERT_EQUAL_INT(3, list_length(l));
    list_destroy(l);
}

void list_append_test() {
    s_node* list = list_create();
    TEST_ASSERT_EQUAL_PTR(NULL, list);
    TEST_ASSERT_EQUAL_INT(1, check_list_state(list, node_int_to_string, "null"));
    int data = 20;
    list = list_append(list, &data);
    TEST_ASSERT_EQUAL_INT(1, check_list_state(list, node_int_to_string, "20 -> null"));
    int data2 = 60;
    list = list_append(list, &data2);
    TEST_ASSERT_EQUAL_INT(1, check_list_state(list, node_int_to_string, "20 -> 60 -> null"));
    int data3 = 10;
    list = list_insert(list, &data3);
    TEST_ASSERT_EQUAL_INT(1, check_list_state(list, node_int_to_string, "10 -> 20 -> 60 -> null"));
    int data4 = 80;
    list = list_append(list, &data4);
    TEST_ASSERT_EQUAL_INT(1, check_list_state(list, node_int_to_string, "10 -> 20 -> 60 -> 80 -> null"));

    free(list->next->next->next);
    free(list->next->next);
    free(list->next);
    free(list);
}

int compare_int_basic(s_node* s, void* data) {
    assert(s != NULL && s->data != NULL && data != NULL);
    return *(int *)(s->data) - *(int *)data;
}

void list_orderedAppend_test() {
    s_node* list = list_create();
    int i=10, j=40, k=20, l=5, m=80;
    TEST_ASSERT_EQUAL_PTR(NULL, list);
    TEST_ASSERT_EQUAL_INT(1, check_list_state(list, node_int_to_string, "null"));
    s_node* added = list_orderedAppend(&list, compare_int_basic, &i);
    TEST_ASSERT_EQUAL_INT(1, check_list_state(list, node_int_to_string, "10 -> null"));
    TEST_ASSERT_EQUAL_PTR(list, added);
    added = list_orderedAppend(&list, compare_int_basic, &j);
    TEST_ASSERT_EQUAL_INT(1, check_list_state(list, node_int_to_string, "10 -> 40 -> null"));
    TEST_ASSERT_NOT_EQUAL_PTR(list, added);
    TEST_ASSERT_EQUAL_PTR(list->next, added);
    added = list_orderedAppend(&list, compare_int_basic, &j);
    TEST_ASSERT_EQUAL_INT(1, check_list_state(list, node_int_to_string, "10 -> 40 -> null"));
    TEST_ASSERT_NOT_EQUAL_PTR(list, added);
    TEST_ASSERT_EQUAL_PTR(list->next, added);
    added = list_orderedAppend(&list, compare_int_basic, &k);
    TEST_ASSERT_EQUAL_INT(1, check_list_state(list, node_int_to_string, "10 -> 20 -> 40 -> null"));
    TEST_ASSERT_EQUAL_PTR(list->next, added);
    added = list_orderedAppend(&list, compare_int_basic, &l);
    TEST_ASSERT_EQUAL_INT(1, check_list_state(list, node_int_to_string, "5 -> 10 -> 20 -> 40 -> null"));
    TEST_ASSERT_EQUAL_PTR(list, added);
    added = list_orderedAppend(&list, compare_int_basic, &m);
    TEST_ASSERT_EQUAL_INT(1, check_list_state(list, node_int_to_string, "5 -> 10 -> 20 -> 40 -> 80 -> null"));
    TEST_ASSERT_EQUAL_PTR(list->next->next->next->next, added);

    free(list->next->next->next->next);
    free(list->next->next->next);
    free(list->next->next);
    free(list->next);
    free(list);
}

void list_remove_test() {
    s_node* list = list_create();
    TEST_ASSERT_EQUAL_PTR(NULL, list);
    TEST_ASSERT_EQUAL_INT(1, check_list_state(list, node_int_to_string, "null"));
    int data = 20, data2 = 40, data3 = 60;
    list = list_insert(list, &data);
    list = list_append(list, &data2);
    list = list_append(list, &data3);
    TEST_ASSERT_EQUAL_INT(1, check_list_state(list, node_int_to_string, "20 -> 40 -> 60 -> null"));

    list = list_remove(list,&data);
    TEST_ASSERT_EQUAL_INT(1, check_list_state(list, node_int_to_string, "40 -> 60 -> null"));

    list = list_insert(list, &data);
    TEST_ASSERT_EQUAL_INT(1, check_list_state(list, node_int_to_string, "20 -> 40 -> 60 -> null"));
    list = list_remove(list,&data2);
    TEST_ASSERT_EQUAL_INT(1, check_list_state(list, node_int_to_string, "20 -> 60 -> null"));

    list = list_insert(list, &data2);
    list = list_remove(list,&data3);
    TEST_ASSERT_EQUAL_INT(1, check_list_state(list, node_int_to_string, "40 -> 20 -> null"));

    list = list_remove(list, &data);
    TEST_ASSERT_EQUAL_INT(1, check_list_state(list, node_int_to_string, "40 -> null"));

    list = list_remove(list, &data);
    TEST_ASSERT_EQUAL_INT(1, check_list_state(list, node_int_to_string, "40 -> null"));
    list = list_remove(list, &data2);
    TEST_ASSERT_EQUAL_INT(1, check_list_state(list, node_int_to_string, "null"));
}


void list_headRemove_test() {
    s_node* list = list_create();
    TEST_ASSERT_EQUAL_PTR(NULL, list);
    TEST_ASSERT_EQUAL_INT(1, check_list_state(list, node_int_to_string, "null"));
    int data = 20, data2 = 40, data3 = 60;
    list = list_insert(list, &data);
    list = list_append(list, &data2);
    list = list_append(list, &data3);
    TEST_ASSERT_EQUAL_INT(1, check_list_state(list, node_int_to_string, "20 -> 40 -> 60 -> null"));

    list = list_headRemove(list);
    TEST_ASSERT_EQUAL_INT(1, check_list_state(list, node_int_to_string, "40 -> 60 -> null"));

    list = list_headRemove(list);
    TEST_ASSERT_EQUAL_INT(1, check_list_state(list, node_int_to_string, "60 -> null"));

    list = list_headRemove(list);
    TEST_ASSERT_EQUAL_INT(1, check_list_state(list, node_int_to_string, "null"));


}

void list_destroy_test() {
    s_node* list = list_create();
    TEST_ASSERT_EQUAL_PTR(NULL, list);
    list_destroy(list);

    int data = 20, data2 = 40, data3 = 60;
    s_node* list2 = list_create();
    list2 = list_insert(list2, &data);
    list_destroy(list2);

    s_node* list3 = list_create();
    list3 = list_insert(list3, &data);
    list3 = list_append(list3, &data2);
    list3 = list_append(list3, &data3);
    list_destroy(list3);
}



void list_general_test(){
    int i=1, j=2;
    s_node * l=list_create();
    l=list_insert(l,&i);
    l=list_insert(l,&j);
    TEST_ASSERT_EQUAL_INT(1,check_list_state(l,&node_int_to_string,"2 -> 1 -> null"));
    free(l->next);
    free(l);
}

void list_test() {
    RUN_TEST(list_create_test);
    RUN_TEST(list_insert_test);
    RUN_TEST(list_get_data_test);
    RUN_TEST(list_set_data_test);
    RUN_TEST(list_next_test);
    RUN_TEST(list_length_test);
    RUN_TEST(list_append_test);
    RUN_TEST(list_orderedAppend_test);
    RUN_TEST(list_headRemove_test);
    RUN_TEST(list_remove_test);
    RUN_TEST(list_general_test);
    RUN_TEST(list_destroy_test);
}
