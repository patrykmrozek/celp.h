#define CELP_TEST
#include "../celp.h"

celp_map(int, int);
celp_map_t(int, int) map = {};

CELP_TEST_SETUP(map)
{
    celp_map_init(&map);
}

CELP_TEST_TEARDOWN(map)
{
    celp_map_destroy(&map);
}

CELP_TESTCASE(map_hash)
{
    int key = 10000;
    celp_u32 hash = _celp_map_get_hash(&map, key);
    CELP_EXPECT(hash<=CELP_MAP_INITIAL_CAPACITY);
}

CELP_TESTCASE(map_insert)
{
    int amount = 10;
    for (int i = 0; i < amount; i++) {
        celp_map_insert(&map, i, 100);
    }
    CELP_EXPECT_EQ(map.count, amount);
}

CELP_TESTCASE(map_contains)
{
    for (int i = 0; i < map.count; i++) {
        CELP_EXPECT(celp_map_contains(&map, i));
    }
    CELP_EXPECT(!celp_map_contains(&map, 1000));
}

CELP_TESTCASE(map_get)
{
    int ret = celp_map_get(&map, 5, -1);
    CELP_EXPECT_EQ(ret, 100);

    ret = celp_map_get(&map, 999, -1);
    CELP_EXPECT_EQ(ret, -1);
}

CELP_TESTCASE(map_increment_existing_value)
{
    int before = celp_map_get(&map, 5, -1);
    CELP_EXPECT_NEQ(before, -1);

    celp_map_increment(&map, 5);
    int after = celp_map_get(&map, 5, -1);
    CELP_EXPECT_EQ(after, before+1);
}

CELP_TESTCASE(map_increment_non_existing_value)
{
    int key = 999;
    int before = celp_map_get(&map, key, -1);
    CELP_EXPECT_EQ(before, -1);

    celp_map_increment(&map, key);
    int after = celp_map_get(&map, key, -1);
    CELP_EXPECT_NEQ(after, -1);
    CELP_EXPECT_EQ(after, 1);
}

CELP_TESTCASE(map_remove)
{
    int ret = celp_map_remove(&map, 999, -1);
    CELP_EXPECT_NEQ(ret, -1);

    ret = celp_map_remove(&map, 999, -1);
    CELP_EXPECT_EQ(ret, -1);
}

CELP_TEST_SUITE_START(kv_map);
{
    CELP_TEST_SUITE_ADD_SETUP(kv_map, map);
    CELP_TEST_SUITE_ADD_TEARDOWN(kv_map, map);
    
    CELP_TEST_SUITE_ADD_TEST(kv_map, map_hash);
    CELP_TEST_SUITE_ADD_TEST(kv_map, map_insert);
    CELP_TEST_SUITE_ADD_TEST(kv_map, map_contains);
    CELP_TEST_SUITE_ADD_TEST(kv_map, map_get);
    CELP_TEST_SUITE_ADD_TEST(kv_map, map_increment_existing_value);
    CELP_TEST_SUITE_ADD_TEST(kv_map, map_increment_non_existing_value);
    CELP_TEST_SUITE_ADD_TEST(kv_map, map_remove);
}
CELP_TEST_SUITE_END(kv_map);

void test_celp_map() {
    CELP_TEST_SUITE_RUN(kv_map);
    CELP_TEST_SUITE_REPORT(kv_map);
    CELP_TEST_SUITE_DESTROY(kv_map);
}

