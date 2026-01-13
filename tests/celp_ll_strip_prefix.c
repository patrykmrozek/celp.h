#include <complex.h>
#include <stddef.h>
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
    int removed_first = ll_remove_first(&n);
    log(CELP_LOG_LEVEL_DEBUG, "Removed first: %i", removed_first);
    log(CELP_LOG_LEVEL_DEBUG, "After removing first: %i", n.head->next->data);
    ll_info(&n);

    log(CELP_LOG_LEVEL_DEBUG, "Before removing last: %i", n.tail->prev->data);
    int removed_last = ll_remove_last(&n);
    log(CELP_LOG_LEVEL_DEBUG, "Removed last: %i", removed_last);
    log(CELP_LOG_LEVEL_DEBUG, "After removing last: %i", n.tail->prev->data);
    ll_info(&n);

    ll_print_int(&n);

    int remove_idx = 1;
    log(CELP_LOG_LEVEL_DEBUG, "Removing idx[%zu]", remove_idx);
    int removed = ll_remove(&n, remove_idx);
    log(CELP_LOG_LEVEL_DEBUG, "Removed idx[%zu]: %i", remove_idx, removed);
    ll_info(&n);

    ll_free(&n);
    ll_info(&n);
    return 0;
}
