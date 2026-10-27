#pragma once
#include "list.h"
#include <stddef.h>

typedef struct {
    s_node** entries;
    size_t* entry_sizes;
    size_t size;
} strhash;

/**
 * Génère un hash à partir d'une chaîne de caractères en utilisant un algorithme basique.
 */
unsigned int _hash_key(char* str);

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
 * Permet d'afficher des statistiques sur la table de hachage.
 */
void strhash_stats(strhash* hashtable);
