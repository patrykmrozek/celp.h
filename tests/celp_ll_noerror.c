#define CELP_TEST
#include "../celp.h"

celp_ll(int);
celp_ll_t(int) n;

CELP_TEST_SETUP(llne)
{
    celp_ll_init(&n);
}

CELP_TEST_TEARDOWN(llne)
{
    celp_ll_free(&n);
}

CELP_TESTCASE(llne_get_first_empty)
{
    celp_last_err_clear();
    (void)celp_ll_get_first(&n);
    CELP_EXPECT_NEQ(celp_last_err(), CELP_ERR_OK);
}

CELP_TESTCASE(llne_get_last_empty)
{
    celp_last_err_clear();
    (void)celp_ll_get_last(&n);
    CELP_EXPECT_NEQ(celp_last_err(), CELP_ERR_OK);
}
CELP_TESTCASE(llne_get_first_node_empty)
{
    celp_last_err_clear();
    (void)celp_ll_get_first_node(&n);
    CELP_EXPECT_NEQ(celp_last_err(), CELP_ERR_OK);
}

CELP_TESTCASE(llne_remove_first_empty)
{
    celp_last_err_clear();
    (void)celp_ll_remove_first(&n);
    CELP_EXPECT_NEQ(celp_last_err(), CELP_ERR_OK);
}

CELP_TESTCASE(llne_remove_last_empty)
{
    celp_last_err_clear();
    (void)celp_ll_remove_last(&n);
    CELP_EXPECT_NEQ(celp_last_err(), CELP_ERR_OK);
}

CELP_TESTCASE(llne_add)
{
    celp_last_err_clear();
    celp_ll_add(&n, 5);
    CELP_EXPECT_EQ(celp_last_err(), CELP_ERR_OK);
    lln_int_t *curr = n.head->next;
    CELP_EXPECT_EQ(curr->data, 5);

    celp_last_err_clear();
    celp_ll_add(&n, 6);
    CELP_EXPECT_EQ(celp_last_err(), CELP_ERR_OK);
    curr = curr->next;
    CELP_EXPECT_EQ(curr->data, 6);

    celp_last_err_clear();
    celp_ll_add(&n, 7);
    CELP_EXPECT_EQ(celp_last_err(), CELP_ERR_OK);
    curr = curr->next;
    CELP_EXPECT_EQ(curr->data, 7);

    celp_last_err_clear();
    celp_ll_add_first(&n, 9);
    CELP_EXPECT_EQ(celp_last_err(), CELP_ERR_OK);
    CELP_EXPECT_EQ(n.head->next->data, 9);

    celp_last_err_clear();
    celp_ll_add_last(&n, 11);
    CELP_EXPECT_EQ(celp_last_err(), CELP_ERR_OK);
    CELP_EXPECT_EQ(n.tail->prev->data, 11);

    for (int i = 100; i < 150; i++) {
        celp_last_err_clear();
        celp_ll_add(&n, i);
        CELP_EXPECT_EQ(celp_last_err(), CELP_ERR_OK);
    }
}

CELP_TESTCASE(llne_foreach)
{
    int count = 0;
    celp_ll_foreach(&n, node) {
        CELP_EXPECT(node);
        count++;
    }
    CELP_EXPECT_EQ(n.count, count);
}

CELP_TESTCASE(llne_get_first)
{
    celp_last_err_clear();
    int first = celp_ll_get_first(&n);
    CELP_EXPECT_EQ(celp_last_err(), CELP_ERR_OK);
    int first_check = n.head->next->data;
    CELP_EXPECT_EQ(first, first_check);
}

CELP_TESTCASE(llne_get_first_node)
{
    celp_last_err_clear();
    lln_int_t *first = celp_ll_get_first_node(&n);
    CELP_EXPECT_EQ(celp_last_err(), CELP_ERR_OK);
    lln_int_t *first_check = n.head->next;
    CELP_EXPECT_EQ(first, first_check);
}

