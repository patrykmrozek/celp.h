#define CELP_PROFILE
#include "../celp.h"

int celp_profile_malloc(const celp_usize iterations,
                        const celp_u8 block_size)
{
    CELP_PROFILE_CREATE(malloc, NULL);
    CELP_PROFILE_CREATE(malloc_alloc, CELP_PROFILE_PARENT(malloc));
    CELP_PROFILE_CREATE(malloc_free, CELP_PROFILE_PARENT(malloc));

    CELP_PROFILE_START(malloc);
    {
        void **pointers = malloc(iterations * block_size);
        CELP_PROFILE_START(malloc_alloc);
        {
            for (celp_u32 i = 0; i < iterations; i++) {
                pointers[i] = malloc(block_size);
                CELP_PROFILE_COUNT_ADD(malloc_alloc, memory_allocations);
            }
        }

        CELP_PROFILE_END(malloc_alloc);

        CELP_PROFILE_START(malloc_free);
        {
            for (celp_u32 i = 0; i < iterations; i++) {
                free(pointers[i]);
                CELP_PROFILE_COUNT_ADD(malloc_free, memory_frees);
            }
        }
        CELP_PROFILE_END(malloc_free);
    }
    CELP_PROFILE_END(malloc);
    CELP_PROFILE_REPORT(malloc);

    CELP_PROFILE_FREE();
    return 0;
}
