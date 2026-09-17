#define CELP_TEST
#include "../celp.h"

const char *hello = "hello world";
celp_str_t str;

CELP_TEST_SETUP(str)
{
    celp_str_t str = celp_str(hello);
}

CELP_TEST_TEARDOWN(str)
{
    celp_str_free(&str);
}

CELP_TESTCASE(str_create)
{
    str = celp_str(hello);
    bool str_eq = celp_str_eq(str, hello);
    CELP_EXPECT(str_eq);
}

CELP_TEST_SUITE_START(string)
{
    CELP_TEST_SUITE_ADD_SETUP(string, str);
    CELP_TEST_SUITE_ADD_TEARDOWN(string, str);
    CELP_TEST_SUITE_ADD_TEST(string, str_create);
}
CELP_TEST_SUITE_END(string);

void test_celp_str(void)
{
    CELP_TEST_SUITE_RUN(string);
    CELP_TEST_SUITE_REPORT(string);
    CELP_TEST_SUITE_DESTROY(string);
}
