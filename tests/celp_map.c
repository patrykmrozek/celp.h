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

CELP_TESTCASE(map_clear_and_is_empty)
{
    celp_map_clear(&map);
    CELP_EXPECT(celp_map_is_empty(&map));
}

CELP_TEST_SUITE_START(kv_map)
{
    CELP_TEST_SUITE_ADD_SETUP(map);
    CELP_TEST_SUITE_ADD_TEARDOWN(map);
    CELP_TEST_SUITE_ADD_TEST(map_hash);
    CELP_TEST_SUITE_ADD_TEST(map_insert);
    CELP_TEST_SUITE_ADD_TEST(map_contains);
    CELP_TEST_SUITE_ADD_TEST(map_get);
    CELP_TEST_SUITE_ADD_TEST(map_increment_existing_value);
    CELP_TEST_SUITE_ADD_TEST(map_increment_non_existing_value);
    CELP_TEST_SUITE_ADD_TEST(map_remove);
    CELP_TEST_SUITE_ADD_TEST(map_clear_and_is_empty);
} CELP_TEST_SUITE_END();

void test_celp_map() {
    CELP_TEST_SUITE_RUN(kv_map);
    CELP_TEST_SUITE_REPORT();
    CELP_TEST_SUITE_DESTROY();
}


/*
void _celp_map() {
    celp_map(int, size_t);
    celp_map_t(int, size_t) test = {};
    celp_map_init(&test);
    int k = 10;
    const unsigned char* k_bytes = (const unsigned char*)&k;
    uint32_t h = CELP_HASH(k_bytes, sizeof(k)) % test.capacity;
    CELP_DEBUG(1, "hash for key 10: %u, bucket count: %zu",h, test.buckets[h].count);
    celp_map_insert(&test, 10, 100);
    celp_map_insert(&test, 11, 100);
    celp_map_insert(&test, 12, 100);
    CELP_DEBUG(1, "after, bucket[%u].count = %zu, map.count = %zu", h, test.buckets[h].count, test.count);
    CELP_DEBUG(1, "bucket %u first item key: %d", h, test.buckets[h].head->next->data.key);
    celp_map_info(&test);
    size_t get = celp_map_get(&test, 10, 0);
    CELP_DEBUG(1, "Gotten: %zu", get);
    CELP_DEBUG(1, "Incrementing");
    celp_map_increment(&test, 10);
    CELP_DEBUG(1, "Gotten after: %zu", celp_map_get(&test, 10, 0));
    CELP_DEBUG(1, "Found key: %s", (celp_map_contains(&test, 10) ? "yes" : "no"));
    CELP_DEBUG(1, "Found key: %s", (celp_map_contains(&test, 5) ? "yes" : "no"));

    celp_map_remove(&test, 10);

    celp_map_destroy(&test);
    celp_map_info(&test);
}
*/
