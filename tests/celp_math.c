#define CELP_MATH
#define CELP_STRIP_PREFIX
#include "../celp.h"
#include <stdio.h>

CELP_V2(int);
#define v2i CELP_V2_T(int)
CELP_V3(int);
#define v3i CELP_V3_T(int)

void celp_math() {
    CELP_DEBUG(1, "MATH");

    v2i w = {2, 2};
    v2i e = {3, 3};
    v2i r = celp_v2_add(w, e);
    CELP_DEBUG(1, 
        "v2 Add: "
        "[%i %i] + [%i %i] = [%i %i]",
        w.x, w.y, e.x, e.y, r.x, r.y);

    v2i w2 = {2, 2};
    v2i e2 = {3, 3};
    v2i r2 = celp_v2_sub(w2, e2);
    CELP_DEBUG(1, 
        "v2 Sub: "
        "[%i %i] - [%i %i] = [%i %i]",
        w2.x, w2.y, e2.x, e2.y, r2.x, r2.y);

    int r3 = celp_v2_dot(w, e);
    CELP_DEBUG(1, 
        "v2 Dot: "
        "[%i %i] . [%i %i] = %i",
        w.x, w.y, e.x, e.y, r3);

    int scale = 2;
    v2i r4 = celp_v2_scale(w2, scale);
    CELP_DEBUG(1, 
        "v2 Scale: "
        "[%i %i] * %i = [%i %i]",
        w2.x, w2.y, scale, r4.x, r4.y);

    v3i a = {1, 1, 1};
    v3i b = {1, 1, 1};
    v3i res = celp_v3_add(a, b);

    CELP_DEBUG(1, 
        "v3 Add: "
        "[%i %i %i] + [%i %i %i] = [%i %i %i]",
        a.x, a.y, a.z, b.x, b.y, b.z, res.x, res.y, res.z);

    v3i a2 = {4, 4, 4};
    v3i b2 = {1, 1, 1};
    v3i res2 = celp_v3_sub(a2, b2);

    CELP_DEBUG(1, 
        "v3 Sub: "
        "[%i %i %i] - [%i %i %i] = [%i %i %i]",
        a2.x, a2.y, a2.z, b2.x, b2.y, b2.z, res2.x, res2.y, res2.z);

    int res3 = celp_v3_dot(a2, b2);

    CELP_DEBUG(1, 
        "v3 Sub: "
        "[%i %i %i] . [%i %i %i] = %i",
        a2.x, a2.y, a2.z, b2.x, b2.y, b2.z, res3);

    int scale2 = 4;
    v3i r5 = celp_v3_scale(a2, scale2);

    CELP_DEBUG(1, 
        "v3 Scale: "
        "[%i %i %i] * %i = [%i %i %i]",
        a2.x, a2.y, a2.z, scale2, r5.x, r5.y, r5.z);

    v3i c1 = {2, 3, 4};
    v3i c2 = {5, 6, 7};
    v3i cr = celp_v3_cross(c1, c2);

    CELP_DEBUG(1, 
        "v3 Cross: "
        "[%i %i %i] X [%i %i %i] = [%i %i %i]",
        c1.x, c1.y, c1.z,
        c2.x, c2.y, c2.z,
        cr.x, cr.y, cr.z);
}
