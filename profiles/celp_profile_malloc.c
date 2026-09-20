#define CELP_PROFILE
#include "../celp.h"

int celp_profile_malloc(const celp_usize iterations,
                        const celp_u8 block_size)
{
    void **pointers = malloc(iterations * block_size);

    CELP_PROFILE_START(malloc);
    {
        for (int i = 0; i < iterations; i++) {
            pointers[i] = malloc(block_size);
        }
    }
    CELP_PROFILE_END(malloc);
    CELP_PROFILE_REPORT(malloc);

    CELP_PROFILE_START(free);
    for (int i = 0; i < iterations; i++) {
        free(pointers[i]);
    }
    CELP_PROFILE_END(free);
    CELP_PROFILE_REPORT(free);

    return 0;
}
