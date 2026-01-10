//TODO:
// - file reader
// - string builder/view
// - hashmap
// - memory alloc macros
// - DSs
// - Math (linear algebra)
// - Debug/Log
// - strip perfix macro

#ifndef CELP_H
#define CELP_H

#ifndef CELP_DEF
#define CELP_DEF
#endif //CELP_DEF

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

#ifndef CELP_REALLOC
#include <stdlib.h>
#define CELP_REALLOC realloc
#endif //CELP_REALLOC


#define CELP_DA_INIT_SIZE 256

/*
 * Generates an array struct for a given type
 */
#define CELP_DA_ARRAY(name, dtype) \
typedef struct { \
    dtype* items; \
    size_t count; \
    size_t capacity; \
} name##Array##_t;

#define celp_da_init(da) \
do { \
    (da)->items = NULL;\
    (da)->count = 0;\
    (da)->capacity = 0;\
} while(0)

#define celp_da_reserve(da, expected_capacity) \
    do {\
        if ((expected_capacity) > (da)->capacity) {\
            if ((da)->capacity == 0) {\
                (da)->capacity = CELP_DA_INIT_SIZE;\
            }\
            while ((da)->capacity < expected_capacity) {\
                (da)->capacity *= 2;\
            }\
            (da)->items = CELP_REALLOC((da)->items, (da)->capacity * sizeof((da)->items[0]));\
            CELP_ASSERT((da)->items != NULL);\
        }\
    } while(0)

#define celp_da_append(da, item) \
    do {\
        celp_da_reserve(da, (da)->count + 1);\
        (da)->items[(da)->count++] = (item);\
    } while(0)

#define celp_da_free(da) \
    do { \
        CELP_FREE((da)->items); \
        celp_da_init(da); \
    } while(0)

//TODO_DA: remove, insert, bulk append


#ifdef CELP_IMPLEMENTATION
//implementation


#endif

#ifdef CELP_STRIP_PREFIX
    //dynamic array
    #define DA_ARRAY CELP_DA_ARRAY
    #define da_init celp_da_init
    #define da_append celp_da_append
    #define da_free celp_da_free

#endif //CELP_STRIP_PREFIX

#endif //CELP_H
