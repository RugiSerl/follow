#include "unity.h"
#include "list.h"
#include <stdio.h>

extern void list_test();
extern void strhash_test();

void setUp() {
}

void tearDown() {
}


int main() {
    UNITY_BEGIN();
    list_test();
    strhash_test();
    return UNITY_END();
}
