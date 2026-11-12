#include "tokenize.h"
#include <stddef.h>
#include <stdlib.h>

token_list* token_list_create(size_t length) {
    token_list* res = malloc(sizeof(token_list));
    res->length = length;
    res->tokens = malloc(sizeof(struct token) * length);
    return res;
}

void token_list_free(token_list* list) {
    free(list->tokens);
    free(list);
}
