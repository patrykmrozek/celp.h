/*
 * celp.h - single header c util library.
 */

#ifndef _CELP_H
#define _CELP_H

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
#include <stdint.h>

#define celp_f32   float
#define celp_f64   double
#define celp_u8    uint8_t
#define celp_u16   uint16_t
#define celp_u32   uint32_t
#define celp_u64   uint64_t
#define celp_i8    int8_t
#define celp_i16   int16_t
#define celp_i32   int32_t
#define celp_i64   int64_t
#define celp_usize size_t
#define celp_isize ssize_t

/* Misc */
#define CELP_COMP(a, b) \
    memcmp(&(a), &(b), sizeof(a))

#define CELP_SWAP(a, b) \
    do { \
        typeof((a)) _tmp = (a); \
        (a) = (b); \
        (b) = _tmp; \
    } while (0);

#define _CELP_CAT(a, b) a##b
#define CELP_CAT(a, b) _CELP_CAT(a, b)

#define _CELP_S(prefix)   CELP_CAT(prefix, _s)
#define _CELP_T(prefix)   CELP_CAT(prefix, _t)
#define _CELP_E(prefix)   CELP_CAT(prefix, _e)

//djb2 hash alg
#define CELP_HASH(buffer, buffer_size) \
    ({ \
        uint32_t _hash = 5381; \
        for (size_t _i = 0; _i < buffer_size; _i++) { \
            _hash = ((_hash << 5) + _hash) + (uint32_t)buffer[_i]; \
        } \
        _hash; \
    })

/* generic typedefs */

/* Dynamic Array */
#define _da(T) da_##T
#define CELP_DA(T) \
    typedef struct _CELP_S(_da(T)) { \
        T* items; \
        celp_usize count; \
        celp_usize capacity; \
    } _CELP_T(_da(T));
#define CELP_DA_T(T) _CELP_T(_da(T)) 

/* Linked List */
#define _lln(T) lln_##T
#define _ll(T)  ll_##T
#define CELP_LL(T) \
    typedef struct _CELP_S(_lln(T)) { \
        T data; \
        struct _CELP_S(_lln(T))* prev; \
        struct _CELP_S(_lln(T))* next; \
    } _CELP_T(_lln(T)); \
    \
    typedef struct _CELP_S(_ll(T)) { \
        _CELP_T(_lln(T))* head; \
        _CELP_T(_lln(T))* tail; \
        celp_usize count; \
    } _CELP_T(_ll(T));
#define CELP_LLN_T(T) _CELP_T(_lln(T))
#define CELP_LL_T(T)  _CELP_T(_ll(T))

/* HashMap */
#define _kv(T)  kv_##kT##_##vT
#define _map(T) map_##kT##_##vT
#define CELP_MAP(kT, vT) \
    typedef struct _CELP_S(_kv(T)) { \
        kT key; \
        vT value; \
    } _CELP_T(_kv(T)); \
    \
    CELP_LL(_CELP_T(_kv(T))); \
    \
    typedef struct _CELP_S(_map(T)) { \
        CELP_LL_T(_CELP_T(_kv(T)))* buckets; \
        celp_usize count; \
        celp_usize capacity; \
    }  _CELP_T(_map(T));
#define CELP_KV_T(kT, vT)  _CELP_T(_kv(T)) 
#define CELP_MAP_T(kT, vT) _CELP_T(_map(T))

/* Testing */
#define CELP_EXPECT(cond) do { \
    if (!(cond)) { \
        celp_test_result = CELP_TEST_RESULT_FAIL; \
        CELP_ERROR("TESTCASE FAILURE: %s", #cond); \
    } \
} while(0)
#define CELP_EXPECT_EQ(x, y) CELP_EXPECT(x==y)
#define CELP_EXPECT_NEQ(x, y) CELP_EXPECT(x!=y)

typedef enum celp_test_result_e {
    CELP_TEST_RESULT_PASS,
    CELP_TEST_RESULT_FAIL,
    CELP_TEST_RESULT_NONE,
} celp_test_result_t;

static celp_test_result_t celp_test_result = CELP_TEST_RESULT_NONE;
static celp_u32 celp_test_runs = 0;
static celp_u32 celp_test_passes = 0;
static celp_u32 celp_test_fails = 0;

typedef struct celp_testcase_s {
    char *name;
    void (*testcase)(void);
    celp_test_result_t result;
} celp_testcase_t;

CELP_LL(celp_testcase_t);
typedef struct celp_test_suite_s {
    //struct celp_test_suite_s *suites;
    char *name;
    void (*setup)(void);
    void (*teardown)(void);
    CELP_LL_T(celp_testcase_t) tests;
} celp_test_suite_t;

#define CELP_TESTCASE(t)      void test_##t()
#define CELP_TEST_SETUP(s)    void setup_##s()
#define CELP_TEST_TEARDOWN(t) void teardown_##t()

