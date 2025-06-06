#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "bmp.h"
#include "geometry.h"
#include "raytracer.h"
#include "object.h"

int main() {
    raytracer_settings_t settings = {
        .height = 450,
        .width = 800,
        .fov_angle = 1.570796327, // pi / 2
        .ambient_luminosity = 0.2,
        .light = vector_normalize((vector_t){-1, 1, 1}),
        .camera = {-3, 0.5, 2.5},
        .forward = vector_normalize((vector_t){1.3, -0.3, -1}),
        .up = vector_normalize((vector_t){0, 1, 0})
    };

    object_t *taxi = object_load("assets/Taxi.obj");
    if (taxi == NULL) {
        printf("Error loading OBJ file\n");
        return -1;
    }

    uint8_t pixels[settings.height * settings.width * 3];
    raytracer_scene(settings, taxi, pixels);

    if (bmp_export("output.bmp", settings.height, settings.width, pixels) != 0) {
        printf("Error exporting BMP file\n");
    }

    object_free(taxi);
}
