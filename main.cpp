// Add by yaukey at 2017-07-30.
// Main entry.

#include <iostream>
#include <fstream>
#include "sphere.h"
#include "hitable_list.h"
#include "camera.h"
#include "float.h"

vec3 random_in_unit_sphere()
{
    vec3 p;
    do
    {
        p = 2.0f * vec3(drand48(), drand48(), drand48()) - 1.0f;
    } while (dot(p, p) >= 1.0f);

    return p;
}

vec3 color(const ray& r, hitable* world)
{
    hit_record rec;
    if (world->hit(r, 0.0f, FLT_MAX, rec))
    {
        vec3 target = rec.p + rec.normal + random_in_unit_sphere();
        return 0.5f * color(ray(rec.p, target - rec.p), world);
    }
    else
    {
        vec3 unit_direction = unit_vector(r.direction());
        float t = 0.5 * (unit_direction.y() + 1.0f);
        return (1.0f - t) * vec3(1.0f, 1.0f, 1.0f) + t * vec3(0.5f, 0.7f, 1.0f);
    }
}

int main()
{
    int nx = 200;
    int ny = 100;
    int ns = 100;


    const char* file_name = "result.ppm";
    std::ofstream result;
    result.open(file_name);
    result << "P3\n" << nx << " " << ny << "\n255\n";

    hitable* list[2];
    list[0] = new sphere(vec3(0.0f, 0.0f, -1.0f), 0.5f);
    list[1] = new sphere(vec3(0.0f, -100.5f, -1.0f), 100.0f);

    hitable* world = new hitable_list(list, 2);
    camera cam;

    for (int j = ny - 1; j >= 0; j--)
    {
        for (int i = 0; i < nx; i++)
        {
            vec3 col(0.0f, 0.0f, 0.0f);
            for (int s = 0; s < ns; s++)
            {
                float u = float(i + drand48()) / float(nx);
                float v = float(j + drand48()) / float(ny);
                ray r = cam.get_ray(u, v);
                vec3 p = r.point_at_parameter(2.0f);
                col += color(r, world);
            }

            col /= float(ns);
            col = vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));
            int ir = int(255.99 * col[0]);
            int ig = int(255.99 * col[1]);
            int ib = int(255.99 * col[2]);

            result << ir << " " << ig << " " << ib << "\n";
        }
    }

    result.close();

    // Caution: need to fix memory leak!
    // ...

    return 0;
}
