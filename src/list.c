#include "list.h"

s_node * list_create(){
    s_node * liste = NULL;
    return liste;
}

void *list_get_data(s_node * node){
    return node->data;
}

void list_set_data(s_node * node, void * data){
    node->data = data;
}

s_node * list_insert(s_node * head, void * data){
    s_node * new_node = list_create();
    new_node->data = data;
    //l'ancienne tete devient le node apres le nouveau (donc le nouveau devient la tete)
    new_node->next = head;
    return new_node;
}

s_node * list_append(s_node * head, void * data){
    s_node * new_node = list_create();
    new_node->data = data;
    new_node->next = NULL;
    //si y a pas de tete alors le nouveau node devient la tete(et donc la queue)
    if(head->data==NULL)return new_node;
    s_node * next = head->next;
    //boucle pour aller jusqu'à la queue
    while(next->next!=NULL){
        next=next->next;
    }
    next->next=new_node;
    return head;
}