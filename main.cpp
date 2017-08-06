// Add by yaukey at 2017-07-30.
// Main entry.

#include <iostream>
#include <fstream>
#include "sphere.h"
#include "hitable_list.h"
#include "camera.h"
#include "material.h"
#include "float.h"

vec3 color(const ray& r, hitable* world, int depth)
{
    hit_record rec;
    if (world->hit(r, 0.001f, FLT_MAX, rec))
    {
        ray scattered;
        vec3 attenuation;
        if (depth < 50 && rec.mat_ptr->scatter(r, rec, attenuation, scattered))
        {
            return attenuation * color(scattered, world, depth + 1);
        }
        else
        {
            return vec3(0.0f, 0.0f, 0.0f);
        }
    }
    else
    {
        vec3 unit_direction = unit_vector(r.direction());
        float t = 0.5 * (unit_direction.y() + 1.0f);
        return (1.0f - t) * vec3(1.0f, 1.0f, 1.0f) + t * vec3(0.5f, 0.7f, 1.0f);
    }
}

hitable* random_scene()
{
    int n = 500;
    hitable** list = new hitable*[n + 1];
    list[0] = new sphere(vec3(0.0f, -1000.0f, 0.0f), 1000.0f, new lambertian(vec3(0.5f, 0.5f, 0.5f)));
    
    int i = 1;
    for (int a = -11; a < 11; a++)
    {
        for (int b = -11; b < 11; b++)
        {
            float choose_mat = drand48();
            vec3 center(a + 0.9f * drand48(), 0.2f, b + 0.9f * drand48());
            if ((center - vec3(4.0f, 0.2f, 0.0f)).length() > 0.9f)
            {
                if (choose_mat < 0.8f)
                {
                    // Diffuse.
                    list[i++] = new sphere(center, 0.2f, 
                        new lambertian(vec3(drand48() * drand48(), drand48() * drand48(), drand48() * drand48())));
                }
                else if (choose_mat < 0.95)
                {
                    // Metal.
                    list[i++] = new sphere(center, 0.2f,
                        new metal(vec3(0.5f * (1.0f + drand48()), 0.5f * (1.0f + drand48()), 0.5f * (1.0f + drand48())), 0.5f * drand48()));
                }
                else
                {
                    // Glass.
                    list[i++] = new sphere(center, 0.2f, new dielectric(1.5f));
                }
            }
        }
    }

    list[i++] = new sphere(vec3(0.0f, 1.0f, 0.0f), 1.0f, new dielectric(1.5f));
    list[i++] = new sphere(vec3(-4.0f, 1.0f, 0.0f), 1.0f, new lambertian(vec3(0.4f, 0.2f, 0.1f)));
    list[i++] = new sphere(vec3(4.0f, 1.0f, 0.0f), 1.0f, new metal(vec3(0.7f, 0.6f, 0.5f), 0.0f));

    return new hitable_list(list, i);
}

int main()
{
    int nx = 1200;
    int ny = 800;
    int ns = 10;

    const char* file_name = "result.ppm";
    std::ofstream result;
    result.open(file_name);
    result << "P3\n" << nx << " " << ny << "\n255\n";

    // // Dielectric material.
    // hitable* list[5];
    // list[0] = new sphere(vec3(0.0f, 0.0f, -1.0f), 0.5f, new lambertian(vec3(0.1f, 0.2f, 0.5f)));
    // list[1] = new sphere(vec3(0.0f, -100.5f, -1.0f), 100.0f, new lambertian(vec3(0.8f, 0.8f, 0.0f)));
    // list[2] = new sphere(vec3(1.0f, 0.0f, -1.0f), 0.5f, new metal(vec3(0.8f, 0.6f, 0.2f)));
    // list[3] = new sphere(vec3(-1.0f, 0.0f, -1.0f), 0.5f, new dielectric(1.5f));
    // list[4] = new sphere(vec3(-1.0f, 0.0f, -1.0f), -0.45f, new dielectric(1.5f));

    // // Positionable camera test.
    // hitable* list[2];
    // float R = cos(M_PI / 4.0f);
    // list[0] = new sphere(vec3(-R, 0.0f, -1.0f), R, new lambertian(vec3(0.0f, 0.0f, 1.0f)));
    // list[1] = new sphere(vec3(R, 0.0f, -1.0f), R, new lambertian(vec3(1.0f, 0.0f, 0.0f)));

    //hitable* world = new hitable_list(list, 5);

    // The cover page scene.
    hitable* world = random_scene();

    vec3 lookfrom(13.0f, 2.0f, 3.0f);
    vec3 lookat(0.0f, 0.0f, 0.0f);
    float dist_to_focus = 10.0f;
    float aperture = 0.1f;
    camera cam(lookfrom, lookat, vec3(0.0f, 1.0f, 0.0f), 20.0f, float(nx) / float(ny), aperture, dist_to_focus);

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
                col += color(r, world, 0);
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
