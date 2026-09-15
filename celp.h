/*
 * celp.h - single header generic C util library.
 *
 * Provides many useful features like:
 *  celp_log - comprehensive and detailed logger
 *  celp_da - generic dynamic array implementation
 *  celp_ll - generic doubly linked list implementation
 *  celp_map - generic hashmap implementation
 *  CELP_MATH - generic linear algebra
 *  CELP_TEST - a lightweight unit testing framework
 *
 * Additional Features:
 *  CELP_STRIP_PREFIX - if defined before you include celp, it strips every
 *  macro, function, type of the word "celp". An extra thing you can do with
 *  this is, say you strip celp_log() of it's prefix, which leaves you with
 *  "log()", this wil surely clash with an existing log function, and so you 
 *  have the ability of "undefining" the stripped version for specific names
 *  like this.
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

#define _CELP_CATTT(a, b, c) a##b##c
#define CELP_CATTT(a, b, c) _CELP_CATTT(a, b, c)

#define CELP_S(prefix)   CELP_CAT(prefix, _s)
#define CELP_T(prefix)   CELP_CAT(prefix, _t)
#define CELP_E(prefix)   CELP_CAT(prefix, _e)

//djb2 hash alg
#define CELP_HASH(buffer, buffer_size) \
    ({ \
        uint32_t _hash = 5381; \
        for (size_t _i = 0; _i < buffer_size; _i++) { \
            _hash = ((_hash << 5) + _hash) + (uint32_t)buffer[_i]; \
        } \
        _hash; \
    })


/* errors */
typedef enum celp_err_s {
    CELP_ERR_OK,
    CELP_ERR_TYPE,
    CELP_ERR_OOB,
    CELP_ERR_ALLOC,
} celp_err_t;

#ifdef CELP_ERRORS
    #define _celp_err_arg(err) (err)
#else
    #define _celp_err_arg(...) NULL
#endif //CELP_ERRORS

#define _celp_err_clear(err) \
    (*(err) = CELP_ERR_OK)

#define _celp_err_set(err, err_code) \
    (*(err) = (err_code))

#define _celp_err_failed(err) \
    (*(err) != CELP_ERR_OK)

/* macro definintion helpers */
#define _celp_out_label(fname) celp_out_##fname

#define _celp_init(err, fname) \
     __label__ _celp_out_label(fname); \
    celp_err_t *_celp_err_in = (err); \
    celp_err_t _celp_err_local = CELP_ERR_OK; \
    celp_err_t *_celp_err = (_celp_err_in) ? _celp_err_in : \
                                             &_celp_err_local; \
    _celp_err_clear(_celp_err); \

#define _celp_expr_init(return_t, err, fname) \
    _celp_init((err), fname); \
    return_t _celp_return = {0};

#define _celp_stmt_init(err, fname) \
   _celp_init((err), fname); 

#define _celp_goto_out(fname) \
    goto _celp_out_label(fname);

#define _celp_fail(err_code, fname) \
    do { \
        _celp_err_set(_celp_err, (err_code)); \
        _celp_goto_out(fname); \
    } while(0)

#define _celp_require(cond, err_code, fname) \
    do { \
        if (!(cond)) { \
            _celp_fail((err_code), fname); \
        } \
    } while(0)

#define _celp_propogate(fname) \
    do { \
        if (_celp_err_failed(_celp_err)) { \
            _celp_goto_out(fname); \
        } \
    } while(0)

#define _celp_expr_return(fname) \
    _celp_out_label(fname): \
        _celp_return;

#define _celp_stmt_end(fname) \
    _celp_out_label(fname): \
        ((void)0) \

 


/* Logging */
/*
 * celp_log() is designed to be a detailed logger, giving the user absolute 
 * control. Logging can be enabled by adding one or more of the 
 * following compiler flags:
 *     - LOG_MODE_INFO
 *     - LOG_MODE_DEBUG
 *     - LOG_MODE_TRACE
 *     - LOG_MODE_ERROR
 *     - LOG_MODE_ALL
 *
 *celp_log(celp_u8 level,         - log level, the lower the level, the higher
                                    the priority
           celp_log_t log,        - log type
           const char *file,      - __FILE__
           const char *function,  - __FUNCTION__
           celp_u32 line,         - __LINE__
           FILE *out,             - which output to write to (e.g stdout,..)
           char *buff,            - which buffer to write to
           celp_u32 bufflen,      - size of said buffer
           const char *tag,       - log tag (e.g "[ERROR] ") - gets prepended
           const char* fmt_string,- format string ("x: %f, y: %f")
           ...);                  - variadic arguments to fill the fmt_string
 *
 * That being said, There exist many user friendly macro 
 * wrappers for general use.
 *
 * celp_log_level_t is a private enumeration. It is defined this way so that
 * a user can call the wrapper enums defined below which automatically override
 * unecessary parameters based on the log type (except for CELP_LOG_NONE). For 
 * example, with INFO and DEBUG logs, we don't care to log any tracing 
 * information, so it is overridden with NULL'd out values. On the other hand,
 * ERROR and TRACE do need tracing information, so we automatically fill this 
 * out when this "enum" is passed in. These can be called as follows:
 *
 *     celp_log(0, CELP_LOG_INFO, "[MY TAG] ", "what is this: %f", x);
 *     celp_log(0, CELP_LOG_ERROR, "[FAIL] ", "what is that: %d", y);
 *
 * Then, theres our highest level of abstraction macros, which fill out every 
 * field for the user, bar the fmt string and args. These can be called as
 * follows:
 *  
 *     CELP_INFO("Something!");
 *     CELP_ERROR("X incorrect value! %f", x);
 *
 * And finally an example using the function wihtout any wrappers:
 *
 *     char *name = "John";
 *     celp_u32 buffer_len = 256;
 *     char buffer[buffer_len];
 *     celp_log(0, CELP_LOG_NONE, __FILE__, __FUNCTION__, __LINE__,
 *              stdout, buffer, buffer_len, "[MY INFO] ", 
 *              "Hello my name is %s", name);
 */

typedef enum celp_log_e {
   CELP_LOG_NONE, /* if user wants explicit functionality */
   _CELP_LOG_INFO,
   _CELP_LOG_DEBUG,
   _CELP_LOG_ERROR,
   _CELP_LOG_TRACE,
} celp_log_t;

#define CELP_LOG_INFO  (celp_log_t)_CELP_LOG_INFO,  NULL, NULL, 0, \
                                   stdout, NULL, 0
#define CELP_LOG_DEBUG (celp_log_t)_CELP_LOG_DEBUG, NULL, NULL, 0, \
                                   stdout, NULL, 0
#define CELP_LOG_ERROR (celp_log_t)_CELP_LOG_ERROR, \
                                   __FILE__, __FUNCTION__, __LINE__, \
                                   stderr, NULL, 0
#define CELP_LOG_TRACE (celp_log_t)_CELP_LOG_TRACE, \
                                   __FILE__, __FUNCTION__, __LINE__, \
                                   stderr, NULL, 0

