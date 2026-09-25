#define CELP_PROFILE
#include "../celp.h"

int celp_profile_malloc(const celp_usize iterations,
                        const celp_u8 block_size)
{
    CELP_PROFILE_START(malloc, NULL);
    {
        void **pointers = malloc(iterations * block_size);
        CELP_PROFILE_START(malloc_alloc, CELP_PROFILE_PARENT(malloc));
        {
            for (int i = 0; i < iterations; i++) {
                pointers[i] = malloc(block_size);
                CELP_PROFILE_COUNT_INCREMENT(malloc_alloc, memory_allocations);
            }
        }
        CELP_PROFILE_COUNT_INCREMENT(malloc, memory_allocations_and_frees);

        CELP_PROFILE_END(malloc_alloc);

        CELP_PROFILE_START(malloc_free, CELP_PROFILE_PARENT(malloc));
        {
            for (int i = 0; i < iterations; i++) {
                free(pointers[i]);
                CELP_PROFILE_COUNT_INCREMENT(malloc_free, memory_frees);
            }
        }
        CELP_PROFILE_END(malloc_free);
    }
    CELP_PROFILE_END(malloc);
    CELP_PROFILE_REPORT(malloc);

    CELP_PROFILE_FREE();
    return 0;
}
