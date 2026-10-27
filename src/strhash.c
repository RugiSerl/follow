#include "strhash.h"
#include "list.h"
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

unsigned int _hash_key(char* str) {
    unsigned int key = 0;
    while (*str != '\0') {
        key = key * 2 + (unsigned char)*str;
        str++;
    }
    return key;
}

strhash* strhash_create(unsigned int size) {
    strhash* s = (strhash*)malloc(sizeof(strhash));
    s->size = size;
    s->entries = (s_node**)malloc(sizeof(s_node*)*size);
    s->entry_sizes = (size_t*)malloc(sizeof(size_t)*size);
    for (unsigned int i = 0; i<s->size; i++) {
        s->entries[i] = list_create();
        s->entry_sizes[i] = 0;
    }
    return s;
}

void strhash_free(strhash *hashtable) {
    for (unsigned int i = 0; i<hashtable->size; i++) {
        list_destroy(hashtable->entries[i]);
    }
    free(hashtable->entries);
    free(hashtable->entry_sizes);
    free(hashtable);
}

int compare_string(s_node* node, void* value) {
    return strcmp((char*)node->data, (char*)value);
}

int strhash_add(strhash* hashtable, char* value){
    size_t key = _hash_key(value)%hashtable->size;
    size_t l1 = list_length(hashtable->entries[key]);

    // L'usage de cette fonction est essentiel pour conserver l'ordre dans la liste chaînée.
    list_orderedAppend(&hashtable->entries[key], compare_string, value);

    size_t l2 = list_length(hashtable->entries[key]);
    hashtable->entry_sizes[key] = l2;
    return l1 != l2;
}


int strhash_remove(strhash* hashtable, char* value){
    size_t key = _hash_key(value)%hashtable->size;
    for (s_node *n = hashtable->entries[key]; n != NULL; n = list_next(n)){
        if (strcmp(n->data, value) == 0) {
            hashtable->entries[key] = list_remove(hashtable->entries[key],value);
            hashtable->entry_sizes[key]--;
            return 1;
        }
    }
    return 0;
}
