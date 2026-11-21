#include "tokenize.h"
#include "strhash.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

text_analysis* text_analysis_create(char *ref_path, char *src_path) {
    text_analysis* t = (text_analysis*) malloc(sizeof(text_analysis));
    t->ref = openfile(ref_path);
    t->src = openfile(src_path);
    t->t_hash = strhash_create(32);
    if (t->ref == NULL || t->src == NULL) {
        free(t->ref);
        free(t->src);
        strhash_free(t->t_hash);
        free(t);
        return NULL;
    }
    t->ref_tokens = token_list_create(32);
    t->src_tokens = token_list_create(32);
    tokenize_text(t->ref, t->t_hash, t->ref_tokens);
    tokenize_text(t->src, t->t_hash, t->src_tokens);
    return t;
}

void text_analysis_free(text_analysis *t) {
    if (t == NULL) return;
    strhash_free(t->t_hash);
    token_list_free(t->ref_tokens);
    token_list_free(t->src_tokens);
    free(t->ref);
    free(t->src);
    free(t);
}

char* openfile(char* src){
    FILE* f = fopen(src,"r");
    if (f == NULL) return NULL;
    struct stat st;
    stat(src,&st);
    char* dest = malloc((sizeof(char) * st.st_size) + 1);
    char c = fgetc(f);
    unsigned int i = 0;
    while (c != EOF) { // Boucle pour remplir la chaine de caractère.
        dest[i++] = c;
        c = fgetc(f);
    }
    fclose(f);
    dest[i] = '\0';
    return dest;
}


token_list* token_list_create(size_t capacity) {
    token_list* res = (token_list*)malloc(sizeof(token_list));
    res->length = 0;
    res->capacity = capacity;
    res->tokens = (token**)malloc(sizeof(token*) * capacity);
    for (int i = 0; i<capacity; i++) {
        res->tokens[i] = NULL;
    }
    return res;
}

void token_list_free(token_list* list) {
    if (list == NULL) return;
    for (int i = 0; i<list->capacity; i++) {
        token_free(list->tokens[i]);
    }
    free(list->tokens);
    free(list);
}

int token_list_add(token_list *list, token* t) {
    if (t==NULL)return 0;
    if (list->length+1 > list->capacity) {
        list->capacity *= 2;
        list->tokens = realloc(list->tokens, sizeof(token*) * list->capacity);
        if (list->tokens == NULL) {
            return 0;
        }
        // IMPORTANT : permet de ne pas avoir de problème quand on free la liste.
        for (int i = list->length+1; i<list->capacity; i++) {
            list->tokens[i] = NULL;
        }
    }
    list->tokens[list->length++] = t;
    return 1;
}

void token_list_to_string(token_list *list, char* dest) {
    dest[0] = '\0';
    for (int i = 0; i<list->length; i++) {
        char buf[100];
        token_to_string(list->tokens[i], buf);
        strcat(dest, buf);
    }
}

void token_list_display(token_list *list) {
    char buf[1000];
    token_list_to_string(list, buf);
    printf("%s\n", buf);
}

int is_delimiter(char c) {
    return c == ' ' || c == '\t' || c == '\n';
}

token* token_create(char *text, size_t *offset, strhash *stockage) {
    token* t = (token*)malloc(sizeof(token));
    t->textOffset = *offset;
    unsigned int space_length = 0;

    // Détermination du type et augmentation de offset pour arriver à la fin du mot/space.
    if (is_delimiter(text[*offset])) {
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
        if (data_size < 4) {
            t->data.space[data_size] = '\0';
        }
    } else {
        char* str = (char*)malloc(sizeof(char)*(data_size+1));
        strncpy(str, &text[t->textOffset], data_size);
        str[data_size] = '\0';
        t->data.word = strhash_add(stockage, str);
        free(str);
    }

    return t;
}

token* token_empty() {
    token* t = (token*) malloc(sizeof(token));
    t->textOffset = 0;
    t->type = EMPTY;
    return t;
}

void token_free(token *token) {
    free(token);
}

void tokenize_text(char *text, strhash *stockage, token_list* res) {
    size_t i = 0;
    while (text[i] != '\0') {
        token_list_add(res, token_create(text, &i, stockage));
    }
}

int token_equals(token *t1, token *t2) {
    if (t1->type != t2->type) return 0;
    if (t1->type == WORD || t1->type == SPACE) {
        return strcmp(t1->data.word, t2->data.word) == 0;
    } else {
        return strncmp(t1->data.space, t2->data.space, 4) == 0;
    }
}

token* token_duplicate(token *t) {
    token* new = malloc(sizeof(token));
    *new = *t;
    return new;
}

void token_to_string(token *t, char* dest) {
    char buf[20];
    switch (t->type) {
        case WORD: case SPACE:
            sprintf(dest, "%s", t->data.word);
            break;
        case SHORT_SPACE:
            {
                char* str = (char*)malloc(sizeof(char)*5);
                str[4] = '\0';
                strncpy(str, t->data.space, 4);
                sprintf(dest, "%s", str);
                free(str);
            }
            break;
        case ERASE:
            token_to_string(t->data.token_referenced, buf);
            sprintf(dest, "[ERASE:%s]", buf);
            break;
        case INSERT:
            token_to_string(t->data.token_referenced, buf);
            sprintf(dest, "[INSERT:%s]", buf);
            break;
        case REPLACE:
            sprintf(dest, "[REPLACE]");
            break;
        case EMPTY:
            sprintf(dest, "[EMPTY]");
            break;
        default:
            sprintf(dest, "[UNKNOWN_TOKEN]");
            break;
    }
}
