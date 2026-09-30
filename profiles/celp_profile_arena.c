#define CELP_PROFILE
#include "../celp.h"

int celp_profile_arena(const celp_usize iterations,
                       const celp_u8 block_size)
{
    CELP_PROFILE_CREATE(arena, NULL);
    CELP_PROFILE_CREATE(arena_alloc, CELP_PROFILE_PARENT(arena));
    CELP_PROFILE_CREATE(arena_free, CELP_PROFILE_PARENT(arena));

    CELP_PROFILE_START(arena);
    {
        celp_arena_t *arena = celp_arena_create(iterations * block_size);
        CELP_PROFILE_START(arena_alloc);
        {
            for (celp_u32 i = 0; i < iterations; i++) {
                (void)celp_arena_alloc(arena, sizeof(celp_u8));
                CELP_PROFILE_COUNT_ADD(arena_alloc, allocations);
            }
        }
        CELP_PROFILE_END(arena_alloc);

        CELP_PROFILE_START(arena_free);
        {
            celp_arena_free(arena);
            CELP_PROFILE_COUNT_ADD(arena_free, frees);
        }
        CELP_PROFILE_END(arena_free);
    }
    CELP_PROFILE_END(arena);
    CELP_PROFILE_REPORT(arena);

    CELP_PROFILE_FREE();
    return 0;
}
