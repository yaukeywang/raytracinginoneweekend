// Add by yaukey at 2017-07-30.
// Camera class.

#ifndef CAMERAH
#define CAMERAH

#include "ray.h"

class camera
{
public:
    camera()
    {
        origin = vec3(0.0f, 0.0f, 0.0f);
        lower_left_corner = vec3(-2.0f, -1.0f, -1.0f);
        horizontal = vec3(4.0f, 0.0f, 0.0f);
        vertical = vec3(0.0f, 2.0f, 0.0f);
    }

    ray get_ray(float u, float v) { return ray(origin, lower_left_corner + horizontal * u + vertical * v - origin); }

    vec3 origin;
    vec3 lower_left_corner;
    vec3 horizontal;
    vec3 vertical;
};

#endif
