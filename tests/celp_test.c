#define CELP_IMPLEMENTATION
#include "../celp.h"

extern void celp_da();
extern void celp_ll();
extern void celp_map();
extern void celp_math();

int main()
{
    celp_da();
    printf("celp_da() [PASSED]\n");
    celp_ll();
    printf("celp_ll() [PASSED]\n");
    celp_map();
    printf("celp_map() [PASSED]\n");
    celp_math();
    printf("celp_math() [PASSED]\n");

    return 0;
}