CELP_DEF void celp_log(celp_u8 level,
                       celp_log_t log,
                       const char *file,
                       const char *function,
                       celp_u32 line,
                       FILE *out,
                       char *buff,
                       celp_u32 bufflen,
                       const char *tag,
                       const char* fmt_string,
                       ...);

#define CELP_INFO(fmt, ...)       celp_log(0,   CELP_LOG_INFO,  "[INFO] ", \
                                           fmt, ##__VA_ARGS__)
#define CELP_DEBUG(lvl, fmt, ...) celp_log(lvl, CELP_LOG_DEBUG, "[DEBUG] ", \
                                           fmt, ##__VA_ARGS__)
#define CELP_ERROR(fmt, ...)      celp_log(0,   CELP_LOG_ERROR, "[ERROR] ", \
                                           fmt, ##__VA_ARGS__)
#define CELP_TRACE(lvl, fmt, ...) celp_log(lvl, CELP_LOG_TRACE, "[TRACE] ", \
                                           fmt, ##__VA_ARGS__)

/* Generic Abstract Data Types */
/*
 * The following are definitions for various generic abstract data types. The 
 * convention for defining, instantiating and using these is as follows:
 *     celp_example(TYPE);
 *     celp_example_t(TYPE) object;
 *     celp_example_init(&object);
 *     celp_example_operation(&object);
 *     celp_example_destroy(&object);
 *
 *  Under the hood, when celp_example(TYPE) is called, it dynamically generates
 *  and defines the structure for the specific type passed in, and can from
 *  then on be referenced by the same macro with the _t suffix.
 */

/* Dynamic Array Implementation */
/*
 * Generic dynamic array implementation (celp_da). This one is relatively 
 * simple. The structure holds an array of items, a count describing how many 
 * items are in the array and the capacity of items. Upon initialization of the
 * da, we allocate initial capacity. Then when we append, we simply check if 
 * adding one more item would exceed the capcity. If not, we just insert our 
 * item into the next slot, but if it does exceed, we reallocate the items to 
 * a block with double the size.
 *
 * Example:
 *     celp_da(int);
 *     celp_da_t(int) dynamic;
 *     celp_da_init(&dyamic);
 *     celp_da_append(&dynamic, 1);
 *     celp_da_append(&dynamic, 2);
 *     celp_da_append(&dynamic, 3);
 *     celp_da_append(&dynamic, 4);
 *
 *     celp_err_t err;
 *     int popped = celp_da_pop(&dyamic, &err);
 *     if (err != CELP_ERR_OK) return;
 *
 *     celp_da_foreach(&dynamic, item) {
 *         *item++;
 *     }
 *
 * TODO: realloc when shrinking.
 */
#define _da(T) da_##T
#define celp_da_s(T) CELP_S(_da(T))
#define celp_da_t(T) CELP_T(_da(T))

#define celp_da(T) \
    typedef struct celp_da_s(T) { \
        T* items; \
        celp_usize count; \
        celp_usize capacity; \
    } celp_da_t(T);

#define CELP_DA_INITIAL_CAPACITY 256

#define _da_items_t(da) typeof((da)->items)
#define _da_item_t(da)  typeof(*(da)->items)

#define _celp_da_clear(da) \
    do { \
        (da)->items = NULL;\
        (da)->count = 0;\
        (da)->capacity = 0;\
    } while(0)

#define _celp_da_reserve(da, expected_capacity, err) \
    do {\
        _celp_stmt_init((err), da_reserve); \
        \
        if ((expected_capacity) > (da)->capacity) {\
            celp_usize _new_capacity = (da)->capacity; \
            \
            if (_new_capacity == 0) {\
                _new_capacity = CELP_DA_INITIAL_CAPACITY;\
            }\
            while (_new_capacity < expected_capacity) {\
                _new_capacity *= 2;\
            }\
            _da_items_t((da)) _new_items = \
                CELP_REALLOC((da)->items, _new_capacity * sizeof((da)->items[0]));\
            \
            _celp_require(_new_items != NULL, CELP_ERR_ALLOC, da_reserve); \
            \
            (da)->items = _new_items; \
            (da)->capacity = _new_capacity; \
        }\
        _celp_stmt_end(da_reserve); \
    } while(0)

#define _celp_da_append(da, item, err) \
    do {\
        _celp_stmt_init((err), da_append); \
        \
        _celp_da_reserve((da), (da)->count + 1, _celp_err);\
        _celp_propogate(da_append); \
        \
        (da)->items[(da)->count++] = (item);\
        \
        _celp_stmt_end(da_append); \
    } while(0)

#define _celp_da_last(da, err) \
    ({ \
        _celp_expr_init(_da_item_t((da)), (err), da_last); \
        _celp_require((da)->count > 0, CELP_ERR_OOB, da_last); \
        \
        _celp_return = (da)->items[(da)->count-1]; \
        \
        _celp_expr_return(da_last); \
     })

#define _celp_da_pop(da, err) \
    ({ \
        _celp_expr_init(_da_item_t((da)), (err), da_pop); \
        _celp_require((da)->count > 0, CELP_ERR_OOB, da_pop); \
        \
        _celp_return = (da)->items[--(da)->count]; \
        \
        _celp_expr_return(da_pop); \
    })

#define _celp_da_remove(da, idx, err) \
    ({ \
        _celp_expr_init(_da_item_t((da)), (err), da_remove); \
        _celp_require((idx) < (da)->count, CELP_ERR_OOB, da_remove); \
        \
        _da_item_t((da)) _temp = (da)->items[(idx)]; \
        (da)->items[(idx)] = _celp_da_last((da), _celp_err); \
        _celp_propogate(da_remove); \
        \
        (da)->items[(da)->count-1] = _temp; \
        \
        _celp_return = _celp_da_pop((da), _celp_err); \
        _celp_propogate(da_remove); \
        \
        _celp_expr_return(da_remove); \
    })

#define _celp_da_free(da, err) \
    do { \
        _celp_stmt_init((err), da_free); \
        _celp_require((da) != NULL && (da)->items != NULL, \
                      CELP_ERR_ALLOC, da_free); \
        CELP_FREE((da)->items); \
        _celp_da_clear(da); \
        \
        _celp_stmt_end(da_free); \
    } while(0)


/* Dynamic Array Public API */

#define celp_da_init(da)     _celp_da_clear((da))
#define celp_da_clear(da)    ((da)->count = 0)
#define celp_da_is_empty(da) ((da)->count == 0)

#define celp_da_append(da, item, ...) \
    _celp_da_append((da), (item), _celp_err_arg(__VA_ARGS__))

#define celp_da_last(da, ...) \
    _celp_da_last((da), _celp_err_arg(__VA_ARGS__))

#define celp_da_pop(da, ...) \
    _celp_da_pop((da), _celp_err_arg(__VA_ARGS__))

#define celp_da_remove(da, idx, ...) \
    _celp_da_remove((da), (idx), _celp_err_arg(__VA_ARGS__))

