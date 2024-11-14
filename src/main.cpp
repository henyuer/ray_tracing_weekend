#include "hittable_list.h"
#include "rtweekend.h"
#include "hittable.h"
#include "sphere.h"
#include "camera.h"
#include "material.h"
#include <chrono>

using namespace std;

int main()
{
    auto start = chrono::high_resolution_clock::now();

    hittable_list world;
    auto material_ground = make_shared<lambertian>(color(0.8, 0.8, 0.0));
    auto material_center = make_shared<lambertian>(color(0.1, 0.2, 0.5));
    auto material_left = make_shared<metal>(color(0.8, 0.8, 0.8));
    auto material_right = make_shared<metal>(color(0.8, 0.6, 0.2));

    world.add(make_shared<sphere>(point3(0, -100.5, -1), 100, material_ground));
    world.add(make_shared<sphere>(point3(0, 0, -1.2), 0.5, material_center));
    world.add(make_shared<sphere>(point3(-1, 0, -1), 0.5, material_left));
    world.add(make_shared<sphere>(point3(1, 0.5, -1.5), 0.5, material_right));

    camera cam;
    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width = 800;
    cam.samples_per_pixel = 100;
    cam.max_depth = 50;

    cam.render(world);

    auto end = chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    cerr << "Elapsed time: " << elapsed.count() << " s\n";

    return 0;
}
