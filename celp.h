//TODO:
// - file reader
// - string builder/view
// - hashmap
// - memory alloc macros
// - DSs
// - Math (linear algebra)
// - Debug/Log

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

#define celp_da_info(da) \
    do{ \
        celp_log(CELP_LOG_LEVEL_INFO, "Capacity: %lu, Count: %lu\n", (da)->capacity, (da)->count); \
    } while(0)

//TODO_DA: remove, insert, bulk append


/* HashMap */
#define CELP_KV(name, key_dtype, value_dtype) \
typedef struct { \
    key_dtype* key; \
    value_dtype value; \
} name##KV##_t;

#define CELP_MAP(kv, name) \
typedef struct { \
    kv* items; \
    size_t count; \
    size_t capacity; \
} name##Map##_t;

#define celp_map_init(map) \
    do {\
        (map)->items = NULL; \
        (map)->count = 0; \
        (map)->capacity = 0; \
    } while(0)

#define celp_map_info(map) \
    do { \
        celp_log(CELP_LOG_LEVEL_INFO, "Capacity: %lu, Count: %lu", (map)->capacity, (map)->count); \
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
#ifdef CELP_STRIP_PREFIX
    //log
    #define log celp_log
    //dynamic array
    #define DA_ARRAY CELP_DA_ARRAY
    #define da_init celp_da_init
    #define da_append celp_da_append
    #define da_free celp_da_free
    #define da_info celp_da_info
    //map
    #define KV CELP_KV
    #define MAP CELP_MAP
    #define map_init celp_map_init
    #define map_info celp_map_info

#endif //CELP_STRIP_PREFIX

#endif //CELP_H