#define celp_da_foreach(da, i) \
    for (_da_item_t((da))* (i) = (da)->items; \
         (i) < (da)->items + (da)->count; \
         (i)++)

#define celp_da_free(da, ...) \
    _celp_da_free((da), _celp_err_arg(__VA_ARGS__))

#define celp_da_info(da) \
    do{ \
        CELP_INFO("Dynamic Array at: %p, Capacity: %zu, Count: %zu\n", \
             (da), (da)->capacity, (da)->count); \
    } while(0)

/* Linked List Implementation */
/*
 * Generic linked list implementation (celp_ll). The ll is a bit more
 * interesting, as with every definition, we actually need to define two types,
 * Both the node and the list. The linked list is accessed by the celp_ll 
 * prefix, while the node is accessed by the celp_lln prefix.
 *
 * Example:
 *     celp_err_t err;
 *
 *     celp_ll(int);
 *     celp_ll_t(int) linked;
 *     celp_ll_init(&linked);
 *
 *     celp_ll_add(&linked, 5,  &err);
 *     celp_ll_add(&linked, 10, &err);
 *
 *     int last = celp_ll_get_last(&linked, &err);
 *
 *     celp_ll_foreach(&linked, _node) {
 *         _node->data++;
 *     }
 *
 *     celp_lln_t(int) node = celp_ll_get_at_index_node(&linked, 1, &err);
 */
#define _lln(T) lln_##T
#define celp_lln_s(T) CELP_S(_lln(T))
#define celp_lln_t(T) CELP_T(_lln(T))

#define _ll(T) ll_##T
#define celp_ll_s(T)  CELP_S(_ll(T))
#define celp_ll_t(T)  CELP_T(_ll(T))

#define celp_ll(T) \
    typedef struct celp_lln_s(T) { \
        T data; \
        struct celp_lln_s(T)* prev; \
        struct celp_lln_s(T)* next; \
    } celp_lln_t(T); \
    \
    typedef struct celp_ll_s(T) { \
        celp_lln_t(T) *head; \
        celp_lln_t(T) *tail; \
        celp_usize count; \
    } celp_ll_t(T);

#define _lln_t(ll)      typeof((ll)->head)
#define _lln_data_t(ll) typeof((ll)->head->data)

#define _celp_ll_create_node(ll, x, p, n, err) \
    ({ \
        _celp_err_clear((err)); \
        _lln_t((ll)) _node = CELP_MALLOC(sizeof(*((ll)->head))); \
        if (!_node) {\
            _celp_err_set((err), CELP_ERR_ALLOC); \
        } else { \
            _node->data = (x); \
            _node->prev = (p); \
            _node->next = (n); \
        } \
        \
        _node; \
    })

#define _celp_ll_init(ll, err) \
    do { \
        _celp_stmt_init((err), ll_init); \
        \
        _lln_data_t((ll)) _x_null = {0}; \
        (ll)->head = _celp_ll_create_node((ll), _x_null, NULL, NULL, _celp_err); \
        _celp_propogate(ll_init); \
        \
        (ll)->tail = _celp_ll_create_node((ll), _x_null, NULL, NULL, _celp_err); \
        _celp_propogate(ll_init); \
        \
        (ll)->head->next = (ll)->tail; \
        (ll)->tail->prev = (ll)->head; \
        (ll)->count = 0; \
        \
        _celp_stmt_end(ll_init); \
    } while(0)

#define _celp_ll_foreach(ll, iter) \
    for (_lln_t((ll)) iter = (ll)->head->next; \
        iter != (ll)->tail; \
        iter = iter->next)

#define _celp_ll_get_first(ll, err) \
    ({ \
        _celp_expr_init(_lln_data_t((ll)), (err), ll_get_first); \
        _celp_require((ll)->count > 0, CELP_ERR_OOB, ll_get_first); \
        \
        _celp_return = (ll)->head->next->data; \
        \
        _celp_expr_return(ll_get_first); \
    })

#define _celp_ll_get_first_node(ll, err) \
    ({ \
        _celp_expr_init(_lln_t((ll)), (err), ll_get_first_node); \
        _celp_require((ll)->count > 0, CELP_ERR_OOB, ll_get_first_node); \
        \
        _celp_return = (ll)->head->next; \
        \
        _celp_expr_return(ll_get_first_node); \
    })

#define _celp_ll_get_last(ll, err) \
    ({ \
        _celp_expr_init(_lln_data_t((ll)), (err), ll_get_last); \
        _celp_require((ll)->count > 0, CELP_ERR_OOB, ll_get_last); \
        \
        _celp_return = (ll)->tail->prev->data; \
        \
        _celp_expr_return(ll_get_last); \
    })

#define _celp_ll_get_last_node(ll, err) \
    ({ \
        _celp_expr_init(_lln_t((ll)), (err), ll_get_last_node); \
        _celp_require((ll)->count > 0, CELP_ERR_OOB, ll_get_last_node); \
        \
        _celp_return = (ll)->tail->prev; \
        \
        _celp_expr_return(ll_get_last_node); \
    })

#define _celp_ll_get_node_at(ll, i, err, out) \
    do { \
        *(out) = NULL; \
        _celp_err_clear((err)); \
        \
        if ((i) >= (ll)->count || (i) < 0) { \
            _celp_err_set((err), CELP_ERR_OOB); \
            break; \
        } \
        _lln_t((ll)) _curr = (ll)->head->next; \
        for (celp_usize _i = 0; _i < (i); _i++) { \
            _curr = _curr->next; \
        } \
        *(out) = _curr; \
     } while(0)

#define _celp_ll_get_at_index(ll, i, err) \
    ({ \
        _celp_expr_init(_lln_data_t((ll)), (err), ll_get_at_index); \
        _lln_t((ll)) _node = NULL; \
        \
        _celp_ll_get_node_at((ll), (i), _celp_err, &_node); \
        _celp_propogate(ll_get_at_index); \
        \
        _celp_return = _node->data; \
        \
        _celp_expr_return(ll_get_at_index); \
     })

#define _celp_ll_get_at_index_node(ll, i, err) \
    ({ \
        _celp_expr_init(_lln_t((ll)), (err), ll_get_at_index_node); \
        \
        _lln_t((ll)) _node = NULL; \
        _celp_ll_get_node_at((ll), (i), _celp_err, &_node); \
        _celp_propogate(ll_get_at_index_node); \
        \
        _celp_return = _node; \
        \
        _celp_expr_return(ll_get_at_index_node); \
     })

#define _celp_ll_add_after(ll, x, n, err) \
    do { \
        _celp_stmt_init((err), ll_add_after); \
        _celp_require((n) != (ll)->tail, CELP_ERR_OOB, ll_add_after); \
        \
        _lln_t((ll)) _node = \
            _celp_ll_create_node((ll), (x), (n), (n)->next, _celp_err); \
        _celp_propogate(ll_add_after); \
        \
        _node->prev->next = _node; \
        _node->next->prev = _node; \
        (ll)->count++; \
        \
        _celp_stmt_end(ll_add_after); \
    } while(0)

