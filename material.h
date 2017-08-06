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

bool refract(const vec3& v, const vec3& n, float ni_over_nt, vec3& refracted)
{
    vec3 uv = unit_vector(v);
    float dt = dot(uv, n);
    float discriminant = 1.0f - ni_over_nt * ni_over_nt * (1.0f - dt * dt);
    if (discriminant > 0)
    {
        refracted = ni_over_nt * (uv - n * dt) - n * sqrt(discriminant);
        return true;
    }
    else
    {
        return false;
    }
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

class dielectric : public material
{
public:
    dielectric(float ri) : ref_idx(ri) {}

    virtual bool scatter(const ray& r, const hit_record& rec, vec3& attenuation, ray& scattered) const
    {
        vec3 outward_normal;
        vec3 reflected = reflect(r.direction(), rec.normal);

        float ni_over_nt;
        attenuation = vec3(1.0f, 1.0f, 1.0f);
        vec3 refracted;
        if (dot(r.direction(), rec.normal) > 0.0f)
        {
            outward_normal = -rec.normal;
            ni_over_nt = ref_idx;
        }
        else
        {
            outward_normal = rec.normal;
            ni_over_nt = 1.0f / ref_idx;
        }

        if (refract(r.direction(), outward_normal, ni_over_nt, refracted))
        {
            scattered = ray(rec.p, refracted);
            return true;
        }
        else
        {
            scattered = ray(rec.p, reflected);
            return false;
        }
    }

    float ref_idx;
};

#endif
