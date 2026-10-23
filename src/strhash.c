#include "strhash.h"

int _hash_key(char* str) {
    int key = 0;
    while (*str != '\0') {
        key = key * 2 + *str;
        str++;
    }
    return key;
}
