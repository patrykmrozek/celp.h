#define CELP_TEST
#include "../celp.h"

typedef struct {int x, y;} foo;
static celp_arena_t *a = NULL;

CELP_TEST_SETUP(arena)
{
    a = celp_arena_create(1024);
} 

CELP_TEST_TEARDOWN(arena)
{
    celp_arena_free(a);
}

CELP_TESTCASE(arena_alloc)
{
    void *thing = celp_arena_alloc(a, 100);
    CELP_EXPECT(thing);
    CELP_EXPECT_NEQ(thing, a);

    foo *thing2 = (foo *)celp_arena_alloc(a, sizeof(foo));
    CELP_EXPECT(thing2);
    CELP_EXPECT_NEQ(thing, thing2);
}

CELP_TESTCASE(arena_alloc_new_region)
{
    celp_usize old_count = a->count;
    (void)celp_arena_alloc(a, 1024);
    CELP_EXPECT_EQ(a->count, old_count + 1);
}

CELP_TEST_SUITE_START(arena_allocator)
{
    CELP_TEST_SUITE_ADD_SETUP(arena_allocator, arena);
    CELP_TEST_SUITE_ADD_TEARDOWN(arena_allocator, arena);

    CELP_TEST_SUITE_ADD_TEST(arena_allocator, arena_alloc);
    CELP_TEST_SUITE_ADD_TEST(arena_allocator, arena_alloc_new_region);
}
CELP_TEST_SUITE_END(arena_allocator);

void test_celp_arena()
{
    CELP_TEST_SUITE_RUN(arena_allocator);
    CELP_TEST_SUITE_REPORT(arena_allocator);
    CELP_TEST_SUITE_DESTROY(arena_allocator);
}
