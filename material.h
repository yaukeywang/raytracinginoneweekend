// Add by yaukey at 2017-07-30.
// Material class.

#ifndef MATERIALH
#define MATERIALH

#include "ray.h"
#include "hitable.h"

vec3 random_in_unit_sphere()
{
    vec3 p;
    do
    {
        p = 2.0f * vec3(drand48(), drand48(), drand48()) - 1.0f;
    } while (dot(p, p) >= 1.0f);

    return p;
}

vec3 reflect(const vec3& v, const vec3& n)
{
    return v - 2.0f * dot(v, n) * n;
}

class material
{
public:
    virtual bool scatter(const ray& r, const hit_record& rec, vec3& attenuation, ray& scattered) const = 0;
};

class lambertian : public material
{
public:
    lambertian(const vec3& a) { albedo = a; }

    virtual bool scatter(const ray& r, const hit_record& rec, vec3& attenuation, ray& scattered) const
    {
        vec3 target = rec.p + rec.normal + random_in_unit_sphere();
        scattered = ray(rec.p, target - rec.p);
        attenuation = albedo;

        return true;
    }

    vec3 albedo;
};

class metal : public material
{
public:
    metal(const vec3& a) : albedo(a) {}

    virtual bool scatter(const ray& r, const hit_record& rec, vec3& attenuation, ray& scattered) const
    {
        vec3 reflected = reflect(unit_vector(r.direction()), rec.normal);
        scattered = ray(rec.p, reflected);
        attenuation = albedo;
        
        return dot(scattered.direction(), rec.normal) > 0.0f;
    }

    vec3 albedo;
};

#endif
