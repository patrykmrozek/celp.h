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
#include <string.h>

/* Logging */
typedef enum {
   CELP_LOG_LEVEL_INFO,
   CELP_LOG_LEVEL_DEBUG,
   CELP_LOG_LEVEL_ERROR,
} Celp_Log_Level_t;

CELP_DEF void celp_log(Celp_Log_Level_t log_type, const char* msg, ...);

/* Compare */

#define celp_compare(a, b) \
    memcmp(&(a), &(b), sizeof(a))

/* Dynamic Array */
#define CELP_DA_INITIAL_CAPACITY 256

/*
 * Generates an array struct for a given type
 */
#define CELP_DA(dtype) \
typedef struct { \
    dtype* items; \
    size_t count; \
    size_t capacity; \
} DA_##dtype##_t;

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

#define celp_da_last(da) ({ \
    CELP_ASSERT((da)->count > 0); \
    (da)->items[(da)->count-1]; \
})

#define celp_da_pop(da) ({ \
    CELP_ASSERT((da)->count > 0); \
    (da)->items[--(da)->count]; \
})

#define celp_da_remove(da, idx) ({ \
        CELP_ASSERT(idx < (da)->count); \
        typeof((da)->items[0]) __temp = (da)->items[(idx)]; \
        (da)->items[(idx)] = celp_da_last((da)); \
        (da)->items[(da)->count-1] = __temp; \
        celp_da_pop(da); \
    })

// user provides some label (i) -> macro initializes it as a pointer to (da)->itemsa
// elements can be accessed by dereferencing (i)
// to get index 0, suntract (i) from (da)->items
#define celp_da_foreach(da, i) for (typeof(*(da)->items)* (i) = (da)->items; (i) < (da)->items + (da)->count; (i)++)

#define celp_da_free(da) \
    do { \
        CELP_FREE((da)->items); \
        celp_da_init(da); \
    } while(0)

#define celp_da_info(da) \
    do{ \
        celp_log(CELP_LOG_LEVEL_INFO, "Dynamic Array at: %p, Capacity: %zu, Count: %zu\n", (da), (da)->capacity, (da)->count); \
    } while(0)

//TODO_DA: remove, insert, bulk append


/* Linked List */
#define CELP_LL(dtype) \
typedef struct LLN_##dtype##_t{ \
    dtype data; \
    struct LLN_##dtype##_t* prev; \
    struct LLN_##dtype##_t* next; \
}LLN_##dtype##_t; \
\
typedef struct { \
    LLN_##dtype##_t* head; \
    LLN_##dtype##_t* tail; \
    size_t count; \
} LL_##dtype##_t;

#define __celp_create_node(ll, x, p, n) ({ \
    typeof((ll)->head) __node = CELP_MALLOC(sizeof(*((ll)->head))); \
    __node->data = (x); \
    __node->prev = (p); \
    __node->next = (n); \
    \
    __node; \
})

#define celp_ll_init(ll) \
    do { \
        \
        typeof(((ll)->head)->data) __x_null = {0}; \
        (ll)->head = __celp_create_node((ll), __x_null, NULL, NULL); \
        (ll)->tail = __celp_create_node((ll), __x_null, NULL, NULL); \
        (ll)->head->next = (ll)->tail; \
        (ll)->tail->prev = (ll)->head; \
        (ll)->count = 0; \
    } while(0)

#define celp_ll_is_empty(ll) ((ll)->count == 0)

#define celp_ll_get_first(ll) ((ll)->head->next->data)

#define celp_ll_get_last(ll) ((ll)->tail->prev->data)

#define celp_ll_add_after(ll, x, n) \
    do { \
        typeof((ll)->head) __node = __celp_create_node((ll), (x), (n), (n)->next); \
        __node->prev->next = __node; \
        __node->next->prev = __node; \
        (ll)->count++; \
    } while(0)

#define celp_ll_add_first(ll, x) \
    do { \
        celp_ll_add_after((ll), (x), (ll)->head); \
    } while(0)

#define celp_ll_add_last(ll, x) \
    do { \
        celp_ll_add_after((ll), (x), (ll)->tail->prev); \
    } while(0)

#define celp_ll_add celp_ll_add_last

#define celp_ll_remove_first(ll) ({ \
        CELP_ASSERT((ll)->count > 0); \
        typeof((ll)->head) __to_remove = (ll)->head->next; \
        typeof((ll)->head->data) __return = __to_remove->data; \
        (ll)->head->next->next->prev = (ll)->head; \
        (ll)->head->next = (ll)->head->next->next; \
        CELP_FREE(__to_remove); \
        (ll)->count--; \
        \
        __return; \
    })

