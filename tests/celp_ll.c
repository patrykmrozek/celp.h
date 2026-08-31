#define CELP_TEST
#include "../celp.h"

celp_ll(int);
celp_ll_t(int) n;

CELP_TEST_SETUP(ll)
{
    celp_ll_init(&n);
}

CELP_TEST_TEARDOWN(ll)
{
    celp_ll_free(&n);
}

CELP_TESTCASE(ll_get_first_empty)
{
    int safe = -1;
    int check = celp_ll_get_first(&n, safe);
    CELP_EXPECT_EQ(check, safe);
}

CELP_TESTCASE(ll_get_last_empty)
{
    int safe = -1;
    int check = celp_ll_get_last(&n, safe);
    CELP_EXPECT_EQ(check, safe);
}
CELP_TESTCASE(ll_get_first_node_empty)
{
    lln_int_t *safe = CELP_LLN_SAFE(int, -1);
    lln_int_t *check = celp_ll_get_first_node(&n, safe);
    CELP_EXPECT_EQ(check, safe);
}

CELP_TESTCASE(ll_remove_first_empty)
{
    int safe = -1;
    int check = celp_ll_remove_first(&n, safe);
    CELP_EXPECT_EQ(check, safe);
}

CELP_TESTCASE(ll_remove_last_empty)
{
    int safe = -1;
    int check = celp_ll_remove_last(&n, safe);
    CELP_EXPECT_EQ(check, safe);
}

CELP_TESTCASE(ll_add)
{
    celp_ll_add(&n, 5);
    lln_int_t *curr = n.head->next;
    CELP_EXPECT_EQ(curr->data, 5);

    celp_ll_add(&n, 6);
    curr = curr->next;
    CELP_EXPECT_EQ(curr->data, 6);

    celp_ll_add(&n, 7);
    curr = curr->next;
    CELP_EXPECT_EQ(curr->data, 7);

    celp_ll_add_first(&n, 9);
    CELP_EXPECT_EQ(n.head->next->data, 9);

    celp_ll_add_last(&n, 11);
    CELP_EXPECT_EQ(n.tail->prev->data, 11);

    for (int i = 100; i < 150; i++) {
        celp_ll_add(&n, i);
    }
}

CELP_TESTCASE(ll_foreach)
{
    int count = 0;
    celp_ll_foreach(&n, node) {
        CELP_EXPECT(node);
        count++;
    }
    CELP_EXPECT_EQ(n.count, count);
}

CELP_TESTCASE(ll_get_first)
{
    int safe = -1;
    int first = celp_ll_get_first(&n, safe);
    CELP_EXPECT_NEQ(first,  safe);
    int first_check = n.head->next->data;
    CELP_EXPECT_EQ(first, first_check);
}

CELP_TESTCASE(ll_get_first_node)
{
    lln_int_t *safe = CELP_LLN_SAFE(int, -1);
    lln_int_t *first = celp_ll_get_first_node(&n, safe);
    CELP_EXPECT_NEQ(first,  safe);
    lln_int_t *first_check = n.head->next;
    CELP_EXPECT_EQ(first, first_check);
}

CELP_TESTCASE(ll_get_last)
{
    int safe = -1;
    int last = celp_ll_get_last(&n, safe);
    CELP_EXPECT_NEQ(last, safe);
    int last_check = n.tail->prev->data;
    CELP_EXPECT_EQ(last, last_check);
}

CELP_TESTCASE(ll_get_last_node)
{
    lln_int_t *safe = CELP_LLN_SAFE(int, -1);
    lln_int_t *last = celp_ll_get_last_node(&n, safe);
    CELP_EXPECT_NEQ(last, safe);
    lln_int_t *last_check = n.tail->prev;
    CELP_EXPECT_EQ(last, last_check);
}

CELP_TESTCASE(ll_add_after)
{
    lln_int_t *check = n.head->next->next;
    lln_int_t *check_after = check->next;
    int check_val = 9;

    celp_ll_add_after(&n, check_val, check);

    CELP_EXPECT_NEQ(check_after->data, check_val);
    CELP_EXPECT_EQ(check->next->data, check_val);
    CELP_EXPECT_EQ(check_after->prev->data, check_val);
}

CELP_TESTCASE(ll_remove_first)
{
    int safe = -1;
    int check = celp_ll_get_first(&n, safe);
    CELP_EXPECT_NEQ(check, safe);

    int count = n.count;
    int ret = celp_ll_remove_first(&n, safe);
    CELP_EXPECT_NEQ(ret, safe);
    CELP_EXPECT_EQ(ret, check);
    CELP_EXPECT_EQ(n.count, count-1);

    int first = celp_ll_get_first(&n, safe); 
    CELP_EXPECT_NEQ(check, safe);
    CELP_EXPECT_NEQ(first, check);
}

