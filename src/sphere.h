#pragma once

#include "hittable.h"
#include "rtweekend.h"

class sphere : public hittable
{
private:
    point3 center;
    double radius;
    shared_ptr<material> mat_ptr;

public:
    sphere(const point3 &center, double radius, shared_ptr<material> mat) : center(center), radius(std::fmax(0, radius)), mat_ptr(mat) {}

    bool hit(const ray &r, interval ray_t, hit_record &rec) const override;
};

bool sphere::hit(const ray &r, interval ray_t, hit_record &rec) const
{
    vec3 oc = center - r.origin();
    auto a = r.direction().length_squared();
    auto h = dot(oc, r.direction());
    auto c = oc.length_squared() - radius * radius;

    auto discriminant = h * h - a * c;
    if (discriminant < 0)
    {
        return false;
    }

    auto sqrtd = sqrt(discriminant);

    // Find the nearest root that lies in the acceptable range.
    auto root = (h - sqrtd) / a;
    // std::cerr << root << ' ';
    if (!ray_t.surrounds(root))
    {
        root = (h + sqrtd) / a;
        if (!ray_t.surrounds(root))
        {
            return false;
        }
    }

    rec.t = root;
    rec.p = r.at(rec.t);
    vec3 outward_normal = (rec.p - center) / radius;
    rec.set_face_normal(r, outward_normal);
    rec.mat_ptr = mat_ptr;

    return true;
};