#define celp_ll_remove_last(ll) ({ \
        CELP_ASSERT((ll)->count > 0); \
        typeof((ll)->tail) __to_remove = (ll)->tail->prev; \
        typeof((ll)->head->data) __return = __to_remove->data; \
        (ll)->tail->prev->prev->next = (ll)->tail; \
        (ll)->tail->prev = (ll)->tail->prev->prev; \
        CELP_FREE(__to_remove); \
        (ll)->count--; \
        \
        __return; \
    })

#define celp_ll_remove_at_index(ll, i) ({ \
        CELP_ASSERT((i) >= 0 && (i) < (ll)->count && (ll)->count > 0); \
        typeof((ll)->head) __curr = (ll)->head; \
        typeof((ll)->head->data) __return = {0}; \
        for (size_t __i = 0; __i <= (i); __i++) { \
            __curr = __curr->next; \
            /* celp_log(CELP_LOG_LEVEL_DEBUG, "current: %i", __curr->data); */ \
        } \
        __curr->next->prev = __curr->prev; \
        __curr->prev->next = __curr->next; \
        __return = __curr->data; \
        CELP_FREE(__curr); \
        (ll)->count--; \
        \
        __return; \
    })

#define celp_ll_remove_node(ll, n) ({ \
    CELP_ASSERT((ll)->count > 0); \
    typeof((ll)->head) __curr = (ll)->head->next; \
    typeof((ll)->head->data) __return = {0}; \
    for (size_t __i = 0; __i < (ll)->count; __i++) { \
        if (__curr == (n)) { \
            break; \
        } \
        __curr = __curr->next; \
    } \
    if (__curr == (n)) { \
        __curr->next->prev = __curr->prev; \
        __curr->prev->next = __curr->next; \
        __return = __curr->data; \
        CELP_FREE(__curr); \
        (ll)->count--; \
    } else { \
        celp_log(CELP_LOG_LEVEL_INFO, "Failed to find and remove node"); \
    } \
    \
    __return; \
})

#define celp_ll_free(ll) \
    do { \
        typeof((ll)->head) __curr = (ll)->head->next; \
        while (__curr != (ll)->tail) { \
            typeof((ll)->head) __next = __curr->next; \
            CELP_FREE(__curr); \
            __curr = __next; \
        } \
        CELP_FREE((ll)->head); \
        CELP_FREE((ll)->tail); \
        (ll)->head = NULL; \
        (ll)->tail = NULL; \
        (ll)->count = 0; \
    } while(0)

#define celp_ll_print_int(ll) \
    do { \
        typeof((ll)->head) __curr = (ll)->head->next; \
        for (size_t __i = 0; __i < (ll)->count; __i++) { \
            celp_log(CELP_LOG_LEVEL_INFO, "[%zu] %i", __i, __curr->data); \
            __curr = __curr->next; \
        } \
    } while(0)

#define celp_ll_info(ll) \
    do { \
        celp_log(CELP_LOG_LEVEL_INFO, "LL at: %p, Count: %zu", (ll), (ll)->count); \
    } while(0)

/* HashMap */
#define CELP_MAP_INITIAL_CAPACITY 64

#define CELP_MAP(key_dtype, value_dtype) \
typedef struct { \
    key_dtype key; \
    value_dtype value; \
} KV_##key_dtype##_##value_dtype; \
\
CELP_LL(KV_##key_dtype##_##value_dtype) \
\
typedef struct { \
    LL_KV_##key_dtype##_##value_dtype##_t* buckets; \
    size_t count; \
    size_t capacity; \
}  Map_##key_dtype##_##value_dtype##_t;


#define __celp_map_clear(map) \
    do {\
        (map)->buckets = NULL; \
        (map)->count = 0; \
        (map)->capacity = 0; \
    } while(0)

// #define celp_map_init(map) \
//     do{ \
//         __celp_map_clear((map)); \
//         (map)->capacity = CELP_MAP_INITIAL_CAPACITY; \
//         (map)->items = CELP_CALLOC((map)->capacity, sizeof((map)->items[0])); \
//     } while(0)

#define celp_map_init(map) \
    do { \
        __celp_map_clear((map)); \
        (map)->capacity = CELP_MAP_INITIAL_CAPACITY; \
        (map)->buckets = CELP_CALLOC((map)->capacity, sizeof((map)->buckets[0])); \
        for (size_t __i = 0; __i < (map)->capacity; __i++) { \
            celp_ll_init(&((map)->buckets[__i])); \
        } \
    } while(0)

//djb2 hash alg
#define celp_hash(buffer, buffer_size) ({ \
    uint32_t __hash = 5381; \
    for (size_t __i = 0; __i < buffer_size; __i++) { \
        __hash = ((__hash << 5) + __hash) + (uint32_t)buffer[__i]; \
    } \
    __hash; \
})

