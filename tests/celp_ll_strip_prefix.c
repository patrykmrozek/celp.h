#include <complex.h>
#define CELP_STRIP_PREFIX
#define CELP_IMPLEMENTATION
#include "../celp.h"
#include <stdio.h>

LL(int);

int main() {
    log(CELP_LOG_LEVEL_INFO, "LL!");

    LL_int_t n;
    ll_init(&n);
    ll_add(&n, 5);
    ll_add(&n, 6);
    ll_add(&n, 7);
    ll_add_first(&n, 9);
    ll_add_last(&n, 11);
    ll_info(&n);

    log(CELP_LOG_LEVEL_DEBUG, "ll print int: ");
    ll_print_int(&n);

    ll_info(&n);
    log(CELP_LOG_LEVEL_DEBUG, "Before removing first: %i", n.head->next->data);
    ll_remove_first(&n);
    log(CELP_LOG_LEVEL_DEBUG, "After removing first: %i", n.head->next->data);
    ll_info(&n);

    log(CELP_LOG_LEVEL_DEBUG, "Before removing last: %i", n.tail->prev->data);
    ll_remove_last(&n);
    log(CELP_LOG_LEVEL_DEBUG, "After removing last: %i", n.tail->prev->data);
    ll_info(&n);

    ll_print_int(&n);

    ll_remove(&n, 1);
    ll_info(&n);

    ll_free(&n);
    ll_info(&n);
    return 0;
}
