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
        celp_log(CELP_LOG_LEVEL_DEBUG, "%i\n", n.items[i]);
    }
    celp_da_info(&n);


    for (uint32_t i = 0; i < 300; i++) {
        celp_da_append(&n, i);
    }
    for (size_t i = 0; i < n.count; i++) {
        celp_log(CELP_LOG_LEVEL_DEBUG, "%i\n", n.items[i]);
    }
    celp_da_info(&n);

    celp_log(CELP_LOG_LEVEL_DEBUG, "last item in da: %i\n", celp_da_last(&n));
    int popped_val = celp_da_pop(&n);
    celp_log(CELP_LOG_LEVEL_DEBUG, "popped value: %i\n", popped_val);
    celp_log(CELP_LOG_LEVEL_DEBUG, "last item in da: %i\n", celp_da_last(&n));

    celp_log(CELP_LOG_LEVEL_DEBUG, "(before removing %i)\n", n.items[n.count-2]);
    for (size_t i = n.count-5; i < n.count; i++) {
        celp_log(CELP_LOG_LEVEL_DEBUG, "%zu) %i\n", i, n.items[i]);
    }
    int removed = celp_da_remove(&n, n.count-2);
    celp_log(CELP_LOG_LEVEL_DEBUG, "(after removing %i)\n", removed);
    for (size_t i = n.count-5; i < n.count; i++) {
        celp_log(CELP_LOG_LEVEL_DEBUG, "%zu) %i\n", i, n.items[i]);
    }

    celp_da_foreach(&n, x) {
        //size_t idx = x - n.items;
        //celp_log(CELP_LOG_LEVEL_DEBUG, "%zu) %i\n", idx, *x);
        celp_log(CELP_LOG_LEVEL_INFO, "%i", *x);
    }


    celp_da_free(&n);
}
