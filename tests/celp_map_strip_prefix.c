#define CELP_IMPLEMENTATION
#define CELP_STRIP_PREFIX
#include "../celp.h"
#include <stdio.h>

KV(Test, int, size_t);
MAP(TestKV_t, Test);

int main() {
    TestMap_t test = {};

    map_init(&test);
    map_info(&test);

    for (int i = 0; i < 10; i++) {
        map_set(&test, i, i*10);
        printf("Set key: %i, value: %i\n", i, i*10);
    }
    map_info(&test);

    map_add(&test, 5);
    int* result = (int *)map_get(&test, 5);
    printf("Result: %i\n", *result);

    for (int i = 50; i < 200; i++) {
        map_set(&test, i, i);
        //printf("Set key: %i, value: %i\n", i, i*10);
    }
    map_info(&test);

    printf("[1 before]Key: %i, Value: %lu\n", test.items[1].key, test.items[1].value);
    for (size_t i = 0; i < 100; i++) {
        map_add(&test, 1);
        //printf("Adding to key (1): Current value: %lu\n", test.items[1].value);
    }
    printf("[1 after]Key: %i, Value: %lu\n", test.items[1].key, test.items[1].value);

    map_free(&test);
    return 0;
}
