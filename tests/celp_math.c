#define CELP_IMPLEMENTATION
#define CELP_STRIP_PREFIX
#include "../celp.h"
#include <stdio.h>

int main() {
    celp_log(CELP_LOG_LEVEL_DEBUG, "MATH");

    Vec2i_t w = {2, 2};
    Vec2i_t e = {3, 3};
    Vec2i_t r = celp_vec2_add(w, e);
    celp_log(CELP_LOG_LEVEL_DEBUG,
        "Vec2 Add: "
        "[%i %i] + [%i %i] = [%i %i]",
        w.x, w.y, e.x, e.y, r.x, r.y);

    Vec2i_t w2 = {2, 2};
    Vec2i_t e2 = {3, 3};
    Vec2i_t r2 = celp_vec2_sub(w2, e2);
    celp_log(CELP_LOG_LEVEL_DEBUG,
        "Vec2 Sub: "
        "[%i %i] - [%i %i] = [%i %i]",
        w2.x, w2.y, e2.x, e2.y, r2.x, r2.y);

    int r3 = celp_vec2_dot(w, e);
    celp_log(CELP_LOG_LEVEL_DEBUG,
        "Vec2 Dot: "
        "[%i %i] . [%i %i] = %i",
        w.x, w.y, e.x, e.y, r3);

    int scale = 2;
    Vec2i_t r4 = celp_vec2_scale(w2, scale);
    celp_log(CELP_LOG_LEVEL_DEBUG,
        "Vec2 Scale: "
        "[%i %i] * %i = [%i %i]",
        w2.x, w2.y, scale, r4.x, r4.y);

    Vec3i_t a = {1, 1, 1};
    Vec3i_t b = {1, 1, 1};
    Vec3i_t res = celp_vec3_add(a, b);

    celp_log(CELP_LOG_LEVEL_DEBUG,
        "Vec3 Add: "
        "[%i %i %i] + [%i %i %i] = [%i %i %i]",
        a.x, a.y, a.z, b.x, b.y, b.z, res.x, res.y, res.z);

    Vec3i_t a2 = {4, 4, 4};
    Vec3i_t b2 = {1, 1, 1};
    Vec3i_t res2 = celp_vec3_sub(a2, b2);

    celp_log(CELP_LOG_LEVEL_DEBUG,
        "Vec3 Sub: "
        "[%i %i %i] - [%i %i %i] = [%i %i %i]",
        a2.x, a2.y, a2.z, b2.x, b2.y, b2.z, res2.x, res2.y, res2.z);

    int res3 = celp_vec3_dot(a2, b2);

    celp_log(CELP_LOG_LEVEL_DEBUG,
        "Vec3 Sub: "
        "[%i %i %i] . [%i %i %i] = %i",
        a2.x, a2.y, a2.z, b2.x, b2.y, b2.z, res3);

    int scale2 = 4;
    Vec3i_t r5 = celp_vec3_scale(a2, scale2);

    celp_log(CELP_LOG_LEVEL_DEBUG,
        "Vec3 Scale: "
        "[%i %i %i] * %i = [%i %i %i]",
        a2.x, a2.y, a2.z, scale2, r5.x, r5.y, r5.z);

}
