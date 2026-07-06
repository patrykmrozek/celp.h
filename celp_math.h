#ifndef _CELP_MATH_H
#define _CELP_MATH_H

/* Math */
#define CELP_V2(dtype) \
    typedef struct CELP_V2_##dtype##_s { \
        dtype x, y; \
    } CELP_V2_##dtype##_t;

#define CELP_V2_T(dtype) CELP_V2_##dtype##_t

#define CELP_V3(dtype) \
    typedef struct CELP_V3_##dtype##_s { \
        dtype x, y, z; \
    } CELP_V3_##dtype##_t;

#define CELP_V3_T(dtype) CELP_V3_##dtype##_t

#define celp_v2_add(v1, v2) \
    ({ \
        typeof((v1)) __v_out = { \
            .x = (v1).x + (v2).x, \
            .y = (v1).y + (v2).y \
        }; \
        __v_out; \
    })

#define celp_v2_sub(v1, v2) \
    ({ \
        typeof((v1)) __v_out = { \
            .x = (v1).x - (v2).x, \
            .y = (v1).y - (v2).y \
        }; \
        __v_out; \
    })

#define celp_v2_dot(v1, v2) ((v1).x * (v2).x) + ((v1).y * (v2).y)

#define celp_v2_scale(v, s) \
    ({ \
        typeof((v)) __v_out = { \
            .x = (v).x * s, \
            .y = (v).y * s \
        }; \
        __v_out; \
    })

#define celp_v3_add(v1, v2) \
    ({ \
       typeof((v1)) __v_out = { \
           .x = (v1).x + (v2).x, \
           .y = (v1).y + (v2).y, \
           .z = (v1).z + (v2).z\
       }; \
       __v_out; \
    })

#define celp_v3_sub(v1, v2) \
({ \
    typeof((v1)) __v_out = { \
        .x = (v1).x - (v2).x, \
        .y = (v1).y - (v2).y, \
        .z = (v1).z - (v2).z\
    }; \
    __v_out; \
})

#define celp_v3_dot(v1, v2) ((v1).x * (v2).x) + ((v1).y * (v2).y) + ((v1).z * (v2).z)

#define celp_v3_scale(v, s) \
    ({ \
        typeof((v)) __v_out = { \
            .x = (v).x * s, \
            .y = (v).y * s, \
            .z = (v).z * s \
        }; \
        __v_out; \
    })

#define celp_v3_cross(v1, v2) \
    ({ \
        typeof((v1)) __v_out = { \
            .x = ((v1).y * (v2).z) - ((v1).z * (v2).y), \
            .y = ((v1).z * (v2).x) - ((v1).x * (v2).z), \
            .z = ((v1).x * (v2).y) - ((v1).y * (v2).x) \
        }; \
        __v_out; \
    })

#define celp_v3_normalize(v) \
({ \
    typeof((v)) __v_out = {0}; \
    float len_sq = (v).x*(v).x + (v).y*(v).y + (v).z*(v).z; \
    if (len_sq > 0.0f) { \
        float inv_len = 1.0f / sqrtf(len_sq); \
        __v_out.x = (v).x * inv_len; \
        __v_out.y = (v).y * inv_len; \
        __v_out.z = (v).z * inv_len; \
    } \
    __v_out; \
})

#ifdef CELP_STRIP_PREFIX
    // CELP_MATH
    #define V2                    CELP_V2
    #define V2_T                  CELP_V2_T
    #define V3                    CELP_V3
    #define V3_T                  CELP_V3_T
    #define v2_add                celp_v2_add
    #define v2_sub                celp_v2_sub
    #define v2_dot                celp_v2_dot
    #define v2_scale              celp_v2_scale
    #define v3_add                celp_v3_add
    #define v3_sub                celp_v3_sub
    #define v3_dot                celp_v3_dot
    #define v3_scale              celp_v3_scale
    #define v3_cross              celp_v3_cross
    #define v3_normalize          celp_v3_normalize
#endif //CELP_STRIP_PREFIX

#endif //_CELP_MATH_H
