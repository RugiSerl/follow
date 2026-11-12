#include "strhash.h"
#include "list.h"
#include <math.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int node_string_to_string(s_node * node, char *buffer, int size) {
	char * value;
	int str_length;

	value=(char *)list_get_data(node);
	str_length=snprintf(buffer, size, "%s", value);
	return (str_length >= size);  // voir man snprintf (return value)
}

unsigned int _hash_key(char* str, size_t max) {
    unsigned int key = 0;
    while (*str != '\0') {
        key = key * 2 + (unsigned char)*str;
        str++;
    }
    if (max > 0) {
        key = key % max;
    }
    return key;
}

int compare_string(s_node* node, void* value) {
    return strcmp((char*)node->data, (char*)value);
}

strhash* strhash_create(size_t capacity) {
    strhash* s = (strhash*)malloc(sizeof(strhash));
    if (s==NULL) return NULL;
    s->capacity = capacity;
    s->size = 0;
    s->entries = (s_node**)malloc(sizeof(s_node*)*capacity);
    if(s->entries == NULL) {
        free(s);
        return NULL;
    }
    s->entry_sizes = (size_t*)malloc(sizeof(size_t)*capacity);
    if(s->entry_sizes==NULL) {
        free(s->entries);
        free(s);
        return NULL;
    }
    for (unsigned int i = 0; i<s->capacity; i++) {
        s->entries[i] = list_create();
        s->entry_sizes[i] = 0;
    }
    return s;
}

void strhash_free(strhash *hashtable) {
    for (size_t i = 0; i<hashtable->capacity; i++) {
        s_node* l = hashtable->entries[i];
        while (l != NULL) {
            free(l->data);
            l = l->next;
        }
        list_destroy(hashtable->entries[i]);
    }
    free(hashtable->entries);
    free(hashtable->entry_sizes);
    free(hashtable);
}

char* strhash_add(strhash* hashtable, char* value){
    size_t key = _hash_key(value, hashtable->capacity);
    s_node* elt;
    if ((elt = list_find(hashtable->entries[key], compare_string, value))!=NULL) {
        return elt->data;
    } else {
        char* value_copy = strdup(value);
        elt = list_orderedAppend(&hashtable->entries[key], compare_string, value_copy);
        hashtable->entry_sizes[key]++;
        hashtable->size++;
        return elt->data;
    }
}

int strhash_remove(strhash* hashtable, char* value){
    size_t key = _hash_key(value, hashtable->capacity);

    s_node* head = hashtable->entries[key];
    if (head == NULL) return 0;
    if (head->next == NULL) {
        if (strcmp(head->data, value) == 0) {
            free(head->data);
            free(head);
            hashtable->entry_sizes[key] = 0;
            hashtable->size--;
            hashtable->entries[key] = NULL;
            return 1;
        } else {
            return 0;
        }
    }
    for (; head->next != NULL; head = list_next(head)) {
        if (strcmp(head->next->data, value) == 0) {
            s_node* tmp = head->next->next;
            free(head->next->data);
            free(head->next);
            head->next = tmp;
            hashtable->entry_sizes[key]--;
            hashtable->size--;
            return 1;
        }
    }
    return 0; // On n'a rien supprimé.
}

int strhash_contains(strhash *hashtable, char *value) {
    size_t key = _hash_key(value, hashtable->capacity);
    return list_find(hashtable->entries[key], compare_string, value) != NULL;
}

int strhash_equal(strhash *s1, strhash *s2) {
    // Première inclusion
    for (size_t i = 0; i<s1->capacity; i++) {
        s_node* l = s1->entries[i];
        while (l != NULL) {
            if (!strhash_contains(s2, l->data)) {
                return 0;
            }
            l = l->next;
        }
    }
    // Inclusion réciproque
    for (size_t i = 0; i<s2->capacity; i++) {
        s_node* l = s2->entries[i];
        while (l != NULL) {
            if (!strhash_contains(s1, l->data)) {
                return 0;
            }
            l = l->next;
        }
    }
    return 1;
}

strhash* strhash_from_string(const char* string, size_t capacity) {
    strhash* res = strhash_create(capacity);
    char* temp = malloc(strlen(string)+1); // On stocke temporairement string dans temp pour éviter de modifier string
    strcpy(temp, string);
    char* token = strtok(temp, "|");
    while (token != NULL) { // Tant qu'on n'est pas arrivé au bout de la chaîne.
        strhash_add(res, token);
        token = strtok(NULL, "|"); // On passe au token suivant (élément suivant).
    }
    free(temp);
    return res;
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

void strhash_display(strhash *hashtable) {
    printf("------------------\n");
    printf("Table de hachage (%d éléments)\n", (int)hashtable->size);
    for (size_t i = 0; i<hashtable->capacity; i++) {
        printf("%000d : %s\n", i, list_to_string(hashtable->entries[i], node_string_to_string));
    }
    printf("------------------\n");
}
