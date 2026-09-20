#define CELP_PROFILE
#include "../celp.h"

int celp_profile_arena(const celp_usize iterations,
                       const celp_u8 block_size)
{
    CELP_PROFILE_START(arena, NULL);
    {
        celp_arena_t *arena = celp_arena_create(iterations * block_size);
        CELP_PROFILE_START(arena_alloc, CELP_PROFILE_PARENT(arena));
        {
            for (int i = 0; i < iterations; i++) {
                celp_arena_alloc(arena, sizeof(celp_u8));
                CELP_PROFILE_COUNT(arena_alloc);
            }
        }
        CELP_PROFILE_END(arena_alloc);
        CELP_PROFILE_COUNT(arena);

        CELP_PROFILE_START(arena_free, CELP_PROFILE_PARENT(arena));
        {
            celp_arena_free(arena);
            CELP_PROFILE_COUNT(arena_free);
        }
        CELP_PROFILE_END(arena_free);
    }
    CELP_PROFILE_END(arena);
    CELP_PROFILE_REPORT(arena);

    CELP_PROFILE_FREE();
    return 0;
}