#define celp_map_insert(map, k, v) \
    do { \
        typeof((map)->buckets[0].head->data.key) __k = (k); \
        const unsigned char* __k_bytes = (const unsigned char*)&(__k); \
        uint32_t __h = celp_hash(__k_bytes, sizeof(__k)) % (map)->capacity; \
        typeof((map)->buckets[0].head->data) __kv = { .key = (__k), .value = (v) }; \
        celp_ll_add(&((map)->buckets[__h]), __kv); \
        (map)->count++; \
    } while(0)

#define celp_map_get(map, k, default_value) ({ \
    typeof((map)->buckets[0].head->data.key) __k = (k); \
    typeof((map)->buckets[0].head->data.value) __return = (default_value); \
    if ((map)->buckets != NULL && (map)->capacity > 0) { \
        const unsigned char* __k_bytes = (const unsigned char*)&(__k); \
        uint32_t __h = celp_hash(__k_bytes, sizeof(__k)) % (map)->capacity; \
        typeof((map)->buckets[0].head) __curr = (map)->buckets[__h].head->next; \
        while(__curr != (map)->buckets[__h].tail) { \
            if (celp_compare(__curr->data.key, __k) == 0) { \
                __return = __curr->data.value; \
                break; \
            } \
            __curr = __curr->next; \
        } \
    } \
    __return; \
})

#define celp_map_contains(map, k) ({ \
    typeof((map)->buckets[0].head->data.key) __k = (k); \
    bool __found = false; \
    if ((map)->buckets != NULL && (map)->capacity > 0) { \
        const unsigned char* __k_bytes = (const unsigned char*)&(__k); \
        uint32_t __h = celp_hash(__k_bytes, sizeof(__k)) % (map)->capacity; \
        typeof((map)->buckets[0].head) __curr = (map)->buckets[__h].head->next; \
        while(__curr != (map)->buckets[__h].tail) { \
            if (celp_compare(__curr->data.key, __k) == 0) { \
                __found = true; \
                break; \
            } \
            __curr = __curr->next; \
        } \
    } \
    __found; \
})

#define celp_map_remove(map, k) ({ \
    typeof((map)->buckets[0].head->data.key) __k = (k); \
    typeof((map)->buckets[0].head->data.value) __return = {0}; \
    if ((map)->buckets != NULL && (map)->capacity > 0) { \
        const unsigned char* __k_bytes = (const unsigned char*)&(__k); \
        uint32_t __h = celp_hash(__k_bytes, sizeof(__k)) % (map)->capacity; \
        typeof((map)->buckets[0].head) __curr = (map)->buckets[__h].head->next; \
        while(__curr != (map)->buckets[__h].tail) { \
            if (celp_compare(__curr->data.key, __k) == 0) { \
                __return = __curr->data.value; \
                celp_ll_remove_node((map)->buckets[__h], __curr); \
                break; \
            } \
            __curr = __curr->next; \
        } \
    } \
    __return; \
})

#define celp_map_free(map) \
    do { \
        if ((map)->buckets != NULL) { \
            for (size_t __i = 0; __i < (map)->capacity; __i++) { \
                celp_ll_free(&((map)->buckets[__i])); \
            } \
            CELP_FREE((map)->buckets); \
        } \
        __celp_map_clear((map)); \
    } while(0)

#define celp_map_info(map) \
    do { \
        celp_log(CELP_LOG_LEVEL_INFO, "Map at: %p, Capacity: %zu, Count: %zu", (map), (map)->capacity, (map)->count); \
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
            case CELP_LOG_LEVEL_DEBUG:
                out = stdout;
                tag = "[DEBUG] ";
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
    #define DA CELP_DA
    #define da_init celp_da_init
    #define da_reserve celp_da_reserve
    #define da_append celp_da_append
    #define da_last celp_da_last
    #define da_pop celp_da_pop
    #define da_remove celp_da_remove
    #define da_foreach celp_da_foreach
    #define da_free celp_da_free
    #define da_info celp_da_info
    //CELP_LL
    #define LL CELP_LL
    #define ll_init celp_ll_init
    #define ll_add celp_ll_add
    #define ll_add_first celp_ll_add_first
    #define ll_add_last celp_ll_add_last
    #define ll_add celp_ll_add
    #define ll_remove_first celp_ll_remove_first
    #define ll_remove_last celp_ll_remove_last
    #define ll_remove_at_index celp_ll_remove_at_index
    #define ll_info celp_ll_info
    #define ll_free celp_ll_free
    #define ll_print_int celp_ll_print_int
    //CELP_MAP
    #define KV CELP_KV
    #define MAP CELP_MAP
    #define map_init celp_map_init
    #define map_set celp_map_set
    #define map_add celp_map_add
    #define map_get celp_map_get
    #define map_remove celp_map_remove
    #define map_free celp_map_free
    #define map_info celp_map_info

#endif //CELP_STRIP_PREFIX

#endif //CELP_H
