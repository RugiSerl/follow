#pragma once

typedef struct _list_node s_node;
struct _list_node{
    void * data;
    struct _list_node * next;
};

/**
 * Crée une liste vide, correspondant au pointeur NULL.
 */
s_node * list_create();

/**
 * Récupère la valeur de la tête de la liste.
 * Si la liste est vide, alors on renvoie NULL.
 */
void *list_get_data(s_node * node);

/**
 * Met à jour la valeur de la tête de la liste.
 * Si la liste est vide, alors on ne fait rien
 */
void list_set_data(s_node * node, void * data);

/**
 * Ajoute un élément en tête de liste.
 * Renvoie le nouveau chainon qui correspond à la nouvelle tête de liste
 * Si l'allocation a échoué, alors cette valeur vaut le pointeur NULL.
 */
s_node * list_insert(s_node * head, void * data);

/**
 * Ajoute un élément à la fin de la liste.
 * Renvoie le nouveau chainon qui correspond à la nouvelle tête de liste
 * Si l'allocation a échoué, alors cette valeur vaut le pointeur NULL.
 */
s_node * list_append(s_node * head, void * data);

/**
* ajout d’un noeud dans une liste ordonnee
* selon le resulat de ” compare ” ;
* si la donner existe deja , elle n’est pas ajoutee 
*/
s_node * list_orderedAppend(s_node ** head, int(*compare)(s_node *,void *),void *param);

/**
* suppression de la premiere instance d’une
* donnee dans la liste , retourne la tete de liste
*/
s_node * list_remove(s_node * head, void * data);

/**
suppression de la tete de liste
// retourne la nouvelle tete de liste
 */
s_node * listheadRemove(s_node * head);

/**
* destruction d’une liste
* (La liberationdes donnees n’est pas prise en charge)
*/
void list_destroy(s_node * head);