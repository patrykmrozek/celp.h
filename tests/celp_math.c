#define CELP_IMPLEMENTATION
#define CELP_STRIP_PREFIX
#include "../celp.h"
#include <stdio.h>

int main() {
    celp_log(CELP_LOG_LEVEL_DEBUG, "MATH");

    Vec2i w = {2, 2};
    Vec2i e = {3, 3};
    Vec2i r = celp_vec2_add(w, e);
    celp_log(CELP_LOG_LEVEL_DEBUG,
        "[%i %i] + [%i %i] = [%i %i]",
        w.x, w.y, e.x, e.y, r.x, r.y);

    Vec2i w2 = {2, 2};
    Vec2i e2 = {3, 3};
    Vec2i r2 = celp_vec2_sub(w2, e2);
    celp_log(CELP_LOG_LEVEL_DEBUG,
        "[%i %i] - [%i %i] = [%i %i]",
        w2.x, w2.y, e2.x, e2.y, r2.x, r2.y);

    int r3 = celp_vec2_dot(w, e);
    celp_log(CELP_LOG_LEVEL_DEBUG,
        "[%i %i] . [%i %i] = %i",
        w.x, w.y, e.x, e.y, r3);

    Vec3i a = {1, 1, 1};
    Vec3i b = {1, 1, 1};
    Vec3i res = celp_vec3_add(a, b);

    celp_log(CELP_LOG_LEVEL_DEBUG,
        "[%i %i %i] + [%i %i %i] = [%i %i %i]",
        a.x, a.y, a.z, b.x, b.y, b.z, res.x, res.y, res.z);

    Vec3i a2 = {4, 4, 4};
    Vec3i b2 = {1, 1, 1};
    Vec3i res2 = celp_vec3_sub(a2, b2);

    celp_log(CELP_LOG_LEVEL_DEBUG,
        "[%i %i %i] - [%i %i %i] = [%i %i %i]",
        a2.x, a2.y, a2.z, b2.x, b2.y, b2.z, res2.x, res2.y, res2.z);

    int res3 = celp_vec3_dot(a2, b2);

    celp_log(CELP_LOG_LEVEL_DEBUG,
        "[%i %i %i] . [%i %i %i] = %i",
        a2.x, a2.y, a2.z, b2.x, b2.y, b2.z, res3);
}
