#include "tokenize.h"
#include "strhash.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* openfile(char* src){
    unsigned int nb=0;
    FILE* f=fopen(src,"r");
    if(f == NULL) return NULL;
    char c=fgetc(f);
    while(c!=EOF) nb++; //première boucle pour connaitre le nombre de caractères dans le fichier
    char* dest=malloc(sizeof(char)*nb +1);
    rewind(f);
    c=fgetc(f);
    unsigned int i=0;
    while(c!=EOF){ //deuxième boucle pour remplir la chaine de caractère
        dest[i++]=c;
    }
    fclose(f);
    return dest;
}


token_list* token_list_create(size_t length) {
    token_list* res = (token_list*)malloc(sizeof(token_list));
    res->length = length;
    res->tokens = (token**)malloc(sizeof(token*) * length);
    for (int i = 0; i<length; i++) {
        res->tokens[i] = NULL;
    }
    return res;
}

void token_list_free(token_list* list) {
    for (int i = 0; i<list->length; i++) {
        token_free(list->tokens[i]);
    }
    free(list->tokens);
    free(list);
}

int is_delimiter(char c) {
    return c == ' ' || c == '\t' || c == '\n';
}

token* token_create(char *text, size_t *offset, strhash *stockage) {
    token* t = (token*)malloc(sizeof(token));
    t->textOffset = *offset;
    unsigned int space_length = 0;

    // Détermination du type et augmentation de offset pour arriver à la fin du mot/space.
    if (text[*offset] == ' ') {
        while (text[*offset] != '\0' && is_delimiter(text[*offset])) {
            (*offset)++;
            space_length++;
        }
        if (space_length <= 4) {
            t->type = SHORT_SPACE;
        } else {
            t->type = SPACE;
        }
    } else {
        t->type = WORD;
        while (text[*offset] != '\0' && !is_delimiter(text[*offset])) {
            (*offset)++;
        }
    }

    // Insertion du mot/space dans t.data.
    size_t data_size = *offset - t->textOffset;
    if (t->type == SHORT_SPACE) {
        strncpy(t->data.space, &text[t->textOffset], data_size);
    } else {
        char* str = (char*)malloc(sizeof(char)*(data_size+1));
        strncpy(str, &text[t->textOffset], data_size);
        str[data_size] = '\0';
        printf("%zu\n", data_size);
        t->data.word = strhash_add(stockage, str);
        free(str);
    }

    return t;
}

void token_free(token *token) {
    free(token);
}

token_list* tokenize_text(char *text, strhash *stockage) {
    token_list* res = token_list_create(100);
    size_t token_count = 0;
    size_t i = 0;
    while (text[i] != '\0') {
        res->tokens[token_count++] = token_create(text, &i, stockage);
    }
    return res;
}
