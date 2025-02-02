#pragma once

#include "hittable.h"
#include "texture.h"

class material
{
public:
    virtual ~material() = default;
    virtual color emitted(double u, double v, const point3 &p) const
    {
        return color(0, 0, 0);
    }

    virtual bool scatter(const ray &r_in, const hit_record &rec, color &attenuation, ray &scattered) const = 0;
    virtual double scattering_pdf(const ray &r_in, const hit_record &rec, const ray &scattered) const
    {
        return 0;
    }
};

class lambertian : public material
{
private:
    shared_ptr<texture> tex;

public:
    lambertian(const color &aldebo) : tex(make_shared<solid_color>(aldebo)) {}
    lambertian(shared_ptr<texture> tex) : tex(tex) {}

    bool scatter(const ray &r_in, const hit_record &rec, color &attenuation, ray &scattered) const override
    {
        vec3 scatter_direction = rec.normal + random_unit_vector();
        if (scatter_direction.near_zero())
        {
            scatter_direction = rec.normal;
        }
        scattered = ray(rec.p, scatter_direction, r_in.time());
        attenuation = tex->value(rec.u, rec.v, rec.p);
        return true;
    }
    double scattering_pdf(const ray &r_in, const hit_record &rec, const ray &scattered)
        const override
    {
        auto cos_theta = dot(rec.normal, unit_vector(scattered.direction()));
        return cos_theta < 0 ? 0 : cos_theta / pi;
    }
};

class metal : public material
{
private:
    color albedo;
    double fuzz;

public:
    metal(const color &a, double fuzz) : albedo(a), fuzz(fuzz < 1 ? fuzz : 1) {}

    bool scatter(const ray &r_in, const hit_record &rec, color &attenuation, ray &scattered) const override
    {
        vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
        reflected = unit_vector(reflected) + fuzz * random_unit_vector();
        scattered = ray(rec.p, reflected, r_in.time());
        attenuation = albedo;
        return (dot(scattered.direction(), rec.normal) > 0);
    }
};

class dielectric : public material
{
private:
    double refraction_index; // Index of Refraction

    static double reflectance(double cosine, double ref_idx)
    {
        // Use Schlick's approximation for reflectance.
        double r0 = (1 - ref_idx) / (1 + ref_idx);
        r0 = r0 * r0;
        return r0 + (1 - r0) * pow((1 - cosine), 5);
    }

public:
    dielectric(double ri) : refraction_index(ri) {}

    bool scatter(const ray &r_in, const hit_record &rec, color &attenuation, ray &scattered) const override
    {
        attenuation = color(1.0, 1.0, 1.0);
        double etai_over_etat = rec.front_face ? (1.0 / refraction_index) : refraction_index;

        vec3 unit_direction = unit_vector(r_in.direction());
        double cos_theta = fmin(dot(-unit_direction, rec.normal), 1.0);
        double sin_theta = sqrt(1.0 - cos_theta * cos_theta);

        bool cannot_refract = etai_over_etat * sin_theta > 1.0;
        vec3 direction;
        if (cannot_refract || reflectance(cos_theta, etai_over_etat) > random_double())
        {
            direction = reflect(unit_direction, rec.normal);
        }
        else
        {
            direction = refract(unit_direction, rec.normal, etai_over_etat);
        }

        scattered = ray(rec.p, direction, r_in.time());
        return true;
    }
};

class diffuse_light : public material
{
private:
    shared_ptr<texture> tex;

public:
    diffuse_light(shared_ptr<texture> tex) : tex(tex) {}
    diffuse_light(const color &emit) : tex(make_shared<solid_color>(emit)) {}

    color emitted(double u, double v, const point3 &p) const override
    {
        return tex->value(u, v, p);
    }
    bool scatter(const ray &r_in, const hit_record &rec, color &attenuation, ray &scattered) const override
    {
        return false;
    }
};