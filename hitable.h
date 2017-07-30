// Add by yaukey at 2017-07-30.
// hitable base class.

#ifndef HITABLEH
#define HITABLEH

#include "ray.h"

struct hit_record
{
    float t;
    vec3 p;
    vec3 normal;
};

class hitable
{
public:
    virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) = 0;
};

#endif
