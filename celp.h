//TODO:
// - file reader
// - string builder/view
// - hashmap
// - memory alloc/arena macros
// - DSs
// - Math (linear algebra)

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

#ifndef CELP_MALLOC
#include <stdlib.h>
#define CELP_MALLOC malloc
#endif //CELP_MALLOC

#ifndef CELP_CALLOC
#include <stdlib.h>
#define CELP_CALLOC calloc
#endif //CELP_CALLOC

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdarg.h>

typedef enum {
   CELP_LOG_LEVEL_INFO,
   CELP_LOG_LEVEL_ERROR,
} Celp_Log_Level_t;

CELP_DEF void celp_log(Celp_Log_Level_t log_type, const char* msg, ...);

/* Dynamic Array */
#define CELP_DA_INITIAL_CAPACITY 256

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
                (da)->capacity = CELP_DA_INITIAL_CAPACITY;\
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

#define celp_da_info(da) \
    do{ \
        celp_log(CELP_LOG_LEVEL_INFO, "Dynamic Array at: %p, Capacity: %lu, Count: %lu\n", (da), (da)->capacity, (da)->count); \
    } while(0)

//TODO_DA: remove, insert, bulk append


/* HashMap */
#define CELP_MAP_INITIAL_CAPACITY 64

#define CELP_KV(name, key_dtype, value_dtype) \
typedef struct { \
    key_dtype key; \
    value_dtype value; \
    bool is_occupied; \
} name##KV##_t;

#define CELP_MAP(kv, name) \
typedef struct { \
    kv* items; \
    size_t count; \
    size_t capacity; \
} name##Map##_t;

#define celp_map_clear(map) \
    do {\
        (map)->items = NULL; \
        (map)->count = 0; \
        (map)->capacity = 0; \
    } while(0)

#define celp_map_init(map) \
    do{ \
        celp_map_clear((map)); \
        (map)->capacity = CELP_MAP_INITIAL_CAPACITY; \
        (map)->items = CELP_CALLOC((map)->capacity, sizeof((map)->items[0])); \
    } while(0)

//TODO: manual realloc when hashing implemented
#define celp_map_reserve(map, expected_capacity) \
    do {\
        if ((expected_capacity) > (map)->capacity) {\
            if ((map)->capacity == 0) {\
                (map)->capacity = CELP_MAP_INITIAL_CAPACITY;\
            }\
            while ((map)->capacity < expected_capacity) {\
                (map)->capacity *= 2;\
            }\
            (map)->items = CELP_REALLOC((map)->items, (map)->capacity * sizeof((map)->items[0]));\
            CELP_ASSERT((map)->items != NULL && "Overflow");\
        }\
    } while(0)

//naive impl - linear search
#define celp_map_set(map, k, v) \
    do { \
        celp_map_reserve((map), (map)->count + 1); \
        for (size_t __i = 0; __i < (map)->capacity; __i++) { \
            if ((map)->items[__i].is_occupied && (map)->items[__i].key == (k)) { \
                (map)->items[__i].value = (v); \
                break; \
            } else if (!(map)->items[__i].is_occupied) { \
                (map)->items[__i].key = (k); \
                (map)->items[__i].value = (v); \
                (map)->items[__i].is_occupied = true; \
                (map)->count++; \
                break; \
            } \
        } \
    } while(0)

#define celp_map_add(map, k) \
do { \
    celp_map_reserve((map), (map)->count + 1); \
    for (size_t __i = 0; __i < (map)->capacity; __i++) { \
        if ((map)->items[__i].is_occupied && (map)->items[__i].key == (k)) { \
            (map)->items[__i].value++; \
            break; \
        } else if (!(map)->items[__i].is_occupied) { \
            (map)->items[__i].key = (k); \
            (map)->items[__i].value = 1; \
            (map)->items[__i].is_occupied = true; \
            (map)->count++; \
            break; \
        } \
    } \
} while(0)

//TODO: take in a defualt value in case not found, ditch void* -> keeps type safety
#define celp_map_get(map, k) ({ \
        void* __result = NULL; \
        for (size_t __i = 0; __i < (map)->capacity; __i++) { \
            if ((map)->items[__i].is_occupied && (map)->items[__i].key == (k)) { \
                __result = &(map)->items[__i].value; \
                break; \
            } \
        } \
        __result; \
    })
//      ^  last experssion becomes return val ({..}) expression -> when returning val
// could just use passed by * param

#define celp_map_free(map) \
    do { \
        CELP_FREE((map)->items); \
        celp_map_clear(map); \
    } while(0)

#define celp_map_info(map) \
    do { \
        celp_log(CELP_LOG_LEVEL_INFO, "Map at: %p, Capacity: %lu, Count: %lu", (map), (map)->capacity, (map)->count); \
    } while(0)


#ifdef CELP_IMPLEMENTATION

    void celp_log(Celp_Log_Level_t log_type, const char* fmt_string, ...) {
        va_list args;
        va_start(args, fmt_string); //last named param -> knows where to start with vargs

        FILE* out = NULL;
        const char* tag = NULL;

        switch(log_type) {
            case CELP_LOG_LEVEL_INFO:
                out = stdout;
                tag = "[INFO] ";
                break;
            case CELP_LOG_LEVEL_ERROR:
                out = stderr;
                tag = "[ERROR] ";
                break;
        }

        fputs(tag, out);
        vfprintf(out, fmt_string, args); //takes format string + va_list
        fputc('\n', out);

        va_end(args);
    }

#endif //CELP_IMPLEMENTATION

//shamelessly ripped from mr tsoding
//if you dont want to keep writing celp :|
#ifdef CELP_STRIP_PREFIX
    //CELP_LOG
    #define log celp_log
    //CELP_DA
    #define DA_ARRAY CELP_DA_ARRAY
    #define da_init celp_da_init
    #define da_append celp_da_append
    #define da_free celp_da_free
    #define da_info celp_da_info
    //CELP_MAP
    #define KV CELP_KV
    #define MAP CELP_MAP
    #define map_init celp_map_init
    #define map_clear celp_map_clear
    #define map_set celp_map_set
    #define map_add celp_map_add
    #define map_get celp_map_get
    #define map_free celp_map_free
    #define map_info celp_map_info

#endif //CELP_STRIP_PREFIX

#endif //CELP_H
