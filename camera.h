// Add by yaukey at 2017-07-30.
// Camera class.

#ifndef CAMERAH
#define CAMERAH

#include "ray.h"

class camera
{
public:
    // vfov is top to bottom in degrees.
    camera(vec3 lookfrom, vec3 lookat, vec3 vup, float vfov, float aspect)
    {
        float theta = (vfov * M_PI) / 180.0f;
        float half_height = tan(theta / 2.0f);
        float half_width = half_height * aspect;

        origin = lookfrom;
        vec3 w = unit_vector(lookfrom - lookat);
        vec3 u = unit_vector(cross(vup, w));
        vec3 v = cross(w, u);

        lower_left_corner = origin - half_width * u - half_height * v - w;
        horizontal = half_width * 2.0f * u;
        vertical = half_height * 2.0f * v;
    }

    ray get_ray(float u, float v) { return ray(origin, lower_left_corner + horizontal * u + vertical * v - origin); }

    vec3 origin;
    vec3 lower_left_corner;
    vec3 horizontal;
    vec3 vertical;
};

#endif
