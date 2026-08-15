#include "../celp.h"

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

void celp_da() {
 
    CELP_DA(int)
    CELP_DA_T(int) n = {};
    celp_da_init(&n);

    for (uint32_t i = 0; i < 10; i++) {
        celp_da_append(&n, i);
    }
    for (size_t i = 0; i < n.count; i++) {
        CELP_DEBUG(1, "%i\n", n.items[i]);
    }
    celp_da_info(&n);


    for (uint32_t i = 0; i < 300; i++) {
        celp_da_append(&n, i);
    }
    for (size_t i = 0; i < n.count; i++) {
        CELP_DEBUG(1, "%i\n", n.items[i]);
    }
    celp_da_info(&n);

    CELP_DEBUG(1, "last item in da: %i\n", celp_da_last(&n));
    int popped_val = celp_da_pop(&n);
    CELP_DEBUG(1, "popped value: %i\n", popped_val);
    CELP_DEBUG(1, "last item in da: %i\n", celp_da_last(&n));

    CELP_DEBUG(1, "(before removing %i)\n", n.items[n.count-2]);
    for (size_t i = n.count-5; i < n.count; i++) {
        CELP_DEBUG(1, "%zu) %i\n", i, n.items[i]);
    }
    int removed = celp_da_remove(&n, n.count-2);
    CELP_DEBUG(1, "(after removing %i)\n", removed);
    for (size_t i = n.count-5; i < n.count; i++) {
        CELP_DEBUG(1, "%zu) %i\n", i, n.items[i]);
    }

    celp_da_foreach(&n, x) {
        //size_t idx = x - n.items;
        //CELP_DEBUG(1, "%zu) %i\n", idx, *x);
        CELP_INFO("%i", *x);
    }


    celp_da_free(&n);
}
