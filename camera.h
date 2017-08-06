// Add by yaukey at 2017-07-30.
// Camera class.

#ifndef CAMERAH
#define CAMERAH

#include "ray.h"

vec3 random_in_unit_disk()
{
    vec3 p;
    do
    {
        p = 2.0f * vec3(drand48(), drand48(), 0.0f) - vec3(1.0f, 1.0f, 0.0f);
    } while (dot(p, p) >= 1.0f);

    return p;
}

class camera
{
public:
    // vfov is top to bottom in degrees.
    camera(vec3 lookfrom, vec3 lookat, vec3 vup, float vfov, float aspect, float aperture, float focus_dist)
    {
        lens_radius = aperture / 2.0f;

        float theta = (vfov * M_PI) / 180.0f;
        float half_height = tan(theta / 2.0f);
        float half_width = half_height * aspect;

        origin = lookfrom;
        w = unit_vector(lookfrom - lookat);
        u = unit_vector(cross(vup, w));
        v = cross(w, u);

        lower_left_corner = origin - half_width * focus_dist * u - half_height * focus_dist * v - focus_dist * w;
        horizontal = half_width * 2.0f * focus_dist * u;
        vertical = half_height * 2.0f * focus_dist * v;
    }

    ray get_ray(float s, float t) 
    { 
        vec3 rd = lens_radius * random_in_unit_disk();
        vec3 offset = u * rd.x() + v * rd.y();
        return ray(origin + offset, lower_left_corner + horizontal * s + vertical * t - origin - offset); 
    }

    vec3 origin;
    vec3 lower_left_corner;
    vec3 horizontal;
    vec3 vertical;
    vec3 u, v, w;
    float lens_radius;
};

#endif
