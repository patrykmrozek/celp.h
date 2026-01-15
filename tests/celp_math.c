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

    Vec3i a = {1, 1, 1};
    Vec3i b = {1, 1, 1};
    Vec3i res = celp_vec3_add(a, b);

    celp_log(CELP_LOG_LEVEL_DEBUG,
        "[%i %i %i] + [%i %i %i] = [%i %i %i]",
        a.x, a.y, a.z, b.x, b.y, b.z, res.x, res.y, res.z);
}