#define CELP_TEST_SUITE_START(s) \
    CELP_DEF_SI celp_test_suite_t *suite_##s() { \
        celp_test_suite_t *_suite = CELP_MALLOC(sizeof(celp_test_suite_t)); \
        _suite->name = #s; \
        _suite->setup = NULL; \
        _suite->teardown = NULL; \
        celp_ll_init(&_suite->tests); \
        
#define CELP_TEST_SUITE_ADD_SETUP(s) \
        _suite->setup = &(setup_##s);

#define CELP_TEST_SUITE_ADD_TEARDOWN(t) \
        _suite->teardown = &(teardown_##t);

#define CELP_TEST_SUITE_ADD_TEST(t) \
        celp_testcase_t _t; \
        _t.name = #t; \
        _t.testcase = &test_##t; \
        _t.result = CELP_TEST_RESULT_NONE; \
        celp_ll_add(&_suite->tests, _t);

#define CELP_TEST_SUITE_END() \
        return _suite; \
    }

#define CELP_TEST_SUITE_RUN(s) \
    celp_test_suite_t *_suite = suite_##s(); \
    _suite->setup(); \
    celp_ll_foreach(&_suite->tests, _t) { \
        celp_test_result = CELP_TEST_RESULT_NONE; \
        _t->data.testcase(); \
        celp_test_runs++; \
        _t->data.result = celp_test_result; \
        (celp_test_result==CELP_TEST_RESULT_FAIL) ? \
        celp_test_fails++ : celp_test_passes++; \
    } \
    _suite->teardown(); \

#define CELP_TEST_SUITE_REPORT() \
    celp_log(0, CELP_LOG_INFO,  "[TEST_SUITE] ", "%s", _suite->name); \
    celp_ll_foreach(&_suite->tests, _t) { \
        celp_log(0, CELP_LOG_INFO,  "\t[TESTCASE] ", "%s %s", _t->data.name, \
                (celp_test_result==CELP_TEST_RESULT_FAIL) ? "[FAIL]" : "[PASS]"); \
    } \
    celp_log(0, CELP_LOG_INFO,  "[REPORT] ", "RUNS: %d - PASSED: %d - FAILED: %d\n", \
            celp_test_runs, celp_test_passes, celp_test_fails); \

#define CELP_TEST_SUITE_DESTROY() \
    celp_ll_free(&_suite->tests); \
    CELP_FREE(_suite); \


/* Logging */
typedef enum celp_log_e {
   _CELP_LOG_INFO,
   _CELP_LOG_DEBUG,
   _CELP_LOG_ERROR,
   _CELP_LOG_TRACE,
} celp_log_t;

//sort of implemented function overloading?
//user uses these fake enums below, which automatically fill
//out the (file, function, line) parameters in celp_log based 
//on the log level
#define CELP_LOG_INFO  (celp_log_t)_CELP_LOG_INFO,  NULL, NULL, 0
#define CELP_LOG_DEBUG (celp_log_t)_CELP_LOG_DEBUG, NULL, NULL, 0
#define CELP_LOG_ERROR (celp_log_t)_CELP_LOG_ERROR, \
                                               __FILE__, __FUNCTION__, __LINE__
#define CELP_LOG_TRACE (celp_log_t)_CELP_LOG_TRACE, \
                                               __FILE__, __FUNCTION__, __LINE__

CELP_DEF void celp_log(celp_u8 level,
                       celp_log_t log,
                       const char *file,
                       const char *function,
                       int line,
                       const char *tag,
                       const char* fmt_string,
                       ...);

#define CELP_INFO(fmt, ...)       celp_log(0,   CELP_LOG_INFO,  "[INFO] ",  fmt, ##__VA_ARGS__)
#define CELP_DEBUG(lvl, fmt, ...) celp_log(lvl, CELP_LOG_DEBUG, "[DEBUG] ", fmt, ##__VA_ARGS__)
#define CELP_ERROR(fmt, ...)      celp_log(0,   CELP_LOG_ERROR, "[ERROR] ", fmt, ##__VA_ARGS__)
#define CELP_TRACE(lvl, fmt, ...) celp_log(lvl, CELP_LOG_TRACE, "[TRACE] ", fmt, ##__VA_ARGS__)

/* Dynamic Array */
#define CELP_DA_INITIAL_CAPACITY 256

#define celp_da_init(da) \
    do { \
        (da)->items = NULL;\
        (da)->count = 0;\
        (da)->capacity = 0;\
    } while(0)

#define celp_da_clear(da)    ((da)->count = 0)
#define celp_da_is_empty(da) ((da)->count == 0)

#define celp_da_reserve(da, expected_capacity) \
    do {\
        if ((expected_capacity) > (da)->capacity) {\
            if ((da)->capacity == 0) {\
                (da)->capacity = CELP_DA_INITIAL_CAPACITY;\
            }\
            while ((da)->capacity < expected_capacity) {\
                (da)->capacity *= 2;\
            }\
            (da)->items = CELP_REALLOC((da)->items, \
                                       (da)->capacity * sizeof((da)->items[0]));\
            CELP_ASSERT((da)->items != NULL);\
            CELP_ERROR("realloc failed"); \
        }\
    } while(0)

#define celp_da_append(da, item) \
    do {\
        celp_da_reserve(da, (da)->count + 1);\
        (da)->items[(da)->count++] = (item);\
    } while(0)

#define celp_da_last(da) \
    ({ \
        CELP_ASSERT((da)->count > 0); \
        (da)->items[(da)->count-1]; \
    })

#define celp_da_pop(da) \
    ({ \
        CELP_ASSERT((da)->count > 0); \
        (da)->items[--(da)->count]; \
    })

#define celp_da_remove(da, idx) \
    ({ \
        CELP_ASSERT(idx < (da)->count); \
        typeof((da)->items[0]) _temp = (da)->items[(idx)]; \
        (da)->items[(idx)] = celp_da_last((da)); \
        (da)->items[(da)->count-1] = _temp; \
        celp_da_pop(da); \
    })

// user provides some label (i) -> macro initializes it as a pointer to (da)->items
// elements can be accessed by dereferencing (i)
// to get index 0, suntract (i) from (da)->items
#define celp_da_foreach(da, i) \
    for (typeof(*(da)->items)* (i) = (da)->items; \
         (i) < (da)->items + (da)->count; \
         (i)++)

#define celp_da_free(da) \
    do { \
        CELP_ASSERT((da)); \
        CELP_FREE((da)->items); \
        celp_da_init(da); \
    } while(0)

#define celp_da_info(da) \
    do{ \
        CELP_INFO("Dynamic Array at: %p, Capacity: %zu, Count: %zu\n", \
             (da), (da)->capacity, (da)->count); \
    } while(0)


/* Linked List*/

#define _celp_ll_create_node(ll, x, p, n) \
    ({ \
        typeof((ll)->head) _node = CELP_MALLOC(sizeof(*((ll)->head))); \
        if (!_node) CELP_ERROR("node malloc failed"); \
        _node->data = (x); \
        _node->prev = (p); \
        _node->next = (n); \
        \
        _node; \
    })

#define celp_ll_init(ll) \
    do { \
        typeof(((ll)->head)->data) _x_null = {0}; \
        (ll)->head = _celp_ll_create_node((ll), _x_null, NULL, NULL); \
        (ll)->tail = _celp_ll_create_node((ll), _x_null, NULL, NULL); \
        (ll)->head->next = (ll)->tail; \
        (ll)->tail->prev = (ll)->head; \
        (ll)->count = 0; \
    } while(0)

#define celp_ll_is_empty(ll) ((ll)->count == 0)

#define celp_ll_foreach(ll, iter) \
    for (typeof((ll)->head) iter = (ll)->head->next; \
        iter != (ll)->tail; \
        iter = iter->next)

#define celp_ll_foreach_until_node(ll, iter, n) \
    for (typeof((ll)->head) iter = (ll)->head->next; \
        iter != (n); \
        iter = iter->next)

#define celp_ll_get_first(ll) \
    ({ \
        CELP_ASSERT((ll)->count > 0); \
        (ll)->head->next->data \
    })

#define celp_ll_get_last(ll) \
    ({ \
        CELP_ASSERT((ll)->count > 0); \
        (ll)->tail->prev->data \
    })

#define celp_ll_add_after(ll, x, n) \
    do { \
        typeof((ll)->head) _node = _celp_ll_create_node((ll), (x), (n), (n)->next); \
        _node->prev->next = _node; \
        _node->next->prev = _node; \
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

#define celp_ll_remove_first(ll) \
    ({ \
        CELP_ASSERT((ll)->count > 0); \
        typeof((ll)->head) _to_remove = (ll)->head->next; \
        typeof((ll)->head->data) _return = _to_remove->data; \
        (ll)->head->next->next->prev = (ll)->head; \
        (ll)->head->next = (ll)->head->next->next; \
        CELP_FREE(_to_remove); \
        (ll)->count--; \
        \
        _return; \
    })

#define celp_ll_remove_last(ll) \
    ({ \
        CELP_ASSERT((ll)->count > 0); \
        typeof((ll)->tail) _to_remove = (ll)->tail->prev; \
        typeof((ll)->head->data) _return = _to_remove->data; \
        (ll)->tail->prev->prev->next = (ll)->tail; \
        (ll)->tail->prev = (ll)->tail->prev->prev; \
        CELP_FREE(_to_remove); \
        (ll)->count--; \
        \
        _return; \
    })

#define celp_ll_remove_at_index(ll, i) \
    ({ \
        CELP_ASSERT((i) >= 0 && (i) < (ll)->count && (ll)->count > 0); \
        typeof((ll)->head) _curr = (ll)->head; \
        typeof((ll)->head->data) _return = {0}; \
        for (size_t _i = 0; _i <= (i); _i++) { \
            _curr = _curr->next; \
        } \
        _curr->next->prev = _curr->prev; \
        _curr->prev->next = _curr->next; \
        _return = _curr->data; \
        CELP_FREE(_curr); \
        (ll)->count--; \
        \
        _return; \
    })

#define celp_ll_remove_node(ll, n) \
    ({ \
    CELP_ASSERT((ll)->count > 0); \
    typeof((ll)->head->data) _return = {0}; \
    bool _found = false; \
    celp_ll_foreach((ll), _curr) { \
        if (_curr == (n)) { \
            _curr->next->prev = _curr->prev; \
            _curr->prev->next = _curr->next; \
            _return = _curr->data; \
            CELP_FREE(_curr); \
            (ll)->count--; \
            _found = true; \
            break; \
        } \
    } \
    if (!_found) { \
        CELP_ERROR("Failed to find and remove node"); \
    } \
    \
    _return; \
})

#define celp_ll_free(ll) \
    do { \
        CELP_ASSERT((ll)); \
        typeof((ll)->head) _curr = (ll)->head->next; \
        while (_curr != (ll)->tail) { \
            typeof((ll)->head) _next = _curr->next; \
            CELP_FREE(_curr); \
            _curr = _next; \
        } \
        CELP_FREE((ll)->head); \
        CELP_FREE((ll)->tail); \
        (ll)->head = NULL; \
        (ll)->tail = NULL; \
        (ll)->count = 0; \
    } while(0)

#define celp_ll_print_int(ll) \
    do { \
        typeof((ll)->head) _curr = (ll)->head->next; \
        for (size_t _i = 0; _i < (ll)->count; _i++) { \
            CELP_INFO("[%zu] %i",_i , _curr->data); \
            _curr = _curr->next; \
        } \
    } while(0)

#define celp_ll_info(ll) \
    do { \
        CELP_INFO("LL at: %p, Count: %zu", (ll), (ll)->count); \
    } while(0)


/* HashMap */
#define CELP_MAP_INITIAL_CAPACITY 64

#define _celp_map_clear(map) \
    do {\
        (map)->buckets = NULL; \
        (map)->count = 0; \
        (map)->capacity = 0; \
    } while(0)

#define celp_map_init(map) \
    do { \
        _celp_map_clear((map)); \
        (map)->capacity = CELP_MAP_INITIAL_CAPACITY; \
        (map)->buckets = CELP_CALLOC((map)->capacity, sizeof((map)->buckets[0])); \
        for (size_t _i = 0; _i < (map)->capacity; _i++) { \
            celp_ll_init(&((map)->buckets[_i])); \
        } \
    } while(0)

#define _celp_map_init_k_and_hash(map) \
    typeof((map)->buckets[0].head->data.key) _k = (k); \
    const unsigned char* _k_bytes = (const unsigned char*)&(_k); \
    uint32_t _h = CELP_HASH(_k_bytes, sizeof(_k)) % (map)->capacity; \

#define celp_map_is_empty(map) ((map)->count == 0)

#define celp_map_insert(map, k, v) \
    do { \
        _celp_map_init_k_and_hash(map) \
        bool _found = false; \
        celp_ll_foreach(&(map)->buckets[_h], _bucket) {\
            if (CELP_COMP(_bucket->data.key, _k) == 0) { \
                _bucket->data.value = (v); \
                _found = true; \
                break; \
            } \
        } \
        if (!_found) { \
            typeof((map)->buckets[_h].head->data) _kv = \
                { .key = (_k), .value = (v) }; \
            celp_ll_add(&((map)->buckets[_h]), _kv); \
            (map)->count++; \
        } \
    } while(0)

// assumes the value can be incremented
// if the key isnt already in the map it assigns value 1
#define celp_map_increment(map, k) \
    do { \
        _celp_map_init_k_and_hash(map) \
        bool _found = false; \
        celp_ll_foreach(&(map)->buckets[_h], _bucket) { \
            if (CELP_COMP(_bucket->data.key, _k) == 0) { \
                _bucket->data.value++; \
                _found = true; \
                break; \
            } \
        } \
        if (!_found) { \
            typeof((map)->buckets[_h].head->data) _kv = \
                { .key = (_k), .value = 1 }; \
            celp_ll_add(&((map)->buckets[_h]), _kv); \
            (map)->count++; \
        } \
    } while(0)

#define celp_map_get(map, k, default_value) \
    ({ \
        typeof((map)->buckets[0].head->data.value) _return = (default_value); \
        if ((map)->buckets != NULL && (map)->capacity > 0) { \
            _celp_map_init_k_and_hash(map) \
            celp_ll_foreach(&(map)->buckets[_h], _bucket) { \
                if (CELP_COMP(_bucket->data.key, _k) == 0) { \
                    _return = _bucket->data.value; \
                    break; \
                } \
            } \
        } \
        _return; \
    })

#define celp_map_contains(map, k) \
    ({ \
        bool _found = false; \
        if ((map)->buckets != NULL && (map)->capacity > 0) { \
            _celp_map_init_k_and_hash(map) \
            celp_ll_foreach(&(map)->buckets[_h], _bucket) { \
                if (CELP_COMP(_bucket->data.key, _k) == 0) { \
                    _found = true; \
                    break; \
                } \
            } \
        } \
        _found; \
    })

#define celp_map_remove(map, k) \
    ({ \
        CELP_ASSERT((map)->count > 0); \
        typeof((map)->buckets[0].head->data.value) _return = {0}; \
        if ((map)->buckets != NULL && (map)->capacity > 0) { \
            _celp_map_init_k_and_hash(map) \
            celp_ll_foreach(&(map)->buckets[_h], _bucket) { \
                if (CELP_COMP(_bucket->data.key, _k) == 0) { \
                    _return = _bucket->data.value; \
                    celp_ll_remove_node(&((map)->buckets[_h]), _bucket); \
                    (map)->count--; \
                    break; \
                } \
            } \
        } \
        _return; \
    })

#define celp_map_free(map) \
    do { \
        if ((map)->buckets != NULL) { \
            for (size_t _i = 0; _i < (map)->capacity; _i++) { \
                celp_ll_free(&((map)->buckets[_i])); \
            } \
            CELP_FREE((map)->buckets); \
        } \
        _celp_map_clear((map)); \
    } while(0)

#define celp_map_info(map) \
    do { \
        CELP_INFO("Map at: %p, Capacity: %zu, Count: %zu", \
             (map), (map)->capacity, (map)->count); \
    } while(0)


//math macros
#ifdef CELP_MATH

/* Vector2 */
#define _v2(T) v2_##T
#define CELP_V2(T) \
    typedef struct _CELP_S(_v2(T)){ \
        T x, y; \
    } _CELP_T(_v2(T));
#define CELP_V2_T(T) _CELP_T(_v2(T))

#define celp_v2_add(v1, v2) \
({ \
    typeof((v1)) _v_out = { \
        .x = (v1).x + (v2).x, \
        .y = (v1).y + (v2).y \
    }; \
    _v_out; \
})

#define celp_v2_sub(v1, v2) \
({ \
    typeof((v1)) _v_out = { \
        .x = (v1).x - (v2).x, \
        .y = (v1).y - (v2).y \
    }; \
    _v_out; \
})

#define celp_v2_dot(v1, v2) ((v1).x * (v2).x) + ((v1).y * (v2).y)

#define celp_v2_cross(v1, v2) ((v1).x * (v2).y) - ((v1).y * (v2).x)

#define celp_v2_scale(v, s) \
({ \
    typeof((v)) _v_out = { \
        .x = (v).x * s, \
        .y = (v).y * s \
    }; \
    _v_out; \
})

/* Vector3 */
#define _v3(T) v3_##T
#define CELP_V3(T) \
    typedef struct _CELP_S(_v3(T)) { \
        T x, y, z; \
    } _CELP_T(_v3(T));
#define CELP_V3_T(T) _CELP_T(_v3(T))

#define CELP_V3F_STR(v) "{ %f, %f, %f }", (v).x, (v).y, (v).z

#define celp_v3_contains_neg(v) (v.x < 0 || v.y < 0 || v.z < 0)

#define celp_v3_is_empty(v) \
    ((v).x == 0 && \
     (v).y == 0 && \
     (v).z == 0)

#define celp_v3_contains_zero(v) \
    ((v).x == 0 || \
     (v).y == 0 || \
     (v).z == 0)

#define celp_v3_add(v1, v2) \
({ \
   typeof((v1)) _v_out = { \
       .x = (v1).x + (v2).x, \
       .y = (v1).y + (v2).y, \
       .z = (v1).z + (v2).z\
   }; \
   _v_out; \
})

#define celp_v3_sub(v1, v2) \
({ \
    typeof((v1)) _v_out = { \
        .x = (v1).x - (v2).x, \
        .y = (v1).y - (v2).y, \
        .z = (v1).z - (v2).z\
    }; \
    _v_out; \
})

#define celp_v3_mul(v1, v2) \
({ \
    typeof((v1)) _v_out = { \
        .x = (v1).x * (v2).x, \
        .y = (v1).y * (v2).y, \
        .z = (v1).z * (v2).z\
    }; \
    _v_out; \
})

#define celp_v3_div(v1, v2) \
({ \
    CELP_ASSERT(!celp_v3_contains_zero((v2))); \
    typeof((v1)) _v_out = { \
        .x = (v1).x / (v2).x, \
        .y = (v1).y / (v2).y, \
        .z = (v1).z / (v2).z\
    }; \
    _v_out; \
})

#define celp_v3_dot(v1, v2) \
({ \
    typeof((v1).x) _ret = ((v1).x * (v2).x) + \
                          ((v1).y * (v2).y) + \
                          ((v1).z * (v2).z);  \
    _ret; \
 })

#define celp_v3_scale(v, s) \
({ \
    typeof((v)) _v_out = { \
        .x = (v).x * s, \
        .y = (v).y * s, \
        .z = (v).z * s \
    }; \
    _v_out; \
})

#define celp_v3_cross(v1, v2) \
({ \
    typeof((v1)) _v_out = { \
        .x = ((v1).y * (v2).z) - ((v1).z * (v2).y), \
        .y = ((v1).z * (v2).x) - ((v1).x * (v2).z), \
        .z = ((v1).x * (v2).y) - ((v1).y * (v2).x) \
    }; \
    _v_out; \
})

#define celp_v3_len(v) \
    sqrtf(celp_v3_dot((v), (v)))

#define celp_v3_norm(v) \
({ \
    typeof((v)) _v_out = {0}; \
    float len = celp_v3_len((v));  \
    typeof((v)) _v_len = {len, len, len}; \
    _v_out = celp_v3_div((v), _v_len); \
    _v_out; \
})

#define celp_v3_neg(v) {-(v).x, -(v).y, -(v).z};

/* Vector4 */
#define _v4(T) v4_##T
#define CELP_V4(T) \
    typedef struct _CELP_S(_v4(T)) { \
        T x, y, z, w; \
    } _CELP_T(_v4(T));
#define CELP_V4_T(T) _CELP_T(_v4(T))

#define CELP_V4F_STR(v) "{ %f, %f, %f, %f }", (v).x, (v).y, (v).z, (v).w

#define celp_v4_is_empty(v) \
    ((v).x == 0 && \
     (v).y == 0 && \
     (v).z == 0 && \
     (v).w == 0)

#define celp_v4_contains_zero(v) \
    ((v).x == 0 || \
     (v).y == 0 || \
     (v).z == 0 || \
     (v).w == 0)

#define celp_v3_to_v4(v, T) \
({ \
    CELP_V4_T(T) _v_out = {(v).x, (v).y, (v).z, 1}; \
    _v_out; \
})

#define celp_v4_norm(v) \
({ \
    CELP_ASSERT((v).w != 0); \
    typeof((v)) _v_out = {(v).x/(v).w, (v).y/(v).w, \
                          (v).z/(v).w, (v).w/(v).w}; \
    _v_out; \
})

#define celp_v4_to_v3(v, T) \
({ \
    CELP_V3_T(T) _v_out = {(v).x, (v).y, (v).z}; \
    _v_out; \
})

#define celp_v4_dot(v1, v2) \
({ \
    typeof((v1).x) _out = (((v1).x*(v2).x) + \
                           ((v1).y*(v2).y) + \
                           ((v1).z*(v2).z) + \
                           ((v1).w*(v2).w)); \
    _out; \
})

/* Matrix4 */
#define _m4(T) m4_##T
#define CELP_M4(T) \
    typedef struct _CELP_S(_m4(T)) { \
        T v[4][4]; \
    } _CELP_T(_m4(T));
#define CELP_M4_T(T) _CELP_T(_m4(T))

#define CELP_M4_ID (m4){{   \
    {1, 0, 0, 0}, \
    {0, 1, 0, 0}, \
    {0, 0, 1, 0}, \
    {0, 0, 0, 1}}}

