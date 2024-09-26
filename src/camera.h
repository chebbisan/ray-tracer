#ifndef CAMERA_H
#define CAMERA_H

#include "hittable.h"
#include "material.h"

#include <thread>
#include <atomic>


class camera {
public:
    double aspect_ratio = 1.0;  // Ratio of image width over height
    int    image_width  = 100;  // Rendered image width in pixel count
    int samples_per_pixel = 10;
    int max_depth = 10;

    double vfov = 90;
    point3 lookfrom = point3(0, 0, 0);
    point3 lookat = point3(0, 0, -1);
    vec3 vup = vec3(0, 1, 0);

    double defocus_angle = 0;
    double focus_dist = 10;

    void render(const hittable& world) {
        initialize();

        std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

        for (int j = 0; j < image_height; j++) {
            std::clog << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush;
            for (int i = 0; i < image_width; i++) {
                color pixel_color = color(0, 0, 0);

                for (int sample = 0; sample < samples_per_pixel; ++sample) {
                    ray r = get_ray(i, j);
                    pixel_color += ray_color(r, max_depth, world);
                }
                write_color(std::cout, pixel_samples_scale * pixel_color);
            }
        }

        std::clog << "\rDone.                 \n";
    }

    void thread_render(const hittable& world, int num_threads) {
        initialize();
        int size = image_height;
        int chunk_size = size / num_threads;

        std::vector<std::string> data(image_height * image_width);
        std::vector<std::thread> threads;

        for (int i = 0; i < num_threads; ++i) {
            int start = i * chunk_size;
            int end = i == num_threads - 1 ? size : start + chunk_size;

            threads.emplace_back(&camera::thread_write, this, start, end, std::ref(world), std::ref(data));
        }

        for (auto& thread : threads) thread.join();

        write_to_file("../images/thread.ppm", data, image_width, image_height);

        std::clog << "\rDone.                 \n";
    }

    void thread_write(int start, int end, const hittable& world,
    std::vector<std::string>& data) {
        for (int j = start; j < end; ++j) {
            std::clog << "\rScanlines remaining: " << remain << ' ' << std::flush;
            for (int i = 0; i < image_width; ++i) {
                color pixel_color = color(0, 0, 0);
                for(int sample = 0; sample < samples_per_pixel; ++sample) {
                    ray r = get_ray(i, j);
                    pixel_color += ray_color(r, max_depth, world);
                }
                write_color(data, j * image_width + i, pixel_color * pixel_samples_scale);
            }
            --remain;
        }
    }

private:
    int    image_height;   // Rendered image height
    double pixel_samples_scale;
    point3 center;         // Camera center
    point3 pixel00_loc;    // Location of pixel 0, 0
    vec3   pixel_delta_u;  // Offset to pixel to the right
    vec3   pixel_delta_v;  // Offset to pixel below
    vec3 u, v, w;
    vec3 defocus_disk_u, defocus_disk_v;
    std::atomic<int> remain;

    void initialize() {
        image_height = int(image_width / aspect_ratio);
        image_height = (image_height < 1) ? 1 : image_height;

        remain = image_height;

        pixel_samples_scale = 1. / samples_per_pixel;

        center = lookfrom;

        // Determine viewport dimensions.
        auto theta = degrees_to_radians(vfov);
        auto h = std::tan(theta / 2);
        auto viewport_height = 2 * h * focus_dist;
        auto viewport_width = viewport_height * (double(image_width)/image_height);

        // calculate camera basis
        w = unit_vector(lookfrom - lookat);
        u = unit_vector(cross(vup, w));
        v = cross(w, u);

        // Calculate the vectors across the horizontal and down the vertical viewport edges.
        auto viewport_u = viewport_width * u;
        auto viewport_v = -viewport_height * v;

        // Calculate the horizontal and vertical delta vectors from pixel to pixel.
        pixel_delta_u = viewport_u / image_width;
        pixel_delta_v = viewport_v / image_height;

        // Calculate the location of the upper left pixel.
        auto viewport_upper_left =
            center - (focus_dist * w) - viewport_u/2 - viewport_v/2;
        pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);

        auto defocus_radius = focus_dist * std::tan(degrees_to_radians(defocus_angle) / 2);
        defocus_disk_u = u * defocus_radius;
        defocus_disk_v = v * defocus_radius;
    }

        color ray_color(const ray& r, int depth, const hittable& world) const {
        if (depth <= 0) return color(0., 0., 0.);

        hit_record rec;

        if (world.hit(r, interval(0.001, infinity), rec)) {
            ray scattered;
            color attenuation;
            if (rec.mat->scatter(r, rec, attenuation, scattered)) 
                return attenuation * ray_color(scattered, depth-1, world);
            return color(0, 0, 0);
        }

        vec3 unit_direction = unit_vector(r.direction());
        auto a = 0.5*(unit_direction.y() + 1.0);
        return (1.0-a)*color(1.0, 1.0, 1.0) + a*color(0.5, 0.7, 1.0);
    }

    ray get_ray(int i, int j) const {
        auto offset = sample_square();
        auto pixel_sample = pixel00_loc
                            + ((i + offset.x()) * pixel_delta_u)
                            + ((j + offset.y()) * pixel_delta_v);
        
        auto ray_origin = defocus_angle <= 0 ? center : defocus_disk_sample();
        auto ray_direction = pixel_sample - ray_origin;

        return ray(ray_origin, ray_direction);
    }

    vec3 sample_square() const {
        return vec3(random_double() - 0.5, random_double() - 0.5, 0);
    }

    point3 defocus_disk_sample() const {
        auto p = random_in_unit_disk();
        return center + (p[0] * defocus_disk_u) + (p[1] * defocus_disk_v);
    }
};

#endif