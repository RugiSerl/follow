#pragma once
#include <stddef.h>
#include "strhash.h"

struct token {
    enum { WORD, SHORT_SPACE, SPACE, ERASE, INSERT, REPLACE, EMPTY } type;
    int textOffset;
    union {
        char* word ;
        char space[4];
    } data ;
};

typedef struct {
    struct token* tokens;
    size_t length;
} token_list;

/**
 * Transforme un texte en une liste de token.
 */
token_list* tokenize_text(char* text, strhash* stockage);

/**
 * Alloue une table de hachage.
 */
token_list* token_list_create(size_t length);

/**
 * Libère les données d'une table de hachage.
 */
void token_list_free(token_list* list);
