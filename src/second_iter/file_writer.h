#pragma once

// Disable strict warnings for this header from the Microsoft Visual C++ compiler.
#ifdef _MSC_VER
    #pragma warning (push, 0)
#endif

#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "../external/stb_image_write.h"

//class needed just to load image into suitable format
class png_image_write {
    private:
        const char* filename = "res_image.png";
        int width;
        int height;
        int channels = 3;

    public:
        png_image_write(int width, int height, int channels=3)
        : width(width), height(height), channels(channels) {}


        //basically writes image into file "res_image.png" located in .exe folder
        int write_image(unsigned char * data) {
            return stbi_write_png(filename, width, height, channels, data, width * channels);
        }

};