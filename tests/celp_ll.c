#define CELP_IMPLEMENTATION
#include "../celp.h"
#include <stdio.h>

CELP_LL(int);

int main() {
    celp_log(CELP_LOG_LEVEL_INFO, "LL!");

    LL_int_t n;
    celp_ll_init(&n);
    celp_ll_add(&n, 5);
    celp_ll_info(&n);


    return 0;
}
