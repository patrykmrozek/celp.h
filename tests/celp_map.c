#define CELP_IMPLEMENTATION
#include "../celp.h"
#include <stdio.h>

//typedef size_t sz;
CELP_MAP(int, size_t);
typedef Map_int_size_t_t Map_int_sz_t;


int main() {
    Map_int_size_t_t test = {};

    celp_map_init(&test);
    celp_map_info(&test);

    for (int i = 0; i < 10; i++) {
        celp_map_set(&test, i, (size_t)i*10);
        //printf("Set key: %i, value: %lu\n", i, (size_t)i*10);
    }
    celp_map_info(&test);

    celp_map_add(&test, 5);
    int result = celp_map_get(&test, 5, 0);
    printf("Result: %i\n", result);

    for (int i = 50; i < 200; i++) {
        celp_map_set(&test, i, (size_t)i);
        //printf("Set key: %i, value: %lu\n", i, (size_t)i*10);
    }
    celp_map_info(&test);

    printf("[1 before]Key: %i, Value: %lu\n", test.items[1].key, test.items[1].value);
    for (size_t i = 0; i < 100; i++) {
        celp_map_add(&test, 1);
        //printf("Adding to key (1): Current value: %lu\n", test.items[1].value);
    }
    printf("[1 after]Key: %i, Value: %lu\n", test.items[1].key, test.items[1].value);

    celp_map_free(&test);
    return 0;
}
