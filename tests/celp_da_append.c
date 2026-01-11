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

    printf("last item in da: %i\n", celp_da_last(&n));
    int popped_val = celp_da_pop(&n);
    printf("popped value: %i\n", popped_val);
    printf("last item in da: %i\n", celp_da_last(&n));

    printf("(before removing %i)\n", n.items[n.count-2]);
    for (size_t i = n.count-5; i < n.count; i++) {
        printf("%zu) %i\n", i, n.items[i]);
    }
    celp_da_remove(&n, n.count-2);
    printf("(after removing)\n");
    for (size_t i = n.count-5; i < n.count; i++) {
        printf("%zu) %i\n", i, n.items[i]);
    }


    celp_da_free(&n);
    return 0;

}
