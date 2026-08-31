#define CELP_TEST
#include "../celp.h"

celp_da(int);
celp_da_t(int) n = {};

CELP_TEST_SETUP(da)
{
    celp_da_init(&n);
}

CELP_TEST_TEARDOWN(da)
{
    celp_da_free(&n);
}

CELP_TESTCASE(da_append)
{
    int num_append = 10;
    for (uint32_t i = 0; i < num_append; i++) {
        celp_da_append(&n, i);
    }
    CELP_EXPECT_EQ(n.count, num_append);
}

CELP_TESTCASE(da_last)
{
    int last = celp_da_last(&n);
    CELP_EXPECT_EQ(n.items[n.count-1], last);
}

CELP_TESTCASE(da_pop)
{
    int old_count = n.count;
    int last_val = n.items[n.count-1];
    int popped_val = celp_da_pop(&n);
    CELP_EXPECT_EQ(last_val, popped_val);
    CELP_EXPECT_EQ(n.count, old_count-1);
}

CELP_TESTCASE(da_remove)
{
    int old_count = n.count;
    int first = n.items[0];
    int removed = celp_da_remove(&n, 0);
    CELP_EXPECT_EQ(first, removed);
    CELP_EXPECT_EQ(n.count, old_count-1);
}

CELP_TESTCASE(da_foreach)
{
    int arr[n.count];
    for (int i = 0; i < n.count; i++) {
        arr[i] = n.items[i]+1;
    }
    celp_da_foreach(&n, iter) {
        *iter+=1;
    }
    for (int i = 0; i < n.count; i++) {
        CELP_EXPECT_EQ(arr[i], n.items[i]);
    }
}

CELP_TESTCASE(da_reserve)
{
    int old_cap = n.capacity;
    for (int i = 0; i < 300; i++) {
        celp_da_append(&n, i);
    }
    CELP_EXPECT(n.capacity == 2*old_cap);
}

CELP_TESTCASE(da_clear)
{
    celp_da_clear(&n);
    CELP_EXPECT(n.count==0);
}

CELP_TESTCASE(da_is_empty)
{
    CELP_EXPECT_EQ(celp_da_is_empty(&n), n.count==0);
}

CELP_TEST_SUITE_START(dynamic_array);
{
    CELP_TEST_SUITE_ADD_SETUP(dynamic_array, da);
    CELP_TEST_SUITE_ADD_TEARDOWN(dynamic_array, da);

    CELP_TEST_SUITE_ADD_TEST(dynamic_array, da_append);
    CELP_TEST_SUITE_ADD_TEST(dynamic_array, da_pop);
    CELP_TEST_SUITE_ADD_TEST(dynamic_array, da_last);
    CELP_TEST_SUITE_ADD_TEST(dynamic_array, da_remove);
    CELP_TEST_SUITE_ADD_TEST(dynamic_array, da_foreach);
    CELP_TEST_SUITE_ADD_TEST(dynamic_array, da_reserve);
    CELP_TEST_SUITE_ADD_TEST(dynamic_array, da_clear);
    CELP_TEST_SUITE_ADD_TEST(dynamic_array, da_is_empty);
}
CELP_TEST_SUITE_END(dynamic_array);

void test_celp_da()
{
    CELP_TEST_SUITE_RUN(dynamic_array);
    CELP_TEST_SUITE_REPORT(dynamic_array);
    CELP_TEST_SUITE_DESTROY(dynamic_array);
}
