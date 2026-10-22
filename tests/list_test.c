#include "list.h"
#include "unity.h"
#include "unity_internals.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/* taille maximum de la chaine representant les valeurs dans la liste */
#define LIST_AS_A_STRING_MAX_SIZE 1024
/* taille maximum de la chaine representant la valeur d'un noeud de la liste */
#define NODE_AS_A_STRING_MAX_SIZE 100

/* -----------------------------------------------------------------
   Conversion d'un noeud pointant sur une donnée de type int
	en chaine de caracteres
   -----------------------------------------------------------------
   node		: pointeur sur le noeud à convertir
   buffer (PS) 	: tampon contenant la chaine obtenue
   size		: taille maximum de la chaine
   -----------------------------------------------------------------
   retourne 1 si la chaine converti est trop longue pour le tampon
	    0 sinon
   -----------------------------------------------------------------
*/
int node_int_to_string(s_node * node, char *buffer, int size) {
	int * value;
	int str_length;

	value=(int *)list_get_data(node);
	str_length=snprintf(buffer, size, "%d", *value);
	return (str_length >= size);  // voir man snprintf (return value)
}

int node_double_to_string(s_node * node, char *buffer, int size) {
	double * value;
	int str_length;

	value=(double *)list_get_data(node);
	str_length=snprintf(buffer, size, "%lg", *value);
	return (str_length >= size);  // voir man snprintf (return value)
}


/* -----------------------------------------------------------------
   Conversion des données d'une liste en chaine de caracteres
	Les donnéees sont separees par ->
   -----------------------------------------------------------------
   head		 : pointeur sur la tete de liste
   node_to_string: fonction pour convertir la valeur d'un noeud
   -----------------------------------------------------------------
   retourne  la chaine de caractères representant la liste
   -----------------------------------------------------------------
*/
char *  list_to_string (s_node * head, int (*node_to_string)(s_node * node, char * buffer, int buffer_size))  {

	static char list_as_a_string[LIST_AS_A_STRING_MAX_SIZE];
	static char buffer[NODE_AS_A_STRING_MAX_SIZE];
	int length = 0;

	int max_length = LIST_AS_A_STRING_MAX_SIZE - 5;
		// 5 : reserve pour la fin de chaine (null ou ...)

	while( head && length<max_length-1 ) {
		int append;
	 	(void)(*node_to_string)(head, buffer, NODE_AS_A_STRING_MAX_SIZE-1);

		append = snprintf( list_as_a_string+length, max_length - length, "%s", buffer);
		if ( append >= max_length-length )  length = max_length-1;
		else			                    length += append;

		append = snprintf( list_as_a_string+length ,
			            max_length - length, " -> ");
		if ( append >= max_length-length ) length = max_length-1;
		else			           length += append;

		head=head->next;
	}
	if (length==max_length-1)
		sprintf( list_as_a_string+length , " ...");
			// incomplet, tampon trop petit
	else
		sprintf( list_as_a_string+length , "null");

	return list_as_a_string;
}

/* -----------------------------------------------------------------
   Compare la chaine representant une liste a une chaine attendu
   -----------------------------------------------------------------
   head		 : pointeur sur la tete de liste
   node_to_string: fonction pour convertir la valeur d'un noeud
   waited	 : chaine de caractère attendue
   -----------------------------------------------------------------
   retourne 1 si le résultat recu est celui attendu
            0 sinon
   -----------------------------------------------------------------
*/
int check_list_state(s_node * head,  int (*node_to_string)(s_node * node, char * buffer, int size),
		 const char * waited) {
	char * obtained=list_to_string(head, node_to_string);
	if ( strcmp(waited, obtained) != 0 ) {
		fprintf(stderr, "Attendu : %s \n Obtenu : %s\n", waited, obtained);
		return 0;
	}
	return 1;
}



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

    free(list);


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
    RUN_TEST(list_append_test);
    RUN_TEST(list_orderedAppend_test);
    RUN_TEST(list_headRemove_test);
    RUN_TEST(list_general_test);
}
