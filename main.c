#include <stdio.h>
#include <stdint.h>
#include "bmp.h"
#include "geometry.h"
#include "settings.h"
#include "raytracer.h"

int main() {
    triangle_t triangles[] = {
        {
            {0, 3, -10},
            {0, 0, -10},
            {-2, -2, -8}
        },
        {
            {0, 3, -10},
            {0, 0, -10},
            {2, -2, -10}
        },
        {
            {-2, -2, -8},
            {2, -2, -10},
            {0, 0, -10}
        }
    };

    vector_t source = {1, 1, 1};
    source = vector_normalize(source);

    camera_settings_t settings = {
        .height = HEIGHT,
        .width = WIDTH,
        .fov_angle = FOV_ANGLE,
        .ambient_luminosity = AMBIENT_LUMINOSITY,
        .light = source
    };

    uint8_t pixels[HEIGHT * WIDTH * 3];
    raytracer_scene(settings, 3, triangles, pixels);

    if (bmp_export("output.bmp", HEIGHT, WIDTH, pixels) != 0) {
        printf("Error exporting BMP file\n");
    }
}
