#include "strhash.h"
#include "list.h"
#include <math.h>
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

strhash* strhash_create(unsigned int capacity) {
    strhash* s = (strhash*)malloc(sizeof(strhash));
    s->capacity = capacity;
    s->size = 0;
    s->entries = (s_node**)malloc(sizeof(s_node*)*capacity);
    s->entry_sizes = (size_t*)malloc(sizeof(size_t)*capacity);
    for (unsigned int i = 0; i<s->capacity; i++) {
        s->entries[i] = list_create();
        s->entry_sizes[i] = 0;
    }
    return s;
}

void strhash_free(strhash *hashtable) {
    for (unsigned int i = 0; i<hashtable->capacity; i++) {
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
    size_t key = _hash_key(value)%hashtable->capacity;
    size_t l_before = list_length(hashtable->entries[key]);

    // L'usage de cette fonction est essentiel pour conserver l'ordre dans la liste chaînée.
    list_orderedAppend(&hashtable->entries[key], compare_string, value);

    size_t l_after = list_length(hashtable->entries[key]);
    hashtable->entry_sizes[key] = l_after;
    if (l_before == l_after) return 0;
    hashtable->size++;
    return 1;
}


int strhash_remove(strhash* hashtable, char* value){
    size_t key = _hash_key(value)%hashtable->capacity;
    for (s_node *n = hashtable->entries[key]; n != NULL; n = list_next(n)){
        if (strcmp(n->data, value) == 0) {
            hashtable->entries[key] = list_remove(hashtable->entries[key],value);
            hashtable->entry_sizes[key]--;
            hashtable->size--;
            return 1;
        }
    }
    return 0;
}

strhash_stats strhash_get_stats(strhash *hashtable) {
    strhash_stats s = { 0 };
    s.size = hashtable->size;
    if (s.size == 0) return s;
    s.entry_max = s.entry_min = hashtable->entry_sizes[0];
    s.entry_avg = ((double)s.size)/((double)hashtable->capacity);
    for (size_t i = 0; i<hashtable->capacity; i++) {
        // Calcul du min/max
        size_t size = hashtable->entry_sizes[i];
        s.entry_min = MIN(s.entry_min, size);
        s.entry_max = MAX(s.entry_max, size);

        // Calcul de l'écart type
        double d = s.entry_avg - (double)hashtable->entry_sizes[i];
        s.entry_stdev += d*d;
    }
    s.entry_stdev = sqrt(s.entry_stdev/(double)hashtable->capacity);
    return s;
}
