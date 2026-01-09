#ifndef CELP_H
#define CELP_H

#ifndef CELP_DEF
#define CELP_DEF
#endif

#ifndef CELP_DEF_SI
#define CELP_DEF_SI static inline
#endif //CELP_DEF_SI

#ifndef CELP_ASSERT
#include <assert.h>
#define CELP_ASSERT assert
#endif //CELP_ASSERT

#ifndef CELP_FREE
#include <stdlib.h>
#define CELP_FREE free
#endif //CELP_FREE

#define celp_da_append(xs, x) \
    do {
        if (xs.count >= xs.capacity) {\
            if (xs.capacity == 0) xs.capacity = 256;\
                else xs.capacity *= 2;\
                xs.items = realloc(xs.items, xs.capacity * sizeof(xs.items));\
        }\
        xs.items[xs.count++] = x;
    } while(0)


#ifdef CELP_IMPLEMENTATION
//implementation


#endif

#endif //CELP_H
