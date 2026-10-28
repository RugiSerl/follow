#pragma once
#include "list.h"
#include <stddef.h>

typedef struct {
    s_node** entries;       // Tableau contenant les entrées.
    size_t* entry_sizes;    // Tableau stockant la taille de toutes les entrées.
    size_t capacity;        // Taille de la table de hachage.
    size_t size;            // Nombre d'éléments dans le tableau.
} strhash;

typedef struct {
    size_t size;            // Nombre d'éléments dans la table de hachage
    size_t entry_min;       // Nombre minimum d'éléments au sein d'une entrée
    size_t entry_max;       // Nombre maximum d'éléments au sein d'une entrée
    double entry_avg;       // Nombre moyen d'éléments dans une entrée
    double entry_stdev;     // Écart type du nombre d'éléments dans une entrée
} strhash_stats;

#define MIN(x,y) ((x)<(y))?(x):(y)
#define MAX(x,y) ((x)>(y))?(x):(y)

/**
 * Génère un hash à partir d'une chaîne de caractères en utilisant un algorithme basique.
 */
unsigned int _hash_key(char* str);

/**
 * Fonction de comparaison pour les chaînes de caractères.
 */
int compare_string(s_node* node, void* value);

/**
 * Crée une nouvelle table de hashage vide.
 * Si l'allocation a échoué, alors NULL est renvoyé.
 */
strhash* strhash_create(unsigned int size);

/**
 * Libère/détruit une table de hachage.
 */
void strhash_free(strhash* hashtable);

/**
 * Insère un élément dans la table de hachage s'il n'est pas déjà inséré
 */
int strhash_add(strhash* hashtable, char* value);

/**
 * Enlève un élément de la table de hachage, s'il est bien dedans.
 */
int strhash_remove(strhash* hashtable, char* value);

/**
 * Vérifie si un élément est présent dans la table de hachage.
 */
int strhash_contains(strhash* hashtable, char* value);

/**
 * Vérifie si deux tables de hachage contiennent les mêmes éléments.
 */
int strhash_equal(strhash* s1, strhash* s2);

/**
 * Permet de récupérer des statistiques sur la table de hachage.
 */
strhash_stats strhash_get_stats(strhash* hashtable);

/**
 * Permet d'afficher dans la console le contenu de la table de hachage.
 */
void strhash_display(strhash* hashtable);
