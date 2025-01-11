#include "hittable_list.h"
#include "rtweekend.h"
#include "hittable.h"
#include "sphere.h"
#include "camera.h"
#include "material.h"
#include "bvh.h"
#include "quad.h"
#include <chrono>
#include <ctime>

using namespace std;

void boucing_spheres()
{

    srand(time(NULL));

    hittable_list world;

    auto checker = make_shared<checker_texture>(0.32, color(0.2, 0.3, 0.1), color(0.9, 0.9, 0.9));
    auto ground_material = make_shared<lambertian>(checker);
    world.add(make_shared<sphere>(point3(0, -1000, 0), 1000, ground_material));

    for (int a = -11; a < 11; a++)
    {
        for (int b = -11; b < 11; b++)
        {
            auto choose_mat = random_double();
            point3 center(a + 0.9 * random_double(), 0.2, b + 0.9 * random_double());

            if ((center - point3(4, 0.2, 0)).length() > 0.9)
            {
                shared_ptr<material> sphere_material;

                if (choose_mat < 0.5)
                {
                    // diffuse
                    auto albedo = random_() * random_();
                    sphere_material = make_shared<lambertian>(albedo);
                    // auto center2 = center + vec3(0, random_double(0, 0.5), 0);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                }
                else if (choose_mat < 0.7)
                {
                    // metal
                    auto albedo = random_(0.5, 1);
                    auto fuzz = random_double(0, 0.5);
                    sphere_material = make_shared<metal>(albedo, fuzz);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                }
                else if (choose_mat < 0.85)
                {
                    // glass
                    sphere_material = make_shared<dielectric>(1.5);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                }
                // glass and bubble with different sizes
                else if (choose_mat < 0.9)
                {
                    sphere_material = make_shared<dielectric>(1.5);
                    shared_ptr<material> sphere_material2 = make_shared<dielectric>(1 / 1.5);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                    world.add(make_shared<sphere>(center, 0.18, sphere_material2));
                }
                // glass and bubble with different sizes
                else if (choose_mat < 0.95)
                {
                    sphere_material = make_shared<dielectric>(1.5);
                    shared_ptr<material> sphere_material2 = make_shared<dielectric>(1 / 1.5);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                    world.add(make_shared<sphere>(center, 0.15, sphere_material2));
                }
                // glass and bubble with different sizes
                else
                {
                    // glass and bubble
                    sphere_material = make_shared<dielectric>(1.5);
                    shared_ptr<material> sphere_material2 = make_shared<dielectric>(1 / 1.5);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                    world.add(make_shared<sphere>(center, 0.1, sphere_material2));
                }
            }
        }
    }

    auto material1 = make_shared<dielectric>(1.5);
    world.add(make_shared<sphere>(point3(0, 1, 0), 1.0, material1));

    auto material2 = make_shared<lambertian>(color(0.4, 0.2, 0.1));
    world.add(make_shared<sphere>(point3(-4, 1, 0), 1.0, material2));

    auto material3 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
    world.add(make_shared<sphere>(point3(4, 1, 0), 1.0, material3));

    world = hittable_list(make_shared<bvh_node>(world));

    camera cam;

    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width = 800;
    cam.samples_per_pixel = 50;
    cam.max_depth = 30;

    cam.vfov = 20;
    cam.lookfrom = point3(13, 2, 3);
    cam.lookat = point3(0, 0, 0);
    cam.vup = vec3(0, 1, 0);

    cam.defocus_angle = 0;
    cam.focus_dist = 10.0;

    cam.render(world);
}

void earth()
{
    auto earth_texture = make_shared<image_texture>("src/textures/earthmap.jpg");
    auto earth_surface = make_shared<lambertian>(earth_texture);
    auto globe = make_shared<sphere>(point3(0, 0, 0), 2, earth_surface);

    camera cam;

    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width = 400;
    cam.samples_per_pixel = 100;
    cam.max_depth = 50;

    cam.vfov = 20;
    cam.lookfrom = point3(0, 0, 12);
    cam.lookat = point3(0, 0, 0);
    cam.vup = vec3(0, 1, 0);

    cam.defocus_angle = 0;

    cam.render(hittable_list(globe));
}
void quads()
{
    hittable_list world;

    // Materials
    auto left_red = make_shared<lambertian>(color(1.0, 0.2, 0.2));
    auto back_green = make_shared<lambertian>(color(0.2, 1.0, 0.2));
    auto right_blue = make_shared<lambertian>(color(0.2, 0.2, 1.0));
    auto upper_orange = make_shared<lambertian>(color(1.0, 0.5, 0.0));
    auto lower_teal = make_shared<lambertian>(color(0.2, 0.8, 0.8));

    // Quads
    world.add(make_shared<quad>(point3(-3, -2, 5), vec3(0, 0, -4), vec3(0, 4, 0), left_red));
    world.add(make_shared<quad>(point3(-2, -2, 0), vec3(4, 0, 0), vec3(0, 4, 0), back_green));
    world.add(make_shared<quad>(point3(3, -2, 1), vec3(0, 0, 4), vec3(0, 4, 0), right_blue));
    world.add(make_shared<quad>(point3(-2, 3, 1), vec3(4, 0, 0), vec3(0, 0, 4), upper_orange));
    world.add(make_shared<quad>(point3(-2, -3, 5), vec3(4, 0, 0), vec3(0, 0, -4), lower_teal));

    camera cam;

    cam.aspect_ratio = 1.0;
    cam.image_width = 400;
    cam.samples_per_pixel = 100;
    cam.max_depth = 50;

    cam.vfov = 80;
    cam.lookfrom = point3(0, 0, 9);
    cam.lookat = point3(0, 0, 0);
    cam.vup = vec3(0, 1, 0);

    cam.defocus_angle = 0;

    cam.render(world);
}

int main()
{
    auto start = chrono::high_resolution_clock::now();
    switch (5)
    {
    case 1:
        boucing_spheres();
        break;
    case 3:
        earth();
        break;
    case 5:
        quads();
        break;
    }
    auto end = chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    clog << "Elapsed time: " << elapsed.count() << " s\n";
    return 0;
}