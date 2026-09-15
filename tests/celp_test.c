#define CELP_IMPLEMENTATION
#include "../celp.h"

extern void test_celp_da();
extern void test_celp_ll();
extern void test_celp_ll_noerror();
extern void test_celp_map();
extern void test_celp_math();

int main()
{
    test_celp_da();
    test_celp_ll();
    test_celp_ll_noerror();
    test_celp_map();
    //test_celp_math();
    return 0;
}
