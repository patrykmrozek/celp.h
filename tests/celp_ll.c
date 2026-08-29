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

CELP_TESTCASE(ll_add)
{
    celp_ll_add(&n, 5);
    celp_lln_t(int) *curr = n.head->next;
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
    int first = celp_ll_get_first(&n);
    int first_check = n.head->next->data;
    CELP_EXPECT_EQ(first, first_check);
}

CELP_TESTCASE(ll_get_last)
{
    int last = celp_ll_get_last(&n);
    int last_check = n.tail->prev->data;
    CELP_EXPECT_EQ(last, last_check);
}

CELP_TESTCASE(ll_add_after)
{
    celp_lln_t(int) *check = n.head->next->next;
    celp_lln_t(int) *check_after = check->next;
    int check_val = 9;

    celp_ll_add_after(&n, check_val, check);

    CELP_EXPECT_NEQ(check_after->data, check_val);
    CELP_EXPECT_EQ(check->next->data, check_val);
    CELP_EXPECT_EQ(check_after->prev->data, check_val);
}

CELP_TESTCASE(ll_remove_first)
{
    int check = celp_ll_get_first(&n);
    int count = n.count;
    int ret = celp_ll_remove_first(&n);

    CELP_EXPECT_EQ(ret, check);
    CELP_EXPECT_EQ(n.count, count-1);
    CELP_EXPECT_NEQ(celp_ll_get_first(&n), check);
}

CELP_TESTCASE(ll_remove_last)
{
    int check = celp_ll_get_last(&n);
    int count = n.count;
    int ret = celp_ll_remove_last(&n);

    CELP_EXPECT_EQ(ret, check);
    CELP_EXPECT_EQ(n.count, count-1);
    CELP_EXPECT_NEQ(celp_ll_get_last(&n), check);
}

CELP_TESTCASE(ll_remove_at_index)
{
    int idx = 20;
    int count = n.count;
    celp_lln_t(int) *node = celp_ll_get_at_index(&n, idx);
    int ret = celp_ll_remove_at_index(&n, idx);

    CELP_EXPECT_NEQ(node->data, ret);
    CELP_EXPECT_EQ(n.count, count-1);
}

CELP_TESTCASE(ll_remove_node)
{
    celp_lln_t(int) *node = celp_ll_get_at_index(&n, 10);
    celp_lln_t(int) *node_after = node->next;
    int count = n.count;
    celp_lln_t(int) *ret = celp_ll_remove_node(&n, node);

    CELP_EXPECT_EQ(n.count, count-1);
    CELP_EXPECT_NEQ(node, celp_ll_get_at_index(&n, 10));
    CELP_EXPECT_NEQ(node_after->prev, ret);
}

CELP_TEST_SUITE_START(linked_list);
{
    CELP_TEST_SUITE_ADD_SETUP(ll);
    CELP_TEST_SUITE_ADD_TEARDOWN(ll);

    CELP_TEST_SUITE_ADD_TEST(ll_add);
    CELP_TEST_SUITE_ADD_TEST(ll_foreach);
    CELP_TEST_SUITE_ADD_TEST(ll_get_first);
    CELP_TEST_SUITE_ADD_TEST(ll_get_last);
    CELP_TEST_SUITE_ADD_TEST(ll_add_after);
    CELP_TEST_SUITE_ADD_TEST(ll_remove_first);
    CELP_TEST_SUITE_ADD_TEST(ll_remove_last);
    CELP_TEST_SUITE_ADD_TEST(ll_remove_at_index);
    CELP_TEST_SUITE_ADD_TEST(ll_remove_node);
}
CELP_TEST_SUITE_END();

void test_celp_ll()
{
    CELP_TEST_SUITE_RUN(linked_list);
    CELP_TEST_SUITE_REPORT();
    CELP_TEST_SUITE_DESTROY();
}
