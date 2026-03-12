#ifndef CAMERA_H
#define CAMERA_H

#include "hittable.h"
#include "material.h"
#include "file_writer.h"

class camera {
    private:
        int image_height;           // Rendered image height
        double pixel_samples_scale; // Color scale factor for a sum of pixel samples
        point3 center;              // Camera center
        point3 pixel00_loc;         // Location of pixel (0, 0)
        vec3 pixel_delta_u;         // Offset to pixel to the right
        vec3 pixel_delta_v;         // Offset to pixel below
        vec3 u, v, w;               //Camera frame basis vectors
        vec3 defocus_disk_u;        //Defocus disk horizontal radius
        vec3 defocus_disk_v;        //Defocus disk vertical radius

        void initialize() {
            image_height = int(image_width / aspect_ratio);
            image_height = (image_height < 1) ? 1 : image_height;

            pixel_samples_scale = 1.0 / samples_per_pixel;

            center = lookfrom;

            // Determine viewport dimensions.
            auto theta = degrees_to_radians(vert_fov);
            auto h = std::tan(theta/2);
            auto viewpoint_height = 2 * h * focus_dist;
            auto viewpoint_width = viewpoint_height * (double(image_width)/image_height);

            //calc u,v,w basis vectors for the cam coordinate frame
            w = unit_vector(lookfrom - lookat);
            u = unit_vector(cross(vup, w));
            v = cross(w, u);


            //calc the vectors across the horizontal(u) and down the vertical(v) viewport edges
            auto viewport_u = viewpoint_width  *  u;
            auto viewport_v = viewpoint_height * -v;

            //calc delta vectors from pixel to pixel
            pixel_delta_u = viewport_u / image_width;
            pixel_delta_v = viewport_v / image_height;

            //calc the location of the upper left pixel
            auto viewport_upper_left = center - focus_dist * w - viewport_u/2 - viewport_v/2;
            pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);

            //calc the camera defocus disk basis vectors
            auto defocus_radius = focus_dist * std::tan(degrees_to_radians(defocus_angle / 2));
            defocus_disk_u = u * defocus_radius;
            defocus_disk_v = v * defocus_radius;
        }

        color ray_color(const ray& r, int depth, const hittable& world) const {
            if (depth <=0 )
                return color(0, 0, 0);

            hit_record rec;

            if (world.hit(r, interval(0.001, infinity), rec)) {
                ray scattered;
                color attenuation;

                if (rec.mat->scatter(r, rec, attenuation, scattered))
                    return attenuation * ray_color(scattered, depth - 1, world);
                return color(0, 0, 0);
            }

            vec3 unit_direction = unit_vector(r.direction());
            auto a = 0.5 * (unit_direction.y() + 1.0);
            return (1.0 - a) * color(1.0, 1.0, 1.0) + a * color(0.5, 0.7, 1.0);
                }

        ray get_ray(int i, int j) {
            // Construct a camera ray originating from the defocus disk and directed at a randomly sampled point around the pixel location i, j.
            auto offset = sample_square();
            auto pixel_sample = pixel00_loc + ((i + offset.x()) * pixel_delta_u) + ((j + offset.y()) * pixel_delta_v);

            auto ray_origin = (defocus_angle <= 0) ? center : defocus_disk_sample();
            auto ray_direction = pixel_sample - ray_origin;
            auto ray_time = random_double();

            
            return ray(ray_origin, ray_direction, ray_time);
        }

        vec3 sample_square() const {
            // Returns the vector to a random point in the [-.5,-.5]-[+.5,+.5] unit square.
            return vec3(random_double() - 0.5, random_double() - 0.5, 0);
        }

        point3 defocus_disk_sample() const {
            //Returns a random point in the cam defocus disk

            auto p = random_in_unit_disk();
            return center + (p[0] * defocus_disk_u) + (p[1] * defocus_disk_v);
        }
    public:
            double aspect_ratio = 16.0 / 9.0; //ratio width/height
            int image_width = 400; // number of pixels of width
            int samples_per_pixel = 10; //count of random samples for each pixel
            int max_depth = 10; // limits ray bounces in scene

            double vert_fov = 90; //vertical field of view in degrees 
            point3 lookfrom = point3(0, 0, 0); // Point camera is looking from
            point3 lookat = point3(0, 0, -1); // Point camera is looking at
            vec3 vup = vec3(0, 1, 0); // Camera-relative "up" direction

            double defocus_angle = 0; //Variation angle of rays through each pixel
            double focus_dist = 10; // Distance from camera lookfrom to plane of perfect focus

        void render(const hittable& world) {
            auto clock_start = std::chrono::steady_clock::now();

            initialize();

            std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

            std::vector<unsigned char> image_data;
            color_char temp_res;
            auto ctr = 0.0;
            for (int j = 0; j < image_height; j++)
            {
                for (int i = 0; i < image_width; i++) {
                    ctr+=1;
                    std::clog << "\rProgress: " << std::fixed << std::setprecision(2) <<  ctr * 100 /image_height /image_width  << "% " << std::flush;
                    color pixel_color(0,0,0);
                    for (int sample = 0; sample < samples_per_pixel; sample++){
                        ray r = get_ray(i,j);
                        pixel_color += ray_color(r, max_depth, world);
                    }
                    //write_color(std::cout, pixel_samples_scale * pixel_color);

                    temp_res = write_color_ret(pixel_samples_scale * pixel_color);
                    image_data.push_back(temp_res.x);
                    image_data.push_back(temp_res.y);
                    image_data.push_back(temp_res.z);
                }
            }

            auto res = png_image_write(image_width, image_height, 3);
            res.write_image(image_data.data());

            std::clog << "\rDone.\n";
            std::clog << "Elapsed(s)=" << since(clock_start).count() << '\n';
        }
};

#endif