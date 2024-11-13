#pragma once

#include "hittable.h"
#include "material.h"

class camera
{
public:
    double aspect_ratio = 16.0 / 9.0;
    int image_width = 400;
    int samples_per_pixel = 1;
    int max_depth = 10;

    void render(const hittable &world)
    {
        initialize();
        cout << "P3\n"
             << image_width << ' ' << image_height << "\n255\n";

        for (int j = 0; j < image_height; j++)
        {
            clog << "\rScanlines remaining: " << (image_height - j) << " " << flush;
            for (int i = 0; i < image_width; i++)
            {
                color pixel_color(0, 0, 0);
                for (int sample = 0; sample < samples_per_pixel; sample++)
                {
                    ray r = get_ray(i, j);
                    pixel_color += ray_color(r, max_depth, world);
                }
                write_color(cout, pixel_color * pixel_sample_scale);
            }
        }
        clog << "\rDone.                     \n";
    }

private:
    int image_height;
    point3 center;
    point3 pixel00_loc;
    vec3 pixel_delta_u;
    vec3 pixel_delta_v;
    double pixel_sample_scale;

    void initialize()
    {
        image_height = int(image_width / aspect_ratio);
        image_height = (image_height > 1) ? image_height : 1;

        pixel_sample_scale = 1.0 / samples_per_pixel;
        center = point3(0, 0, 0);

        auto focal_length = 1.0;
        double viewport_height = 2.0;
        double viewport_width = (double(image_width) / image_height) * viewport_height;

        auto viewport_u = vec3(viewport_width, 0, 0);
        auto viewport_v = vec3(0, -viewport_height, 0);

        pixel_delta_u = viewport_u / image_width;
        pixel_delta_v = viewport_v / image_height;

        auto viewport_upper_left_corner = vec3(-viewport_width / 2, viewport_height / 2, -focal_length);
        pixel00_loc = viewport_upper_left_corner + 0.5 * (pixel_delta_u + pixel_delta_v);
    }

    ray get_ray(int i, int j) const
    {
        vec3 offset = vec3(random_double() - 0.5, random_double() - 0.5, 0);
        auto pixel_sample = pixel00_loc + ((i + offset.x()) * pixel_delta_u) + ((j + offset.y()) * pixel_delta_v);
        auto ray_origin = center;
        auto ray_direction = pixel_sample - ray_origin;
        return ray(ray_origin, ray_direction);
    }

    color ray_color(const ray &r, int depth, const hittable &world) const
    {
        if (depth <= 0)
            return color(0, 0, 0);
        hit_record rec;
        if (world.hit(r, interval(0.001, infinity), rec))
        {
            ray scattered;
            color attenuation;
            if (rec.mat_ptr->scatter(r, rec, attenuation, scattered))
            {
                return attenuation * ray_color(scattered, depth - 1, world);
            }
            return color(0, 0, 0);
        }

        vec3 unit_direction = unit_vector(r.direction());
        auto a = 0.5 * (unit_direction.y() + 1.0);
        return (1.0 - a) * color(1.0, 1.0, 1.0) + a * color(0.5, 0.7, 1.0);
    }
};