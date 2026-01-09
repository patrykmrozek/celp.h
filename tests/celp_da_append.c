#include "../celp.h"
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

int main() {
    printf("Hello\n");

    typedef struct Numbers {
        uint32_t* items;
        size_t count;
        size_t capacity;
    } Numbers;

    Numbers xs = {0};
    for (uint32_t i = 0; i < 10; i++) {
        celp_da_append(xs, i);
    }

    for (uint32_t j = 0; j < xs.count; j++) {
        printf("%i\n", xs.items[j]);
    }
    printf("CAPACITY: %lu\n", xs.capacity);

    for (uint32_t i = 0; i < 300; i++) {
        celp_da_append(xs, i);
    }

    for (uint32_t j = 0; j < xs.count; j++) {
        printf("%i\n", xs.items[j]);
    }
    printf("CAPACITY (after realloc): %lu\n", xs.capacity);

    return 0;
}
