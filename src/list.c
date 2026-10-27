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
