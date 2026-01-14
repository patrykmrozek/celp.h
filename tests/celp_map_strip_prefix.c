#define CELP_IMPLEMENTATION
#define CELP_STRIP_PREFIX
#include "../celp.h"
#include <stdio.h>

typedef size_t sizet;
MAP(int, sizet);

int main() {
    Map_int_sizet_t test = {};
    map_init(&test);
    int k = 10;
    const unsigned char* k_bytes = (const unsigned char*)&k;
    uint32_t h = hash(k_bytes, sizeof(k)) % test.capacity;
    log(LOG_LEVEL_DEBUG, "hash for key 10: %u, bucket count: %zu",h, test.buckets[h].count);
    map_insert(&test, 10, 100);
    map_insert(&test, 11, 100);
    map_insert(&test, 12, 100);
    log(LOG_LEVEL_DEBUG, "after, bucket[%u].count = %zu, map.count = %zu", h, test.buckets[h].count, test.count);
    log(LOG_LEVEL_DEBUG, "bucket %u first item key: %d", h, test.buckets[h].head->next->data.key);
    map_info(&test);
    size_t get = map_get(&test, 10, 0);
    log(LOG_LEVEL_DEBUG, "Gotten: %zu", get);
    log(LOG_LEVEL_DEBUG, "Incrementing");
    map_increment(&test, 10);
    log(LOG_LEVEL_DEBUG, "Gotten after: %zu", map_get(&test, 10, 0));
    log(LOG_LEVEL_DEBUG, "Found key: %s", (map_contains(&test, 10) ? "yes" : "no"));
    log(LOG_LEVEL_DEBUG, "Found key: %s", (map_contains(&test, 5) ? "yes" : "no"));

    map_remove(&test, 10);

    map_free(&test);
    map_info(&test);
    return 0;
}
