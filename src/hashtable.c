#include "hashtable.h"

int hash_key(char* str) {
    int key = 0;
    while (*str != '\0') {
        key = key * 2 + *str;
        str++;
    }
    return key;
}