#define CELP_M4_TRANS(t) (m4){{       \
    {1, 0, 0, t.x}, \
    {0, 1, 0, t.y}, \
    {0, 0, 1, t.z}, \
    {0, 0, 0,  1}}}

#define CELP_M4_SCALE(s) (m4){{  \
    {s,     0,     0,   0}, \
    {  0,   s,     0,   0}, \
    {  0,     0,   s,   0}, \
    {  0,     0,    0,   1}}}

#define CELP_M4_SCALEV(s) (m4){{   \
    {s.x,     0,     0,   0}, \
    {  0,   s.y,     0,   0}, \
    {  0,     0,   s.z,   0}, \
    {  0,     0,     0,    1}}} 

#define CELP_M4_ROTX(a) (m4){{     \
    {1,       0,       0, 0}, \
    {0, cos(a), -sin(a), 0}, \
    {0, sin(a), cos(a), 0}, \
    {0,       0,       0, 1}}}

#define CELP_M4_ROTY(a) (m4){{     \
    { cos(a), 0, sin(a), 0}, \
    {       0, 1,        0, 0}, \
    {-sin(a), 0,  cos(a), 0}, \
    {       0, 0,        0, 1}}}

#define CELP_M4_ROTZ(a) (m4){{     \
    {cos(a), -sin(a), 0, 0}, \
    {sin(a),  cos(a), 0, 0}, \
    {       0,       0, 1, 0}, \
    {       0,       0, 0, 1}}}


