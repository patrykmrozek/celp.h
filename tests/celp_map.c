#define CELP_IMPLEMENTATION
#include "../celp.h"
#include <stdio.h>

CELP_KV(Test, int, size_t);
CELP_MAP(TestKV_t, Test);

int main() {
    TestMap_t test = {};

    celp_map_init(&test);
    celp_map_info(&test);

    for (int i = 0; i < 10; i++) {
        celp_map_set(&test, i, i*10);
        printf("Set key: %i, value: %i\n", i, i*10);
    }
    celp_map_info(&test);

    celp_map_add(&test, 5);
    int* result = (int *)celp_map_get(&test, 5);
    printf("Result: %i\n", *result);

    for (int i = 50; i < 200; i++) {
        celp_map_set(&test, i, i);
        printf("Set key: %i, value: %i\n", i, i*10);
    }
    celp_map_info(&test);
    celp_map_free(&test);

    return 0;
}
