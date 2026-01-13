#include <complex.h>
#define CELP_IMPLEMENTATION
#include "../celp.h"
#include <stdio.h>

CELP_LL(int);

int main() {
    celp_log(CELP_LOG_LEVEL_INFO, "LL!");

    LL_int_t n;
    celp_ll_init(&n);
    celp_ll_add(&n, 5);
    celp_ll_add(&n, 6);
    celp_ll_add(&n, 7);
    celp_ll_add_first(&n, 9);
    celp_ll_add_last(&n, 11);
    celp_ll_info(&n);

    celp_log(CELP_LOG_LEVEL_DEBUG, "ll print int: ");
    celp_ll_print_int(&n);

    celp_ll_info(&n);
    celp_log(CELP_LOG_LEVEL_DEBUG, "Before removing first: %i", n.head->next->data);
    celp_ll_remove_first(&n);
    celp_log(CELP_LOG_LEVEL_DEBUG, "After removing first: %i", n.head->next->data);
    celp_ll_info(&n);

    celp_log(CELP_LOG_LEVEL_DEBUG, "Before removing last: %i", n.tail->prev->data);
    celp_ll_remove_last(&n);
    celp_log(CELP_LOG_LEVEL_DEBUG, "After removing last: %i", n.tail->prev->data);
    celp_ll_info(&n);

    celp_ll_print_int(&n);

    celp_ll_remove(&n, 1);
    celp_ll_info(&n);

    celp_ll_free(&n);
    celp_ll_info(&n);
    return 0;
}