#define celp_m4_v4_mul(m, _v) \
({ \
    typeof((_v)) __v = (_v); \
    typeof(__v.x) _out[4] = {0}; \
    for (celp_u8 _row_idx = 0; _row_idx < 4; _row_idx++) { \
        typeof(__v) _row = {(m).v[_row_idx][0],  \
                            (m).v[_row_idx][1],  \
                            (m).v[_row_idx][2],  \
                            (m).v[_row_idx][3]}; \
        _out[_row_idx] = celp_v4_dot(_row, __v); \
    } \
    (typeof(__v)){_out[0], _out[1], _out[2], _out[3]}; \
})

#define celp_m4_mul(m1, m2) \
({ \
    typeof((m1)) _out[4] = {0}; \
    for (celp_u8 _i = 0; _i < 4; _i++) { \
        for (celp_u8 _j = 0; _j < 4; _j++) { \
            out.v[_i][_j] = v4_dot( \
                ((v4){(m1).v[_i][0], (m1).v[_i][1], (m1).v[_i][2], (m1).v[_i][3]}), \
                ((v4){(m2).v[0][_j], (m2).v[1][_j], (m2).v[2][_j], (m2).v[3][_j]})); \
        } \
    } \
    _out; \
})

#define celp_v4_trans(v, t) \
    celp_m4_v4_mul(CELP_M4_TRANS((t)), (v))

