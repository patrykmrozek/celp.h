#define CELP_TEST
#include "../celp.h"

const char *hello = "hello world";
celp_str_t str;

CELP_TEST_SETUP(str)
{
    str = celp_str(hello);
}

CELP_TEST_TEARDOWN(str)
{
    celp_str_free(&str);
}

CELP_TESTCASE(str)
{
    bool str_eq = celp_str_eq(&str, hello);
    CELP_EXPECT(str_eq);
}

CELP_TESTCASE(str_append_n)
{
    celp_usize old_count = str.count;
    const char *append_n = " testing";
    celp_usize len = strlen(append_n);
    celp_str_append_n(&str, append_n, len);
    CELP_EXPECT_EQ(str.count, old_count + len);
}

CELP_TESTCASE(str_append)
{
    celp_usize old_count = str.count;
    const char *append = "!";
    celp_str_append(&str, append);
    CELP_EXPECT_EQ(str.count, old_count + 1);
    CELP_EXPECT_EQ(celp_da_last(&str, NULL), *append);
}

CELP_TESTCASE(strv)
{
    celp_strv_t view = celp_strv(&str);
    CELP_EXPECT_EQ(str.count, view.count);
    CELP_EXPECT_EQ(str.items, view.items);
}

CELP_TESTCASE(strv_cstr_n)
{
    const char *cstr = "hello";
    celp_strv_t view = celp_strv_cstr(cstr);
    CELP_EXPECT_EQ(view.items, cstr);
}

CELP_TESTCASE(strv_n)
{
    celp_usize n = 5;
    celp_strv_t view = celp_strv_n(&str, n);
    //celp_strv_print(view);
    CELP_EXPECT_EQ(view.count, n);
    CELP_EXPECT(celp_strv_eq(view, "hello"));
}

CELP_TESTCASE(strv_slice)
{
    celp_usize n = 5;
    celp_strv_t view = celp_strv_slice(&str, 6, n);
    //celp_strv_print(view);
    CELP_EXPECT_EQ(view.count, n);
    CELP_EXPECT(celp_strv_eq(view, "world"));
}

CELP_TEST_SUITE_START(string)
{
    CELP_TEST_SUITE_ADD_SETUP(string, str);
    CELP_TEST_SUITE_ADD_TEARDOWN(string, str);
    CELP_TEST_SUITE_ADD_TEST(string, str);
    CELP_TEST_SUITE_ADD_TEST(string, str_append_n);
    CELP_TEST_SUITE_ADD_TEST(string, str_append);
    CELP_TEST_SUITE_ADD_TEST(string, strv_cstr_n);
    CELP_TEST_SUITE_ADD_TEST(string, strv);
    CELP_TEST_SUITE_ADD_TEST(string, strv_n);
    CELP_TEST_SUITE_ADD_TEST(string, strv_slice);
}
CELP_TEST_SUITE_END(string);

void test_celp_str(void)
{
    CELP_TEST_SUITE_RUN(string);
    CELP_TEST_SUITE_REPORT(string);
    CELP_TEST_SUITE_DESTROY(string);
}
