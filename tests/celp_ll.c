#include "../celp.h"
#include <stdio.h>

void celp_ll() {
    CELP_INFO("LL!");

    CELP_LL(int);
    CELP_LL_T(int) n;
    celp_ll_init(&n);
    celp_ll_add(&n, 5);
    celp_ll_add(&n, 6);
    celp_ll_add(&n, 7);
    celp_ll_add_first(&n, 9);
    celp_ll_add_last(&n, 11);
    celp_ll_info(&n);

    CELP_TRACE(1, "ll print foreach: "); 

    //no longer segfaults but need to handle error..
    //CELP_TRACE( "ll print foreach: "); 

    size_t count = 0;
    CELP_INFO("celp_ll_foreach");
    celp_ll_foreach(&n, i) {
        CELP_DEBUG(1,
                "[%zu] %i", 
                count, i->data);
        count++;
    }

    CELP_LLN_T(int) *until = n.tail->prev->prev;
    count = 0;
    CELP_INFO("celp_ll_foreach_until_node");
    celp_ll_foreach_until_node(&n, iter, until) {
        CELP_DEBUG(1, 
                "[%zu] %i", 
                count, iter->data);
        count++;
    }

    celp_ll_info(&n);
    CELP_DEBUG(1, "Before removing first: %i", n.head->next->data);
    celp_ll_remove_first(&n);
    CELP_DEBUG(1, "After removing first: %i", n.head->next->data);
    celp_ll_info(&n);

    CELP_DEBUG(1, "Before removing last: %i", n.tail->prev->data);
    celp_ll_remove_last(&n);
    CELP_DEBUG(1, "After removing last: %i", n.tail->prev->data);
    celp_ll_info(&n);

    celp_ll_print_int(&n);

    int remove_idx = 1;
    CELP_DEBUG(1, "Removing idx[%zu]", remove_idx);
    int removed = celp_ll_remove_at_index(&n, remove_idx);
    CELP_DEBUG(1, "Removed idx[%zu]: %i", remove_idx, removed);
    celp_ll_info(&n);

    CELP_LLN_T(int)* node = n.head->next;
    CELP_DEBUG(1, "Removing using node");
    celp_ll_remove_node(&n, node);
    celp_ll_print_int(&n);
    celp_ll_info(&n);

    celp_ll_free(&n);
    celp_ll_info(&n);
}