#define celp_v3_trans(v, t) \
    celp_v4_to_v3(celp_v4_trans(celp_v3_to_v4((v), typeof((v).x)), (t)), typeof((v).x))

#define celp_v4_scale(v, s) \
    celp_m4_v4_mul(CELP_M4_SCALE((s)), (v))

#define celp_v4_scalev(v, s) \
    celp_m4_v4_mul(CELP_M4_SCALEV((s)), (v))

#endif //CELP_MATH



#ifdef CELP_IMPLEMENTATION

// can't apply usual STRIP_PREFIX logic to these flags
// since theyre passed in through stdin theyre technically
// defined already, so to apply STREIP_PREFIX to these, we
// have to first check if the stripped version have been
// defined, then define the CELP equivalent which works on
// celp_log later on..
#ifdef LOG_MODE_ALL
    #define CELP_LOG_MODE_ALL   LOG_MODE_ALL
    #define CELP_LOG_MODE_INFO  LOG_MODE_INFO
    #define CELP_LOG_MODE_DEBUG LOG_MODE_DEBUG
    #define CELP_LOG_MODE_ERROR LOG_MODE_ERROR
    #define CELP_LOG_MODE_TRACE LOG_MODE_TRACE
#else
    #ifdef LOG_MODE_INFO
        #define CELP_LOG_MODE_INFO LOG_MODE_INFO
    #endif 

    #ifdef LOG_MODE_DEBUG
        #define CELP_LOG_MODE_DEBUG LOG_MODE_DEBUG
    #endif 

    #ifdef LOG_MODE_ERROR
        #define CELP_LOG_MODE_ERROR LOG_MODE_ERROR
    #endif 

    #ifdef LOG_MODE_TRACE
        #define CELP_LOG_MODE_TRACE LOG_MODE_TRACE
    #endif 
