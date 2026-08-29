#define CELP_IMPLEMENTATION
#include "../celp.h"

extern void test_celp_da();
extern void test_celp_ll();
extern void test_celp_map();
extern void test_celp_math();
extern void test_celp_arena();

int main()
{
    test_celp_da();
    test_celp_ll();
    /*
    celp_map();
    printf("celp_map() [PASSED]\n");
    celp_math();
    printf("celp_math() [PASSED]\n");
    */
    test_celp_arena();

    return 0;
}