#define _celp_ll_add_first(ll, x, err) \
    do { \
        _celp_stmt_init((err), ll_add_first); \
        \
        _celp_ll_add_after((ll), (x), (ll)->head, _celp_err); \
        _celp_propogate(ll_add_first); \
        \
        _celp_stmt_end(ll_add_first); \
    } while(0)

#define _celp_ll_add_last(ll, x, err) \
    do { \
        _celp_stmt_init((err), ll_add_last); \
        \
        _celp_ll_add_after((ll), (x), (ll)->tail->prev, _celp_err); \
        _celp_propogate(ll_add_last); \
        \
        _celp_stmt_end(ll_add_last); \
    } while(0)

#define _celp_ll_remove_first(ll, err) \
    ({ \
        _celp_expr_init(_lln_data_t((ll)), (err), ll_remove_first); \
        _celp_require((ll)->count > 0, CELP_ERR_OOB, ll_remove_first); \
        \
        _lln_t((ll)) _to_remove = (ll)->head->next; \
        _celp_return = _to_remove->data; \
        (ll)->head->next->next->prev = (ll)->head; \
        (ll)->head->next = (ll)->head->next->next; \
        CELP_FREE(_to_remove); \
        (ll)->count--; \
        \
        _celp_expr_return(ll_remove_first); \
    })

#define _celp_ll_remove_last(ll, err) \
    ({ \
        _celp_expr_init(_lln_data_t((ll)), (err), ll_remove_last); \
        _celp_require((ll)->count > 0, CELP_ERR_OOB, ll_remove_last); \
        \
        typeof((ll)->tail) _to_remove = (ll)->tail->prev; \
        _celp_return = _to_remove->data; \
        (ll)->tail->prev->prev->next = (ll)->tail; \
        (ll)->tail->prev = (ll)->tail->prev->prev; \
        CELP_FREE(_to_remove); \
        (ll)->count--; \
        \
        _celp_expr_return(ll_remove_last); \
    })

#define _celp_ll_remove_at_index(ll, i, err) \
    ({ \
        _celp_expr_init(_lln_data_t((ll)), (err), ll_remove_at_index); \
        _celp_require((i) >= 0 && (i) < (ll)->count && (ll)->count > 0, \
                      CELP_ERR_OOB, ll_remove_at_index); \
        \
        _lln_t((ll)) _curr = _celp_ll_get_at_index_node((ll), (i), _celp_err);\
        _celp_propogate(ll_remove_at_index); \
        \
        _celp_return = _curr->data; \
        _curr->next->prev = _curr->prev; \
        _curr->prev->next = _curr->next; \
        CELP_FREE(_curr); \
        (ll)->count--; \
        \
        _celp_expr_return(ll_remove_at_index); \
    })

#define celp_lln_exists(n) ((n) != NULL && \
                          !((n)->next == NULL && (n)->prev == NULL))

#define _celp_ll_remove_node(ll, n, err) \
    do { \
        _celp_stmt_init((err), ll_remove_node); \
        _celp_require((ll)->count > 0 && celp_lln_exists((n)), \
                      CELP_ERR_OOB, ll_remove_node); \
        \
        bool _found = false; \
        celp_ll_foreach((ll), _curr) { \
            if (_curr == (n)) { \
                _curr->next->prev = _curr->prev; \
                _curr->prev->next = _curr->next; \
                CELP_FREE(_curr); \
                (ll)->count--; \
                _found = true; \
                break; \
            } \
        } \
        _celp_require(_found, CELP_ERR_OOB, ll_remove_node); \
        \
        _celp_stmt_end(ll_remove_node); \
    } while(0)

#define _celp_ll_free(ll, err) \
    do { \
        _celp_stmt_init((err), ll_free); \
        _celp_require((ll) != NULL, CELP_ERR_ALLOC, ll_free); \
        \
        _lln_t((ll)) _curr = (ll)->head->next; \
        while (_curr != (ll)->tail) { \
            _lln_t((ll)) _next = _curr->next; \
            CELP_FREE(_curr); \
            _curr = _next; \
        } \
        CELP_FREE((ll)->head); \
        CELP_FREE((ll)->tail); \
        (ll)->head = NULL; \
        (ll)->tail = NULL; \
        (ll)->count = 0; \
        \
        _celp_stmt_end(ll_free); \
    } while(0)


/* Linked List Public API */

#define celp_ll_init(ll, ...) \
    _celp_ll_init((ll), _celp_err_arg(__VA_ARGS__))

#define celp_ll_is_empty(ll) ((ll)->count == 0)
#define celp_ll_foreach _celp_ll_foreach

#define celp_ll_get_first(ll, ...) \
    _celp_ll_get_first((ll), _celp_err_arg(__VA_ARGS__))

#define celp_ll_get_first_node(ll, ...) \
    _celp_ll_get_first_node((ll), _celp_err_arg(__VA_ARGS__))

#define celp_ll_get_last(ll, ...) \
    _celp_ll_get_last((ll), _celp_err_arg(__VA_ARGS__))

#define celp_ll_get_last_node(ll, ...) \
    _celp_ll_get_last_node((ll), _celp_err_arg(__VA_ARGS__))

#define celp_ll_get_at_index(ll, i, ...) \
    _celp_ll_get_at_index((ll), (i), _celp_err_arg(__VA_ARGS__))

#define celp_ll_get_at_index_node(ll, i, ...) \
    _celp_ll_get_at_index_node((ll), (i), _celp_err_arg(__VA_ARGS__))

#define celp_ll_add_after(ll, x, n, ...) \
    _celp_ll_add_after((ll), (x), (n), _celp_err_arg(__VA_ARGS__))

#define celp_ll_add_first(ll, x, ...) \
    _celp_ll_add_first((ll), (x), _celp_err_arg(__VA_ARGS__))

#define celp_ll_add_last(ll, x, ...) \
    _celp_ll_add_last((ll), (x), _celp_err_arg(__VA_ARGS__))

#define celp_ll_add celp_ll_add_last

#define celp_ll_remove_first(ll, ...) \
    _celp_ll_remove_first((ll), _celp_err_arg(__VA_ARGS__))

#define celp_ll_remove_last(ll, ...) \
    _celp_ll_remove_last((ll), _celp_err_arg(__VA_ARGS__))

#define celp_ll_remove_at_index(ll, i, ...) \
    _celp_ll_remove_at_index((ll), (i), _celp_err_arg(__VA_ARGS__))

#define celp_ll_remove_node(ll, n, ...) \
    _celp_ll_remove_node((ll), (n), _celp_err_arg(__VA_ARGS__))

#define celp_ll_free(ll, ...) \
    _celp_ll_free((ll), _celp_err_arg(__VA_ARGS__))


