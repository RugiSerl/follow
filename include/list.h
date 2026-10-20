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