CELP_TESTCASE(llne_get_last)
{
    celp_last_err_clear();
    int last = celp_ll_get_last(&n);
    CELP_EXPECT_EQ(celp_last_err(), CELP_ERR_OK);
    int last_check = n.tail->prev->data;
    CELP_EXPECT_EQ(last, last_check);
}

CELP_TESTCASE(llne_get_last_node)
{
    celp_last_err_clear();
    lln_int_t *last = celp_ll_get_last_node(&n);
    CELP_EXPECT_EQ(celp_last_err(), CELP_ERR_OK);
    lln_int_t *last_check = n.tail->prev;
    CELP_EXPECT_EQ(last, last_check);
}

CELP_TESTCASE(llne_add_after)
{
    celp_last_err_clear();
    lln_int_t *check = n.head->next->next;
    lln_int_t *check_after = check->next;
    int check_val = 9;

    celp_ll_add_after(&n, check_val, check);

    CELP_EXPECT_EQ(celp_last_err(), CELP_ERR_OK);
    CELP_EXPECT_NEQ(check_after->data, check_val);
    CELP_EXPECT_EQ(check->next->data, check_val);
    CELP_EXPECT_EQ(check_after->prev->data, check_val);
}

CELP_TESTCASE(llne_remove_first)
{
    celp_last_err_clear();
    int check = celp_ll_get_first(&n);
    CELP_EXPECT_EQ(celp_last_err(), CELP_ERR_OK);

    int count = n.count;
    int ret = celp_ll_remove_first(&n);
    CELP_EXPECT_EQ(celp_last_err(), CELP_ERR_OK);
    CELP_EXPECT_EQ(ret, check);
    CELP_EXPECT_EQ(n.count, count-1);

    int first = celp_ll_get_first(&n); 
    CELP_EXPECT_EQ(celp_last_err(), CELP_ERR_OK);
    CELP_EXPECT_NEQ(first, check);
}

CELP_TESTCASE(llne_remove_last)
{
    celp_last_err_clear();
    int check = celp_ll_get_last(&n);
    CELP_EXPECT_EQ(celp_last_err(), CELP_ERR_OK);
    int count = n.count;
    int ret = celp_ll_remove_last(&n);
    CELP_EXPECT_EQ(celp_last_err(), CELP_ERR_OK);

    CELP_EXPECT_EQ(ret, check);
    CELP_EXPECT_EQ(n.count, count-1);
    CELP_EXPECT_NEQ(celp_ll_get_last(&n), check);
}

CELP_TESTCASE(llne_get_at_index)
{
    celp_last_err_clear();
    int idx = 1;
    int data = celp_ll_get_at_index(&n, idx);
    CELP_EXPECT_EQ(celp_last_err(), CELP_ERR_OK);
}

CELP_TESTCASE(llne_get_at_index_invalid)
{
    celp_last_err_clear();
    int idx = -1;
    int data = celp_ll_get_at_index(&n, idx);
    CELP_EXPECT_NEQ(celp_last_err(), CELP_ERR_OK);
}

CELP_TESTCASE(llne_remove_at_index)
{
    celp_last_err_clear();
    int idx = 20;
    int count = n.count;

    int data = celp_ll_get_at_index(&n, idx);
    CELP_EXPECT_EQ(celp_last_err(), CELP_ERR_OK);

    celp_last_err_clear();
    int ret = celp_ll_remove_at_index(&n, idx);
    CELP_EXPECT_EQ(celp_last_err(), CELP_ERR_OK);
    CELP_EXPECT_EQ(data, ret);
    CELP_EXPECT_EQ(n.count, count-1);

    celp_last_err_clear();
    int data_after = celp_ll_get_at_index(&n, idx);
    CELP_EXPECT_EQ(celp_last_err(), CELP_ERR_OK);
    CELP_EXPECT_NEQ(data, data_after);
}