CELP_TESTCASE(ll_remove_last)
{
    int safe = -1;
    int check = celp_ll_get_last(&n, safe);
    int count = n.count;
    int ret = celp_ll_remove_last(&n, safe);

    CELP_EXPECT_NEQ(ret, safe);
    CELP_EXPECT_EQ(ret, check);
    CELP_EXPECT_EQ(n.count, count-1);
    CELP_EXPECT_NEQ(celp_ll_get_last(&n, safe), check);
}

CELP_TESTCASE(ll_get_at_index)
{
    int idx = 1;
    int safe = -1;
    int data = celp_ll_get_at_index(&n, idx, safe);
    CELP_EXPECT_NEQ(data, safe);
}

CELP_TESTCASE(ll_get_at_index_invalid)
{
    int idx = -1;
    int safe = -1;
    int data = celp_ll_get_at_index(&n, idx, safe);
    CELP_EXPECT_EQ(data, safe);
}

CELP_TESTCASE(ll_remove_at_index)
{
    int idx = 20;
    int count = n.count;
    int safe = -1;

    int data = celp_ll_get_at_index(&n, idx, safe);
    int ret = celp_ll_remove_at_index(&n, idx, safe);
    CELP_EXPECT_NEQ(ret, safe);
    CELP_EXPECT_EQ(data, ret);
    CELP_EXPECT_EQ(n.count, count-1);

    int data_after = celp_ll_get_at_index(&n, idx, safe);
    CELP_EXPECT_NEQ(data, data_after);
}

CELP_TESTCASE(ll_remove_at_index_invalid)
{
    int idx = -1;
    int safe = -1;
    int ret = celp_ll_remove_at_index(&n, idx, safe);
    CELP_EXPECT_EQ(ret, safe);
}

CELP_TESTCASE(ll_remove_node)
{
    lln_int_t *node =
        celp_ll_get_at_index_node(&n, 10, CELP_LLN_SAFE(int, -1));
    lln_int_t *node_after = node->next;
    int count = n.count;
    int ret = celp_ll_remove_node(&n, node, CELP_LLN_SAFE(int, -1));
    lln_int_t *at_index =
        celp_ll_get_at_index_node(&n, 10, CELP_LLN_SAFE(int, -1));

    CELP_EXPECT_EQ(n.count, count-1);
    CELP_EXPECT_NEQ(node, at_index);
    CELP_EXPECT_NEQ(node_after->prev->data, ret);
    CELP_EXPECT_NEQ(ret, -1);
}

CELP_TESTCASE(ll_remove_node_invalid)
{
    lln_int_t invalid = {0, NULL, NULL};
    lln_int_t *safe = CELP_LLN_SAFE(int, -1);
    int ret = celp_ll_remove_node(&n, &invalid, safe);
    CELP_EXPECT_EQ(ret, safe->data);
}

CELP_TEST_SUITE_START(linked_list);
{
    CELP_TEST_SUITE_ADD_SETUP(linked_list, ll);
    CELP_TEST_SUITE_ADD_TEARDOWN(linked_list, ll);

    CELP_TEST_SUITE_ADD_TEST(linked_list, ll_get_first_empty);
    CELP_TEST_SUITE_ADD_TEST(linked_list, ll_get_last_empty);
    CELP_TEST_SUITE_ADD_TEST(linked_list, ll_get_first_node_empty);
    CELP_TEST_SUITE_ADD_TEST(linked_list, ll_remove_first_empty);
    CELP_TEST_SUITE_ADD_TEST(linked_list, ll_remove_last_empty);
    CELP_TEST_SUITE_ADD_TEST(linked_list, ll_add);
    CELP_TEST_SUITE_ADD_TEST(linked_list, ll_foreach);
    CELP_TEST_SUITE_ADD_TEST(linked_list, ll_get_first);
    CELP_TEST_SUITE_ADD_TEST(linked_list, ll_get_first_node);
    CELP_TEST_SUITE_ADD_TEST(linked_list, ll_get_last);
    CELP_TEST_SUITE_ADD_TEST(linked_list, ll_get_last_node);
    CELP_TEST_SUITE_ADD_TEST(linked_list, ll_add_after);
    CELP_TEST_SUITE_ADD_TEST(linked_list, ll_remove_first);
    CELP_TEST_SUITE_ADD_TEST(linked_list, ll_remove_last);
    CELP_TEST_SUITE_ADD_TEST(linked_list, ll_get_at_index);
    CELP_TEST_SUITE_ADD_TEST(linked_list, ll_get_at_index_invalid);
    CELP_TEST_SUITE_ADD_TEST(linked_list, ll_remove_at_index);
    CELP_TEST_SUITE_ADD_TEST(linked_list, ll_remove_at_index_invalid);
    CELP_TEST_SUITE_ADD_TEST(linked_list, ll_remove_node);
    CELP_TEST_SUITE_ADD_TEST(linked_list, ll_remove_node_invalid);
}
CELP_TEST_SUITE_END(linked_list);

void test_celp_ll()
{
    CELP_TEST_SUITE_RUN(linked_list);
    CELP_TEST_SUITE_REPORT(linked_list);
    CELP_TEST_SUITE_DESTROY(linked_list);
}
