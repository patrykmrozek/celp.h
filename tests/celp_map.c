#define CELP_ERRORS
#define CELP_TEST
#include "../celp.h"

celp_map(int, int);
celp_map_t(int, int) map = {};
celp_err_t error = CELP_ERR_OK;

CELP_TEST_SETUP(map)
{
    celp_map_init(&map, &error);
}

CELP_TEST_TEARDOWN(map)
{
    celp_map_free(&map, &error);
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
        error = CELP_ERR_OK;
        celp_map_insert(&map, i, 100, &error);
        CELP_EXPECT_EQ(error, CELP_ERR_OK);
    }
    CELP_EXPECT_EQ(map.count, amount);
}

CELP_TESTCASE(map_contains)
{
    bool found;
    for (int i = 0; i < map.count; i++) {
        error = CELP_ERR_OK;
        found = celp_map_contains(&map, i, &error);
        CELP_EXPECT_EQ(error, CELP_ERR_OK);
        CELP_EXPECT(found);
    }
    CELP_EXPECT(!celp_map_contains(&map, 1000, &error));
}

CELP_TESTCASE(map_get)
{
    error = CELP_ERR_OK;
    int ret = celp_map_get(&map, 5, -1, &error);
    CELP_EXPECT_EQ(error, CELP_ERR_OK);
    CELP_EXPECT_NEQ(ret, -1);
    CELP_EXPECT_EQ(ret, 100);

    ret = celp_map_get(&map, 999, -1, &error);
    CELP_EXPECT_EQ(error, CELP_ERR_OK);
    CELP_EXPECT_EQ(ret, -1);
}

CELP_TESTCASE(map_increment_existing_value)
{
    error = CELP_ERR_OK;
    int before = celp_map_get(&map, 5, -1, &error);
    CELP_EXPECT_EQ(error, CELP_ERR_OK);
    CELP_EXPECT_NEQ(before, -1);

    celp_map_increment(&map, 5, &error);
    int after = celp_map_get(&map, 5, -1, &error);
    CELP_EXPECT_EQ(error, CELP_ERR_OK);
    CELP_EXPECT_EQ(after, before+1);
}

CELP_TESTCASE(map_increment_non_existing_value)
{
    error = CELP_ERR_OK; 
    int key = 999;
    int before = celp_map_get(&map, key, -1, &error);
    CELP_EXPECT_EQ(error, CELP_ERR_OK);
    CELP_EXPECT_EQ(before, -1);

    celp_map_increment(&map, key, &error);
    CELP_EXPECT_EQ(error, CELP_ERR_OK);

    int after = celp_map_get(&map, key, -1, &error);
    CELP_EXPECT_EQ(error, CELP_ERR_OK);
    CELP_EXPECT_NEQ(after, -1);
    CELP_EXPECT_EQ(after, 1);
}

CELP_TESTCASE(map_remove)
{
    error = CELP_ERR_OK;
    int ret = celp_map_remove(&map, 999, &error);
    CELP_EXPECT_NEQ(ret, -1);
    CELP_EXPECT_EQ(error, CELP_ERR_OK);

    ret = celp_map_remove(&map, 999, &error);
    CELP_EXPECT_EQ(error, CELP_ERR_OK);
    CELP_EXPECT_EQ(ret, 0);
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

