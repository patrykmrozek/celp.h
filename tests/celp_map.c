#define CELP_IMPLEMENTATION
#include "../celp.h"
#include <stdio.h>

CELP_KV(Test, int, size_t);
CELP_MAP(TestKV_t, Test);

int main() {
    printf("hello map\n");
    TestMap_t test = {};

    celp_map_init(&test);
    celp_map_info(&test);

    for (uint32_t i = 0; i < 10; i++) {
        celp_map_add(&test, i);
    }
    celp_map_info(&test);
    celp_map_add(&test, 5);
    celp_map_get(&test, 5);

    celp_map_free(&test);

    return 0;
}