/* HashMap Implementation */
/*
 * Generic hash map implementation (celp_map). The map is made up of a few 
 * components:
 *     kv: key value pair, user defines type for both.
 *     buckets: a linked list of kv pairs.
 *     map: the hashmap, which contains X buckets, a count and a capacity
 *
 * Within the initialization macro (celp_map(kT, vT)), we dynamically define
 * both kv and map types. Also, we must generate a linked list with type
 * kv(kT, vT) to be used for our buckets within the hashmap.
 *
 * The hashing function itself isn't very flexible and is designed for integers
 * if I'm not mistaken so it will either need to be updated or I will need to 
 * look into some type defined hashing.
 *
 * The general flow of all of these function is generate the hash for a given
 * key, check at that hash to see if the key already exists, perform some 
 * operation based on this..
 *
 * TODO: realloc and rehash after certain size
 * TODO: update hashing func
 */
#define _map(kT, vT) CELP_CATTT(CELP_CAT(map_, kT), _, vT)
#define celp_map_t(kT, vT) CELP_T(_map(kT, vT))
#define celp_map_s(kT, vT) CELP_S(_map(kT, vT))

#define _kv(kT, vT)  CELP_CATTT(CELP_CAT(kv_, kT), _, vT) 
#define celp_kv_t(kT, vT)  CELP_T(_kv(kT, vT)) 
#define celp_kv_s(kT, vT)  CELP_S(_kv(kT, vT)) 

#define celp_map(kT, vT) \
    typedef struct celp_kv_s(kT, vT) { \
        kT key; \
        vT value; \
    } celp_kv_t(kT, vT); \
    \
    celp_ll(celp_kv_t(kT, vT)); \
    \
    typedef struct celp_map_s(kT, vT) { \
        celp_ll_t(celp_kv_t(kT, vT))* buckets; \
        celp_usize count; \
        celp_usize capacity; \
    }  celp_map_t(kT, vT);

#define CELP_MAP_INITIAL_CAPACITY 64

#define _map_bucket_t(map)      typeof(*(map)->buckets) //ll
#define _map_bucket_node_t(map) typeof((map)->buckets[0].head) //lln
#define _map_kv_t(map)          typeof((map)->buckets[_h].head->data)
#define _map_k_t(map)           typeof((map)->buckets[0].head->data.key)
#define _map_v_t(map)           typeof((map)->buckets[0].head->data.value)

#define _celp_map_clear(map) \
    do {\
        (map)->buckets = NULL; \
        (map)->count = 0; \
        (map)->capacity = 0; \
    } while(0)

#define _celp_map_init(map, err) \
    do { \
        _celp_stmt_init((err), map_init); \
        \
        _celp_map_clear((map)); \
        (map)->capacity = CELP_MAP_INITIAL_CAPACITY; \
        (map)->buckets = CELP_CALLOC((map)->capacity, sizeof((map)->buckets[0])); \
        _celp_require((map)->buckets != NULL, CELP_ERR_ALLOC, map_init); \
        \
        for (size_t _i = 0; _i < (map)->capacity; _i++) { \
            _celp_ll_init(&((map)->buckets[_i]), _celp_err); \
            _celp_propogate(map_init); \
        } \
        _celp_stmt_end(map_init); \
    } while(0)

#define _celp_map_get_hash(map, k) \
    ({ \
        const unsigned char* _k_bytes = (const unsigned char*)&(k); \
        celp_u32 _h = CELP_HASH(_k_bytes, sizeof((k))) % (map)->capacity; \
        _h; \
     })

/*returns the hashed bucket, and whether or not the key was found */
#define _celp_map_find_k(map, k, hash, found, out, err) \
    do { \
        _celp_err_clear((err)); \
        *(out) = NULL; \
        if ((map)->buckets == NULL) { \
            _celp_err_set((err), CELP_ERR_ALLOC); \
            break; \
        } \
        _map_bucket_node_t((map)) _return = (map)->buckets[(hash)].head; \
        *(found) = false; \
        _celp_ll_foreach(&(map)->buckets[(hash)], _bucket) {\
            if (CELP_COMP(_bucket->data.key, (k)) == 0) { \
                _return = _bucket; \
                *(found) = true; \
                break; \
            } \
        } \
        *(out) = _return; \
    } while(0)

#define _celp_map_insert(map, k, v, err) \
    do { \
        _celp_stmt_init((err), map_insert); \
        \
        _map_k_t((map)) _k = (k); \
        celp_u32 _h = _celp_map_get_hash((map), _k); \
        bool _found = false; \
        _map_bucket_node_t((map)) _bucket; \
        _celp_map_find_k((map), _k, _h, &_found, &_bucket, _celp_err); \
        _celp_propogate(map_insert); \
        \
        if (_found) { \
            _bucket->data.value = (v); \
        } else { \
            _map_kv_t((map)) _kv = \
                { .key = (_k), .value = (v) }; \
            \
            _celp_ll_add_last(&((map)->buckets[_h]), _kv, _celp_err); \
            _celp_propogate(map_insert); \
            \
            (map)->count++; \
        } \
        _celp_stmt_end(map_insert); \
    } while(0)

// assumes the value can be incremented
// if the key isnt already in the map it assigns value 1
#define _celp_map_increment(map, k, err) \
    do { \
        _celp_stmt_init((err), map_increment); \
        \
        _map_k_t((map)) _k = (k); \
        celp_u32 _h = _celp_map_get_hash((map), _k); \
        bool _found = false; \
        _map_bucket_node_t((map)) _bucket; \
        _celp_map_find_k((map), _k, _h, &_found, &_bucket, _celp_err); \
        _celp_propogate(map_increment); \
        \
        if (_found) { \
            _bucket->data.value++; \
        } else { \
            typeof((map)->buckets[_h].head->data) _kv = \
                { .key = (_k), .value = 1 }; \
            \
            _celp_ll_add_last(&((map)->buckets[_h]), _kv, _celp_err); \
            _celp_propogate(map_increment); \
            \
            (map)->count++; \
        } \
        _celp_stmt_end(map_increment); \
    } while(0)

#define _celp_map_contains(map, k, err) \
    ({ \
        _celp_expr_init(bool, (err), map_contains); \
        _celp_require((map)->capacity > 0, CELP_ERR_OOB, map_contains); \
        \
        _map_k_t((map)) _k = (k); \
        celp_u32 _h = _celp_map_get_hash((map), _k); \
        _map_bucket_node_t((map)) _bucket; \
        _celp_map_find_k((map), _k, _h, &_celp_return, &_bucket, _celp_err); \
        _celp_propogate(map_contains); \
        \
        _celp_expr_return(map_contains); \
    })

#define _celp_map_get(map, k, dval, err) \
    ({ \
        _celp_expr_init(typeof((dval)), (err), map_get); \
        \
        bool _found = false; \
        _map_k_t((map)) _k = (k); \
        celp_u32 _h = _celp_map_get_hash((map), _k); \
        _map_bucket_node_t((map)) _bucket; \
        \
        _celp_map_find_k((map), _k, _h, &_found, &_bucket, _celp_err); \
        _celp_propogate(map_get); \
        \
        if (_found) { \
            _celp_return = _bucket->data.value; \
        } else { \
            _celp_return = (dval); \
        } \
        _celp_expr_return( map_get); \
    })

