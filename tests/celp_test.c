#define CELP_IMPLEMENTATION
#include "../celp.h"

extern void celp_da();
extern void celp_ll();
extern void celp_map();
extern void celp_math();

int main()
{
    celp_da();
    celp_ll();
    celp_map();
    celp_math();
}
