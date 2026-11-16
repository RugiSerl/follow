#pragma once
#include <stddef.h>
#include "strhash.h"

typedef enum {
    WORD, SHORT_SPACE, SPACE, ERASE, INSERT, REPLACE, EMPTY
} token_type;

typedef union {
    char* word;
    char space[4];
} token_data;

typedef struct {
    token_type type;
    int textOffset;
    token_data data;
} token;

typedef struct {
    token** tokens;
    size_t length;
    size_t capacity;
} token_list;

typedef struct {
    char* ref;
    char* src;
    strhash* t_hash;
    token_list* ref_tokens;
    token_list* src_tokens;
} text_analysis;

/**
 * Stocke les liste de tokens des textes.
 */
text_analysis* text_analysis_create(char* ref_path, char* src_path);

/**
 * Libère les données d'une analyse de texte.
 */
void text_analysis_free(text_analysis* t);

/**
 * Ouvre un fichier et le transforme en chaine de caractères
 */
char* openfile(char* src);

/**
 * Transforme un texte en une liste de token.
 */
void tokenize_text(char* text, strhash* stockage, token_list* res);

/**
 * Alloue une liste de tokens.
 */
token_list* token_list_create(size_t length);

/**
 * Libère les données d'une liste de tokens.
 */
void token_list_free(token_list* list);

/**
 * Ajoute un élément dans la liste de tokens et réalloue la liste si besoin.
 */
int token_list_add(token_list* list, token* token);

/**
 * Crée un token à partir du texte qui lui est donné en entrée et de l'emplacement (offset)
 * où il doit convertir le token.
 * Le offset est modifié par la suite.
 */
token* token_create(char* text, size_t* offset, strhash* stockage);

/**
 * Libère les données du token.
 */
void token_free(token* token);
