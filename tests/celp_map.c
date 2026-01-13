#define CELP_IMPLEMENTATION
#include "../celp.h"
#include <stdio.h>

typedef size_t sizet;
CELP_MAP(int, sizet);

int main() {
    Map_int_sizet_t test = {};
    celp_map_init(&test);
    int k = 10;
    const unsigned char* k_bytes = (const unsigned char*)&k;
    uint32_t h = celp_hash(k_bytes, sizeof(k)) % test.capacity;
    celp_log(CELP_LOG_LEVEL_DEBUG, "hash for key 10: %u, bucket count: %zu",h, test.buckets[h].count);
    celp_map_insert(&test, 10, 100);
    celp_map_insert(&test, 11, 100);
    celp_map_insert(&test, 12, 100);
    celp_log(CELP_LOG_LEVEL_DEBUG, "after, bucket[%u].count = %zu, map.count = %zu", h, test.buckets[h].count, test.count);
    celp_log(CELP_LOG_LEVEL_DEBUG, "bucket %u first item key: %d", h, test.buckets[h].head->next->data.key);
    celp_map_info(&test);
    size_t get = celp_map_get(&test, 10, 0);
    celp_log(CELP_LOG_LEVEL_DEBUG, "Gotten: %zu", get);
    celp_log(CELP_LOG_LEVEL_DEBUG, "Incrementing");
    celp_map_increment(&test, 10);
    celp_log(CELP_LOG_LEVEL_DEBUG, "Gotten after: %zu", celp_map_get(&test, 10, 0));
    celp_log(CELP_LOG_LEVEL_DEBUG, "Found key: %s", (celp_map_contains(&test, 10) ? "yes" : "no"));
    celp_log(CELP_LOG_LEVEL_DEBUG, "Found key: %s", (celp_map_contains(&test, 5) ? "yes" : "no"));

    celp_map_remove(&test, 10);

    celp_map_free(&test);
    celp_map_info(&test);
    return 0;
}