#endif //LOG_MODE_ALL 
       
#ifndef LOG_LEVEL
    #define LOG_LEVEL 0 
#endif //LOG_LEVEL
#define CELP_LOG_LEVEL LOG_LEVEL

CELP_DEF void 
celp_log(celp_u8 level,
         celp_log_t log,
         const char *file,
         const char *function,
         int line,
         const char *tag,
         const char* fmt_string,
         ...)
{
    if (level > CELP_LOG_LEVEL) return;
    va_list args;
    va_start(args, fmt_string); 
    FILE* out = NULL;
    char fmt_str_trace[256];

    switch(log) {
        case _CELP_LOG_INFO:
            #ifdef CELP_LOG_MODE_INFO
                out = stdout;
                (void)file; (void)function; (void)line;
            #else
                return;
            #endif //CELP_LOG_MODE_INFO
            goto end;
        case _CELP_LOG_ERROR:
            #ifdef CELP_LOG_MODE_ERROR
                out = stderr;
                goto prepend;
            #else
                return;
            #endif //CELP_LOG_MODE_ERROR
        case _CELP_LOG_DEBUG:
            #ifdef CELP_LOG_MODE_DEBUG
                out = stdout;
                (void)file; (void)function; (void)line;
            #else
                return;
            #endif //CELP_LOG_MODE_DEBUG
            goto end;
        case _CELP_LOG_TRACE:
            #ifdef CELP_LOG_MODE_TRACE
                out = stdout;
                goto prepend;
            #else
                return;
            #endif //CELP_LOG_MODE_TRACE
    }

prepend:
    snprintf(fmt_str_trace, 56, "%s:%s:%d\n\t",
             file, function, line);
    strncat(fmt_str_trace, fmt_string, 200);
    fmt_string = fmt_str_trace;
end:
    fputs(tag, out);
    vfprintf(out, fmt_string, args); 
    fputc('\n', out);
    va_end(args);
    return;
}

