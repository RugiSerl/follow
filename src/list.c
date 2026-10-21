#include "list.h"
#include "stddef.h"
#include <stdlib.h>

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


s_node * liste_next(s_node * node){
    return node->next;
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
    if(head==NULL)return NULL;
    if(head->data==data)return head->next;
    s_node * suivant = head->next; // On commence par le 2e élément
    //boucle pour aller jusqu'à la queue ou jusquà la première instance de data
    while(suivant->next!=NULL || suivant->data==data){
        suivant=suivant->next;
    }
    if (suivant->next==NULL)return head;
    free(suivant->next);
    suivant->next=(suivant->next)->next;
    return head;
}

s_node * listheadRemove(s_node * head){
    s_node * new_head=head->next;
    free(head);
    return new_head;
}

void list_destroy(s_node * head){
    while(head!=NULL){
        head = listheadRemove(head);
    } 
}