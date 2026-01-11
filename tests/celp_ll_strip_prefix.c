#define CELP_IMPLEMENTATION
#define CELP_STRIP_PREFIX
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

    LLN_int_t curr = *n.head->next;
    for (size_t i = 0; i < n.count; i++) {
        log(CELP_LOG_LEVEL_INFO, "[%zu] %i", i, curr.data);
        curr = *curr.next;
    }
    return 0;
}
