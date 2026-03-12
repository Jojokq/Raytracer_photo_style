#ifndef COLOR_H
#define COLOR_H

#include "interval.h"
#include "vec3.h"

using color = vec3;

struct color_char{
    unsigned char x;
    unsigned char y;
    unsigned char z;
};

inline double linear_to_gamma(double linear_component) {
    if (linear_component > 0)
        return std::sqrt(linear_component);

    return 0;
}

void write_color(std::ostream& out, const color& pixel_color) {
    auto r = pixel_color.x();
    auto g = pixel_color.y();
    auto b = pixel_color.z();

    //apply a gamma correction
    r = linear_to_gamma(r);
    g = linear_to_gamma(g);
    b = linear_to_gamma(b);

    //[0,1] -> [0, 255]
    static const interval intensity(0.000, 0.999);
    int rbyte = int(256 * intensity.clamp(r));
    int gbyte = int(256 * intensity.clamp(g));
    int bbyte = int(256 * intensity.clamp(b));

    out << rbyte << ' ' << gbyte << ' ' << bbyte << '\n';
}

color_char write_color_ret(const color& pixel_color) {
    auto r = pixel_color.x();
    auto g = pixel_color.y();
    auto b = pixel_color.z();

    //apply a gamma correction
    r = linear_to_gamma(r);
    g = linear_to_gamma(g);
    b = linear_to_gamma(b);

    //[0,1] -> [0, 255]
    static const interval intensity(0.000, 0.999);
    int rbyte = int(256 * intensity.clamp(r));
    int gbyte = int(256 * intensity.clamp(g));
    int bbyte = int(256 * intensity.clamp(b));

    return color_char{(unsigned char)rbyte, (unsigned char)gbyte, (unsigned char)bbyte};
}

#endif