#define _celp_map_remove(map, k, err) \
    ({ \
        _celp_expr_init(_map_v_t((map)), (err), map_remove); \
        _celp_require((map)->count > 0, CELP_ERR_OOB, map_remove); \
        \
        bool _found = false; \
        _map_k_t((map)) _k = (k); \
        celp_u32 _h = _celp_map_get_hash((map), _k); \
        _map_bucket_node_t((map)) _bucket; \
        _celp_map_find_k((map), _k, _h, &_found, &_bucket, _celp_err); \
        _celp_propogate(map_remove); \
        \
        if (_found) { \
            _celp_return = _bucket->data.value; \
            _celp_ll_remove_node(&((map)->buckets[_h]), _bucket, _celp_err); \
            _celp_propogate(map_remove); \
            (map)->count--; \
        } \
        _celp_expr_return(map_remove); \
    })

#define _celp_map_free(map, err) \
    do { \
        _celp_stmt_init((err), map_free); \
        _celp_require((map) != NULL && (map)->buckets != NULL, \
                      CELP_ERR_ALLOC, map_free); \
        \
        for (size_t _i = 0; _i < (map)->capacity; _i++) { \
            _celp_ll_free(&((map)->buckets[_i]), _celp_err); \
            _celp_propogate(map_free); \
        } \
        CELP_FREE((map)->buckets); \
        _celp_map_clear((map)); \
        \
        _celp_stmt_end(map_free); \
    } while(0)

#define celp_map_info(map) \
    do { \
        CELP_INFO("Map at: %p, Capacity: %zu, Count: %zu", \
             (map), (map)->capacity, (map)->count); \
    } while(0)


/* HashMap Public API */

#define celp_map_is_empty(map) ((map)->count == 0)

#define celp_map_init(map, ...) \
    _celp_map_init((map), _celp_err_arg(__VA_ARGS__))

#define celp_map_insert(map, k, v, ...) \
    _celp_map_insert((map), (k), (v), _celp_err_arg(__VA_ARGS__))

#define celp_map_increment(map, k, ...) \
    _celp_map_increment((map), (k), _celp_err_arg(__VA_ARGS__))

#define celp_map_contains(map, k, ...) \
    _celp_map_contains((map), (k), _celp_err_arg(__VA_ARGS__))

#define celp_map_get(map, k, dval, ...) \
    _celp_map_get((map), (k), (dval), _celp_err_arg(__VA_ARGS__))

#define celp_map_remove(map, k, ...) \
    _celp_map_remove((map), (k), _celp_err_arg(__VA_ARGS__))

#define celp_map_free(map, ...) \
    _celp_map_free((map), _celp_err_arg(__VA_ARGS__))

#define celp_map_info(map) \
    do { \
        CELP_INFO("Map at: %p, Capacity: %zu, Count: %zu", \
             (map), (map)->capacity, (map)->count); \
    } while(0)


//math macros
#ifdef CELP_MATH

//to silence vector {{x, y, z}} warning
#if defined(__GNUC__) || defined(__clang__)
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wmissing-braces"
#endif

/* Vector2 */
#define _v2(T) v2_##T
#define celp_v2_s(T) CELP_S(_v2(T))
#define celp_v2_t(T) CELP_T(_v2(T))

#define celp_v2(T) \
    typedef union celp_v2_s(T) { \
        struct { \
            T x, y; \
        }; \
        T vs[2]; \
    } celp_v2_t(T); 

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
#define celp_v3_s(T) CELP_S(_v3(T))
#define celp_v3_t(T) CELP_T(_v3(T))

#define celp_v3(T) \
    typedef union celp_v3_s(T) { \
        struct { \
            T x, y, z; \
        }; \
        T vs[3]; \
    } celp_v3_t(T);

#define celp_v3f_str(v) "{ %f, %f, %f }", (v).x, (v).y, (v).z

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

/* Vector4 */
#define _v4(T) v4_##T
#define celp_v4_t(T) CELP_T(_v4(T))
#define celp_v4_s(T) CELP_S(_v4(T))

#define celp_v4(T) \
    typedef union celp_v4_s(T) { \
        struct { \
            T x, y, z, w; \
        }; \
        T vs[4]; \
    } celp_v4_t(T);

#define celp_v4f_str(v) "{ %f, %f, %f, %f }", (v).x, (v).y, (v).z, (v).w

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
    celp_v4_t(T) _v_out = {(v).x, (v).y, (v).z, 1}; \
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
    celp_v3_t(T) _v_out = {(v).x, (v).y, (v).z}; \
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
#define celp_m4_s(T) CELP_S(_m4(T))
#define celp_m4_t(T) CELP_T(_m4(T))

#define celp_m4(T) \
    typedef struct celp_m4_s(T) { \
        T v[4][4]; \
    } celp_m4_t(T);

#define celp_m4_id {{   \
    {1, 0, 0, 0}, \
    {0, 1, 0, 0}, \
    {0, 0, 1, 0}, \
    {0, 0, 0, 1}}}

#define celp_m4_trans(t) {{       \
    {1, 0, 0, t.x}, \
    {0, 1, 0, t.y}, \
    {0, 0, 1, t.z}, \
    {0, 0, 0,  1}}}

#define celp_m4_scale(s) {{  \
    {s, 0, 0, 0},  \
    {0, s, 0, 0},  \
    {0, 0, s, 0},  \
    {0, 0, 0, 1}}}

#define celp_m4_scalev(s) {{ \
    {s.x,   0,   0,  0},    \
    {  0, s.y,   0,  0},    \
    {  0,   0, s.z,  0},    \
    {  0,   0,   0,  1}}} 

#define celp_m4_rotx(a) {{   \
    {1,      0,       0, 0}, \
    {0, cos(a), -sin(a), 0}, \
    {0, sin(a),  cos(a), 0}, \
    {0,      0,       0, 1}}}

#define celp_m4_roty(a) {{   \
    { cos(a), 0, sin(a), 0}, \
    {      0, 1,      0, 0}, \
    {-sin(a), 0, cos(a), 0}, \
    {      0, 0,      0, 1}}}

#define celp_m4_rotz(a) {{   \
    {cos(a), -sin(a), 0, 0}, \
    {sin(a),  cos(a), 0, 0}, \
    {     0,       0, 1, 0}, \
    {     0,       0, 0, 1}}}


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
    typeof((m1)) _out = {0}; \
    for (celp_u8 _i = 0; _i < 4; _i++) { \
        for (celp_u8 _j = 0; _j < 4; _j++) { \
        typeof((m1).v[0][0]) _sum = 0; \
        for (celp_u8 _k = 0; _k < 4; _k++) { \
            _sum += (m1).v[_i][_k] * (m1).v[_k][_j]; \
        } \
        _out.v[_i][_j] = _sum; \
        } \
    } \
    _out; \
})

