#define CELP_IMPLEMENTATION
#include "../celp.h"

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

int main(void) {

    CELP_DA_ARRAY(Nums, int);
    NumsArray_t n = {};
    celp_da_init(&n);

    for (uint32_t i = 0; i < 10; i++) {
        celp_da_append(&n, i);
    }
    for (size_t i = 0; i < n.count; i++) {
        printf("%i\n", n.items[i]);
    }
    celp_da_info(&n);


    for (uint32_t i = 0; i < 300; i++) {
        celp_da_append(&n, i);
    }
    for (size_t i = 0; i < n.count; i++) {
        printf("%i\n", n.items[i]);
    }
    celp_da_info(&n);


    celp_da_free(&n);
    return 0;

}