#endif //CELP_IMPLEMENTATION

//shamelessly ripped from mr tsoding
//if you dont want to keep writing celp :|
#ifdef CELP_STRIP_PREFIX
    //TYPES
    #define f32                   celp_f32
    #define f64                   celp_f64 
    #define u8                    celp_u8 
    #define u16                   celp_u16 
    #define u32                   celp_u32
    #define u64                   celp_u64
    #define i8                    celp_i8
    #define i16                   celp_i16 
    #define i32                   celp_i32 
    #define i64                   celp_i64
    #define usize                 celp_usize 
    #define isize                 celp_isize
    //MISC
    #define INFO                  CELP_INFO 
    #define DEBUG                 CELP_DEBUG 
    #define ERROR                 CELP_ERROR 
    #define TRACE                 CELP_TRACE
    #define COMP                  CELP_COMP 
    #define SWAP                  CELP_SWAP
    #define HASH                  CELP_HASH  
    //CELP_LOG
    #define log                   celp_log
    #define LOG_INFO              CELP_LOG_INFO
    #define LOG_DEBUG             CELP_LOG_DEBUG
    #define LOG_ERROR             CELP_LOG_ERROR 
    #define LOG_TRACE             CELP_LOG_TRACE  
    #define LOG_TEST              CELP_LOG_TEST
    //CELP_DA
    #define DA                    CELP_DA 
    #define DA_T                  CELP_DA_T
    #define da_init               celp_da_init
    #define da_clear              celp_da_clear
    #define da_is_empty           celp_da_is_empty
    #define da_reserve            celp_da_reserve
    #define da_append             celp_da_append
    #define da_last               celp_da_last
    #define da_pop                celp_da_pop
    #define da_remove             celp_da_remove
    #define da_foreach            celp_da_foreach
    #define da_free               celp_da_free
    #define da_info               celp_da_info
    //CELP_LL
    #define LL                    CELP_LL 
    #define LL_T                  CELP_LL_T
    #define ll_init               celp_ll_init
    #define ll_is_empty           celp_ll_is_empty
    #define ll_get_first          celp_ll_get_first
    #define ll_get_last           celp_ll_get_last
    #define ll_add                celp_ll_add
    #define ll_add_first          celp_ll_add_first
    #define ll_add_last           celp_ll_add_last
    #define ll_add                celp_ll_add
    #define ll_remove_first       celp_ll_remove_first
    #define ll_remove_last        celp_ll_remove_last
    #define ll_remove_at_index    celp_ll_remove_at_index
    #define ll_remove_node        celp_ll_remove_node
    #define ll_print_int          celp_ll_print_int
    #define ll_foreach            celp_ll_foreach
    #define ll_foreach_until_node celp_ll_foreach_until_node
    #define ll_free               celp_ll_free
    #define ll_info               celp_ll_info
    //CELP_MAP
    #define KV                    CELP_KV
    #define KV_T                  CELP_KV_T
    #define MAP                   CELP_MAP
    #define MAP_T                 CELP_MAP_T 
    #define map_init              celp_map_init
    #define map_is_empty          celp_map_is_empty
    #define map_insert            celp_map_insert
    #define map_increment         celp_map_increment
    #define map_get               celp_map_get
    #define map_contains          celp_map_contains
    #define map_remove            celp_map_remove
    #define map_free              celp_map_free
    #define map_info              celp_map_info

