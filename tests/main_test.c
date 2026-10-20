#include "unity.h"

extern void list_test();

void setUp() {
}

void tearDown() {
}

int main() {
    UNITY_BEGIN();
    list_test();
    return UNITY_END();
}
