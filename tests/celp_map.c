#include "../celp.h"
#include <stdio.h>

void test_celp_map() {
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

    celp_map_free(&test);
    celp_map_info(&test);
}
