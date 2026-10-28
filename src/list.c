#include "list.h"
#include "stddef.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

s_node * list_create(){
    return NULL;
}

void * list_get_data(s_node * node){
    if (node == NULL) return NULL;
    else return node->data;
}

void list_set_data(s_node * node, void * data){
    if (node != NULL) node->data = data;
}


s_node * list_next(s_node * node){
    return node->next;
}

size_t list_length(s_node *node) {
    if (node == NULL) {
        return 0;
    } else {
        return 1 + list_length(node->next);
    }
}

int list_contains(s_node *node, int (*compare)(s_node *, void *), void *data) {
    if (node == NULL) {
        return 0;
    } else {
        return compare(node, data)==0 || list_contains(node->next, compare, data);
    }
}

s_node * list_insert(s_node * head, void * data){
    s_node * new_node = (s_node *)malloc(sizeof(s_node));
    if (new_node == NULL) return NULL; // Propagation de l'erreur au contexte appelant.
    new_node->data = data;
    //l'ancienne tete devient le node apres le nouveau (donc le nouveau devient la tete)
    new_node->next = head;
    return new_node;
}

s_node * list_append(s_node * head, void * data){
    s_node * new_node = (s_node *)malloc(sizeof(s_node));
    if (new_node == NULL) return NULL; // Propagation de l'erreur au contexte appelant.
    new_node->data = data;
    new_node->next = NULL;
    //si y a pas de tete alors le nouveau node devient la tete(et donc la queue)
    if(head==NULL)return new_node; // C'est bien head qu'on compare à NULL, pas head->data
    s_node * next = head; // On commence bien par le premier élément !
    //boucle pour aller jusqu'à la queue
    while(next->next!=NULL){
        next=next->next;
    }
    next->next=new_node;
    return head;
}


s_node * list_remove(s_node * head, void * data){
    if (head == NULL) return NULL;                              // Cas liste vide
    else if (head->data == data) return list_headRemove(head);  // Cas où la liste commence par l'élément contenant data.
    else if (head->next == NULL) return head;                   // Cas où on arrive à la fin de la liste
    else if (head->next->data == data) {                        // Cas où on a trouvé l'élément dans la liste
        head->next = list_headRemove(head->next);               // On supprime l'élément et on raccorde les chainons autour.
        return head;
    } else {                                                    // Cas récursif, on propage la suppression en queue de liste.
        list_remove(head->next, data);
        return head;
    }
}

s_node * list_headRemove(s_node * head){
    if(head==NULL)return NULL;
    s_node * new_head=head->next; // sauvegarde temporairement la tête
    free(head);
    return new_head;
}

void list_destroy(s_node *head) {
    while (head) {
        head = list_headRemove(head);
    }
}

s_node * list_orderedAppend(s_node ** head, int(*compare)(s_node *,void *),void *param) {
    if (*head == NULL) { // Cas où la liste est vide
        *head = list_insert(NULL, param);
        return *head;
    } else if (compare(*head, param) > 0) { // Cas où on insère au début de la liste.
        *head = list_insert(*head, param);
        return *head;
    } else if (compare(*head, param) == 0) { // Cas ou l'élément est déjà présent en début de liste.
        return *head;
    }
    // Dans les autres cas (après le début de la liste) on n'a pas besoin de changer head.

    // Maintenant on va chercher à insérer entre c et c->next.
    s_node * c = *head;
    // On avance tant qu'on n'arrive pas à la fin et qu'on est inférieur à param...
    while (c->next != NULL && compare(c->next, param) < 0) c = c->next;

    if (c->next == NULL                 // Cas où on est arrivé à la fin de la liste
    || compare(c->next, param) != 0) {  // Cas où on rajoute un élément à l'intérieur de la liste
        // On ajoute entre c et c->next.
        c->next = list_insert(c->next, param);
    }
    // Remarque : dans le cas où l'élément existait déjà dans la liste, c->next correspond déjà à
    // l'élément donc on n'a rien à créer.
    return c->next;
}

// ----------------------------------------
// Fonctions d'affichage / test de la liste

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

int node_string_to_string(s_node * node, char *buffer, int size) {
	char * value;
	int str_length;

	value=(char *)list_get_data(node);
	str_length=snprintf(buffer, size, "%s", value);
	return (str_length >= size);  // voir man snprintf (return value)
}

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

int check_list_state(s_node * head,  int (*node_to_string)(s_node * node, char * buffer, int size), const char * waited) {
	char * obtained=list_to_string(head, node_to_string);
	if ( strcmp(waited, obtained) != 0 ) {
		fprintf(stderr, "Attendu : %s \n Obtenu : %s\n", waited, obtained);
		return 0;
	}
	return 1;
}