#define celp_v4_trans(v, t) \
    celp_m4_v4_mul(celp_m4_trans((t)), (v))

#define celp_v3_trans(v, t) \
    celp_v4_to_v3(celp_v4_trans(celp_v3_to_v4((v), typeof((v).x)), (t)), typeof((v).x))

#define celp_v4_scale(v, s) \
    celp_m4_v4_mul(celp_m4_scale((s)), (v))

#define celp_v4_scalev(v, s) \
    celp_m4_v4_mul(celp_m4_scalev((s)), (v))

#endif //CELP_MATH

/* Testing */
#ifdef CELP_TEST

#define CELP_TEST_FAIL_MSG_LEN 4096
static char celp_test_fail_msg[CELP_TEST_FAIL_MSG_LEN];

#define CELP_EXPECT(cond) do { \
    celp_test_assertions++; \
    if (!(cond)) { \
        celp_test_fails++; \
        celp_test_result = CELP_TEST_RESULT_FAIL; \
        /*
        snprintf(celp_test_fail_msg, CELP_TEST_FAIL_MSG_LEN, \
                 "%s:%s:%d (%s)\n", \
                 __FILE__, __FUNCTION__, __LINE__, #cond); \
        */ \
        celp_log(0, _CELP_LOG_ERROR, \
                 __FILE__, __FUNCTION__, __LINE__, \
                 NULL, celp_test_fail_msg, CELP_TEST_FAIL_MSG_LEN, \
                "[FAILURE] ", "(%s)", #cond); \
    } else { celp_test_passes++; } \
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
static celp_u32 celp_test_assertions = 0;

typedef struct celp_testcase_s {
    char *name;
    void (*testcase)(void);
    celp_test_result_t result;
} celp_testcase_t;

celp_ll(celp_testcase_t);
typedef struct celp_test_suite_s {
    //struct celp_test_suite_s *suites;
    char *name;
    void (*setup)(void);
    void (*teardown)(void);
    celp_ll_t(celp_testcase_t) tests;
} celp_test_suite_t;

#define CELP_TESTCASE(t)      void _celp_testcase_##t()
#define CELP_TEST_SETUP(s)    void _celp_test_setup_##s()
#define CELP_TEST_TEARDOWN(t) void _celp_test_teardown_##t()

#define CELP_TEST_SUITE_START(s) \
    CELP_DEF_SI celp_test_suite_t \
    *_celp_test_suite_##s##_func() { \
        celp_test_suite_t *_celp_test_suite_##s = \
            CELP_MALLOC(sizeof(celp_test_suite_t)); \
        _celp_test_suite_##s->name = #s; \
        _celp_test_suite_##s->setup = NULL; \
        _celp_test_suite_##s->teardown = NULL; \
        celp_err_t _celp_test_suite_##s##_err; \
        celp_ll_init(&_celp_test_suite_##s->tests, &_celp_test_suite_##s##_err); \
        
#define CELP_TEST_SUITE_ADD_SETUP(s, t) \
        _celp_test_suite_##s->setup = &(_celp_test_setup_##t);

#define CELP_TEST_SUITE_ADD_TEARDOWN(s, t) \
        _celp_test_suite_##s->teardown = &(_celp_test_teardown_##t);

#define CELP_TEST_SUITE_ADD_TEST(s, t) \
        celp_testcase_t _##t; \
        _##t.name = #t; \
        _##t.testcase = &_celp_testcase_##t; \
        _##t.result = CELP_TEST_RESULT_NONE; \
        celp_ll_add(&_celp_test_suite_##s->tests, _##t, \
                    &_celp_test_suite_##s##_err);

#define CELP_TEST_SUITE_END(s) \
        return _celp_test_suite_##s; \
    }

#define CELP_TEST_SUITE_RUN(s) \
    celp_test_suite_t *_celp_test_suite_##s = \
        _celp_test_suite_##s##_func(); \
    if (_celp_test_suite_##s->setup) \
        _celp_test_suite_##s->setup(); \
    _celp_ll_foreach(&_celp_test_suite_##s->tests, _celp_test) { \
        celp_test_result = CELP_TEST_RESULT_NONE; \
        _celp_test->data.testcase(); \
        celp_test_runs++; \
        _celp_test->data.result = celp_test_result; \
    } \
    if (_celp_test_suite_##s->teardown) \
        _celp_test_suite_##s->teardown(); \

#define CELP_TEST_SUITE_REPORT(s) \
    celp_log(0, CELP_LOG_INFO, \
             "[TEST_SUITE] ", "%s", _celp_test_suite_##s->name); \
    _celp_ll_foreach(&_celp_test_suite_##s->tests, _celp_test) { \
        celp_log(0, CELP_LOG_INFO, \
                "\t[TESTCASE] ", "%s %s", _celp_test->data.name, \
                (_celp_test->data.result==CELP_TEST_RESULT_FAIL) ? \
                "[FAIL]" : "[PASS]"); \
    } \
    celp_log(0, CELP_LOG_INFO, \
            "[REPORT] ", "RUNS: %d - ASSERTIONS: %d" \
            " - PASSED: %d - FAILED: %d", \
            celp_test_runs, celp_test_assertions, \
            celp_test_passes, celp_test_fails); \
    celp_log(0, CELP_LOG_INFO, \
             (celp_test_fails > 0) ? "[FAILURE] " : "", \
             "%s", celp_test_fail_msg); \

#define CELP_TEST_SUITE_DESTROY(s) \
    celp_err_t _celp_test_suite_##s##_err; \
    celp_ll_free(&_celp_test_suite_##s->tests, \
                 &_celp_test_suite_##s##_err); \
    CELP_FREE(_celp_test_suite_##s); \

#endif //CELP_TEST

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
         celp_u32 line,
         FILE *out, 
         char *buff, /* optional if you want to log to a buffer */
         celp_u32 bufflen,
         const char *tag,
         const char* fmt_string,
         ...)
{
    if (level > CELP_LOG_LEVEL) return;
    va_list args;
    va_start(args, fmt_string); 
    char fmt_str_trace[1024];

    switch(log) {
        case _CELP_LOG_INFO:
            #ifndef CELP_LOG_MODE_INFO
                return;
            #endif //CELP_LOG_MODE_INFO
            goto end;
        case _CELP_LOG_ERROR:
            #ifdef CELP_LOG_MODE_ERROR
                goto prepend;
            #else
                return;
            #endif //CELP_LOG_MODE_ERROR
        case _CELP_LOG_DEBUG:
            #ifndef CELP_LOG_MODE_DEBUG
                return;
            #endif //CELP_LOG_MODE_DEBUG
            goto end;
        case _CELP_LOG_TRACE:
            #ifdef CELP_LOG_MODE_TRACE
                goto prepend;
            #else
                return;
            #endif //CELP_LOG_MODE_TRACE
    }

prepend:
    snprintf(fmt_str_trace, 424, "%s:%s:%d\n\t",
             file, function, line);
    strncat(fmt_str_trace, fmt_string, 600);
    fmt_string = fmt_str_trace;
end:
    if (out) {
        fputs(tag, out);
        vfprintf(out, fmt_string, args); 
        fputc('\n', out);
    }
    if (buff) {
        vsnprintf(buff, bufflen, fmt_string, args);
    }
    va_end(args);
    return;
}

#endif //CELP_IMPLEMENTATION

//shamelessly ripped from mr tsoding
//if you dont want to keep writing celp :|
#ifdef CELP_STRIP_PREFIX
    //TYPES
    #define f32                     celp_f32
    #define f64                     celp_f64 
    #define u8                      celp_u8 
    #define u16                     celp_u16 
    #define u32                     celp_u32
    #define u64                     celp_u64
    #define i8                      celp_i8
    #define i16                     celp_i16 
    #define i32                     celp_i32 
    #define i64                     celp_i64
    #define usize                   celp_usize 
    #define isize                   celp_isize
    //MISC
    #define COMP                    CELP_COMP 
    #define SWAP                    CELP_SWAP
    #define CAT                     CELP_CAT
    #define HASH                    CELP_HASH  

#ifdef CELP_TEST
    #define EXPECT                  CELP_EXPECT
    #define EXPECT_EQ               CELP_EXPECT_EQ
    #define EXPECT_NEQ              CELP_EXPECT_NEQ
    #define TESTCASE                CELP_TESTCASE 
    #define TEST_SETUP              CELP_TEST_SETUP 
    #define TEST_TEARDOWN           CELP_TEST_TEARDOWN
    #define TEST_SUITE_START        CELP_TEST_SUITE_START
    #define TEST_SUITE_ADD_TEARDOWN CELP_TEST_SUITE_ADD_TEARDOWN
    #define TEST_SUITE_ADD_SETUP    CELP_TEST_SUITE_ADD_SETUP
    #define TEST_SUITE_ADD_TEST     CELP_TEST_SUITE_ADD_TEST
    #define TEST_SUITE_END          CELP_TEST_SUITE_END
    #define TEST_SUITE_RUN          CELP_TEST_SUITE_RUN
    #define TEST_SUITE_REPORT       CELP_TEST_SUITE_REPORT
    #define TEST_SUITE_DESTROY      CELP_TEST_SUITE_DESTROY
#endif //CELP_TEST

    //CELP_LOG
    #define log                     celp_log
    #define LOG_INFO                CELP_LOG_INFO
    #define LOG_DEBUG               CELP_LOG_DEBUG
    #define LOG_ERROR               CELP_LOG_ERROR 
    #define LOG_TRACE               CELP_LOG_TRACE  
    #define INFO                    CELP_INFO 
    #define DEBUG                   CELP_DEBUG 
    #define ERROR                   CELP_ERROR 
    #define TRACE                   CELP_TRACE
    //CELP_DA
    #define da                      celp_da
    #define da_t                    celp_da_t
    #define da_init                 celp_da_init
    #define da_clear                celp_da_clear
    #define da_is_empty             celp_da_is_empty
    #define da_append               celp_da_append
    #define da_last                 celp_da_last
    #define da_pop                  celp_da_pop
    #define da_remove               celp_da_remove
    #define da_foreach              celp_da_foreach
    #define da_free                 celp_da_free
    #define da_info                 celp_da_info
    //celp_ll
    #define ll                      celp_ll 
    #define ll_t                    celp_ll_t
    #define ll_init                 celp_ll_init
    #define ll_is_empty             celp_ll_is_empty
    #define ll_get_first            celp_ll_get_first
    #define ll_get_last             celp_ll_get_last
    #define ll_add                  celp_ll_add
    #define ll_add_first            celp_ll_add_first
    #define ll_add_last             celp_ll_add_last
    #define ll_add                  celp_ll_add
    #define ll_remove_first         celp_ll_remove_first
    #define ll_remove_last          celp_ll_remove_last
    #define ll_remove_at_index      celp_ll_remove_at_index
    #define ll_remove_node          celp_ll_remove_node
    #define ll_print_int            celp_ll_print_int
    #define ll_foreach              celp_ll_foreach
    #define ll_free                 celp_ll_free
    #define ll_info                 celp_ll_info
    //celp_map
    #define kv                      celp_kv
    #define kv_t                    celp_kv_t
    #define map                     celp_map
    #define map_t                   celp_map_t 
    #define map_init                celp_map_init
    #define map_is_empty            celp_map_is_empty
    #define map_insert              celp_map_insert
    #define map_increment           celp_map_increment
    #define map_get                 celp_map_get
    #define map_contains            celp_map_contains
    #define map_remove              celp_map_remove
    #define map_free                celp_map_free
    #define map_info                celp_map_info

#ifdef CELP_MATH 
    //v2
    #define v2                      celp_v2
    #define v2_t                    celp_v2_t
    #define v2_add                  celp_v2_add
    #define v2_sub                  celp_v2_sub
    #define v2_dot                  celp_v2_dot
    #define v2_cross                celp_v2_cross
    #define v2_scale                celp_v2_scale
    //v3
    #define v3                      celp_v3
    #define v3_t                    celp_v3_t
    #define v3_add                  celp_v3_add
    #define v3_sub                  celp_v3_sub
    #define v3_dot                  celp_v3_dot
    #define v3_scale                celp_v3_scale
    #define v3_cross                celp_v3_cross
    #define v3_len                  celp_v3_len
    #define v3_norm                 celp_v3_norm
    #define v3_neg                  celp_v3_neg
    #define v3_contains_neg         celp_v3_contains_neg
    #define v3_contains_zero        celp_v3_contains_zero
    #define v3_is_empty             celp_v3_is_empty
    #define v3f_str                 celp_v3f_str
    //v4
    #define v4                      celp_v4
    #define v4_t                    celp_v4_t
    #define v4f_str                 celp_v4f_str
    #define v3_to_v4                celp_v3_to_v4 
    #define v4_norm                 celp_v4_norm
    #define v4_to_v3                celp_v4_to_v3
    #define v4_dot                  celp_v4_dot
    //m4
    #define m4                      celp_m4
    #define m4_t                    celp_m4_t 
    #define m4_id                   celp_m4_id
    #define m4_trans                celp_m4_trans
    #define m4_scale                celp_m4_scale
    #define m4_scalev               celp_m4_scalev
    #define m4_rotx                 CELP_m4_rotx
    #define m4_roty                 CELP_m4_roty
    #define m4_rotz                 CELP_m4_rotz 
    #define m4_v4_mul               celp_m4_v4_mul
    #define m4_mul                  celp_m4_mul
    //m4 wrappers 
    #define v4_trans                celp_v4_trans
    #define v3_trans                celp_v3_trans
    #define v4_scale                celp_v4_scale 
    #define v4_scalev               celp_v4_scalev
#endif //CELP_MATH
       
#endif //CELP_STRIP_PREFIX
       
#endif //_CELP_H