#ifdef CELP_MATH 
    //v2
    #define V2                    CELP_V2
    #define V2_T                  CELP_V2_T
    #define v2_add                celp_v2_add
    #define v2_sub                celp_v2_sub
    #define v2_dot                celp_v2_dot
    #define v2_cross              celp_v2_cross
    #define v2_scale              celp_v2_scale
    //v3
    #define V3                    CELP_V3
    #define V3_T                  CELP_V3_T
    #define v3_add                celp_v3_add
    #define v3_sub                celp_v3_sub
    #define v3_dot                celp_v3_dot
    #define v3_scale              celp_v3_scale
    #define v3_cross              celp_v3_cross
    #define v3_len                celp_v3_len
    #define v3_norm               celp_v3_norm
    #define v3_neg                celp_v3_neg
    #define v3_contains_neg       celp_v3_contains_neg
    #define v3_contains_zero      celp_v3_contains_zero
    #define v3_is_empty           celp_v3_is_empty
    #define V3F_STR               CELP_V3F_STR

    //v4
    #define V4                    CELP_V4
    #define V4_T                  CELP_V4_T
    #define V4F_STR               CELP_V4F_STR
    #define v3_to_v4              celp_v3_to_v4 
    #define v4_norm               celp_v4_norm
    #define v4_to_v3              celp_v4_to_v3
    #define v4_dot                celp_v4_dot
    //m4
    #define M4                    CELP_M4
    #define M4_T                  CELP_M4_T 
    #define M4_ID                 CELP_M4_ID
    #define M4_TRANS              CELP_M4_TRANS
    #define M4_SCALE              CELP_M4_SCALE
    #define M4_SCALEV             CELP_M4_SCALEV
    #define M4_ROTX               CELP_M4_ROTX
    #define M4_ROTY               CELP_M4_ROTY
    #define M4_ROTZ               CELP_M4_ROTZ 
    #define m4_v4_mul             celp_m4_v4_mul
    #define m4_mul                celp_m4_mul
    //m4 wrappers 
    #define v4_trans              celp_v4_trans
    #define v3_trans              celp_v3_trans
    #define v4_scale              celp_v4_scale 
    #define v4_scalev             celp_v4_scalev

#endif //CELP_MATH
       
#endif //CELP_STRIP_PREFIX
       
#endif //_CELP_H
