#define CELP_IMPLEMENTATION
#include "../celp.h"

extern int celp_profile_malloc(const celp_usize iterations,
                               const celp_u8 block_size);
extern int celp_profile_arena(const celp_usize iterations,
                              const celp_u8 block_size);



#define ITERATIONS 100000000
#define BLOCK_SIZE 64

int main()
{
    celp_profile_malloc(ITERATIONS, BLOCK_SIZE);
    celp_profile_arena(ITERATIONS, BLOCK_SIZE);

    return 0;
}
