#include "strhash.h"
#include "list.h"
#include <stdlib.h>
#include <string.h>

int _hash_key(char* str) {
    int key = 0;
    while (*str != '\0') {
        key = key * 2 + *str;
        str++;
    }
    return key;
}

strhash* strhash_create(unsigned int size) {
    strhash* s = (strhash*)malloc(sizeof(strhash));
    s->size = size;
    s->entries = (s_node**)malloc(sizeof(s_node*)*size);
    for (unsigned int i = 0; i<s->size; i++) {
        s->entries[i] = list_create();
    }
    return s;
}

void strhash_free(strhash *hashtable) {
    for (unsigned int i = 0; i<hashtable->size; i++) {
        list_destroy(hashtable->entries[i]);
    }
    free(hashtable->entries);
    free(hashtable);
}


int strhash_add(strhash* hashtable, char* value){
    int key=_hash_key(value);
    for (s_node *n = hashtable->entries[key]; n != NULL; n = list_next(n)){
        if(strcmp(n->data, value) == 0){
        return 0;
        }
    }
    hashtable->entries[key]=list_insert(hashtable->entries[key],value);
    return 1;
}


int strhash_remove(strhash* hashtable, char* value){
    int key=_hash_key(value);
    for (s_node *n = hashtable->entries[key]; n != NULL; n = list_next(n)){
        if(strcmp(n->data, value) == 0){
            hashtable->entries[key]=list_remove(hashtable->entries[key],value);
            return 1;
        }
    }
    return 0;
}