CELP_TESTCASE(llne_remove_at_index_invalid)
{
    celp_last_err_clear();
    int idx = -1;
    int ret = celp_ll_remove_at_index(&n, idx);
    CELP_EXPECT_NEQ(celp_last_err(), CELP_ERR_OK);
}

CELP_TESTCASE(llne_remove_node)
{
    celp_last_err_clear();
    lln_int_t *node = celp_ll_get_at_index_node(&n, 10);
    CELP_EXPECT_EQ(celp_last_err(), CELP_ERR_OK);

    lln_int_t *node_after = node->next;
    int count = n.count;

    celp_last_err_clear();
    celp_ll_remove_node(&n, node);
    CELP_EXPECT_EQ(celp_last_err(), CELP_ERR_OK);
    celp_last_err_clear();
    lln_int_t *at_index = celp_ll_get_at_index_node(&n, 10);
    CELP_EXPECT_EQ(celp_last_err(), CELP_ERR_OK);

    CELP_EXPECT_EQ(n.count, count-1);
    CELP_EXPECT_NEQ(node, at_index);
}

CELP_TESTCASE(llne_remove_node_invalid)
{
    celp_last_err_clear();
    lln_int_t invalid = {0, NULL, NULL};
    celp_ll_remove_node(&n, &invalid);
    CELP_EXPECT_NEQ(celp_last_err(), CELP_ERR_OK);
} 

CELP_TEST_SUITE_START(linked_list_noerr);
{
    CELP_TEST_SUITE_ADD_SETUP(linked_list_noerr, llne);
    CELP_TEST_SUITE_ADD_TEARDOWN(linked_list_noerr, llne);

    CELP_TEST_SUITE_ADD_TEST(linked_list_noerr, llne_get_first_empty);
    CELP_TEST_SUITE_ADD_TEST(linked_list_noerr, llne_get_last_empty);
    CELP_TEST_SUITE_ADD_TEST(linked_list_noerr, llne_get_first_node_empty);
    CELP_TEST_SUITE_ADD_TEST(linked_list_noerr, llne_remove_first_empty);
    CELP_TEST_SUITE_ADD_TEST(linked_list_noerr, llne_remove_last_empty);
    CELP_TEST_SUITE_ADD_TEST(linked_list_noerr, llne_add);
    CELP_TEST_SUITE_ADD_TEST(linked_list_noerr, llne_foreach);
    CELP_TEST_SUITE_ADD_TEST(linked_list_noerr, llne_get_first);
    CELP_TEST_SUITE_ADD_TEST(linked_list_noerr, llne_get_first_node);
    CELP_TEST_SUITE_ADD_TEST(linked_list_noerr, llne_get_last);
    CELP_TEST_SUITE_ADD_TEST(linked_list_noerr, llne_get_last_node);
    CELP_TEST_SUITE_ADD_TEST(linked_list_noerr, llne_add_after);
    CELP_TEST_SUITE_ADD_TEST(linked_list_noerr, llne_remove_first);
    CELP_TEST_SUITE_ADD_TEST(linked_list_noerr, llne_remove_last);
    CELP_TEST_SUITE_ADD_TEST(linked_list_noerr, llne_get_at_index);
    CELP_TEST_SUITE_ADD_TEST(linked_list_noerr, llne_get_at_index_invalid);
    CELP_TEST_SUITE_ADD_TEST(linked_list_noerr, llne_remove_at_index);
    CELP_TEST_SUITE_ADD_TEST(linked_list_noerr, llne_remove_at_index_invalid);
    CELP_TEST_SUITE_ADD_TEST(linked_list_noerr, llne_remove_node);
    CELP_TEST_SUITE_ADD_TEST(linked_list_noerr, llne_remove_node_invalid);
}
CELP_TEST_SUITE_END(linked_list_noerr);

void test_celp_ll_noerror()
{
    CELP_TEST_SUITE_RUN(linked_list_noerr);
    CELP_TEST_SUITE_REPORT(linked_list_noerr);
    CELP_TEST_SUITE_DESTROY(linked_list_noerr);
}
