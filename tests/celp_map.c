#include "../celp.h"
#include <stdio.h>

CELP_KV(Test, int, size_t);
CELP_MAP(TestKV_t, Test);

int main() {
    printf("hello map\n");
    TestMap_t test = {};

    celp_map_init(&test);

    printf("capacity: %lu, count: %lu\n", test.capacity, test.count);



    return 0;
}
