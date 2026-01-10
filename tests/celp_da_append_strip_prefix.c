#define CELP_STRIP_PREFIX
#include "../celp.h"

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

int main(void) {

    DA_ARRAY(Nums, int)
    NumsArray_t n = {};
    da_init(&n);

    for (uint32_t i = 0; i < 10; i++) {
        da_append(&n, i);
    }

    for (size_t i = 0; i < n.count; i++) {
        printf("%i\n", n.items[i]);
    }

    printf("Capacity (before realloc): %lu", n.capacity);


    for (uint32_t i = 0; i < 300; i++) {
        da_append(&n, i);
    }

    for (size_t i = 0; i < n.count; i++) {
        printf("%i\n", n.items[i]);
    }

    printf("Capacity (after realloc): %lu", n.capacity);

    da_free(&n);
    return 0;

}
