#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "bmp.h"
#include "geometry.h"
#include "settings.h"
#include "raytracer.h"
#include "obj.h"

int main() {
    asset_t *asset = NULL;
    if (obj_load("assets/Taxi.obj", &asset) != 0) {
        printf("Error loading OBJ file\n");
        return -1;
    }

    triangle_cache_t **caches = malloc(asset->tcount * sizeof(triangle_cache_t *));
    for (int i = 0; i < asset->tcount; i++) {
        caches[i] = triangle_cache_create(asset->triangles[i]);
    }

    vector_t source = {1, 1, 1};
    source = vector_normalize(source);

    camera_settings_t settings = {
        .height = HEIGHT,
        .width = WIDTH,
        .fov_angle = FOV_ANGLE,
        .ambient_luminosity = AMBIENT_LUMINOSITY,
        .light = source,
        .camera = {-3, 0.5, 2.5},
        .forward = vector_normalize((vector_t){1.3, -0.3, -1}),
        .up = vector_normalize((vector_t){0, 1, 0})
    };

    uint8_t pixels[HEIGHT * WIDTH * 3];
    raytracer_scene(settings, asset->tcount, caches, pixels);

    if (bmp_export("output.bmp", HEIGHT, WIDTH, pixels) != 0) {
        printf("Error exporting BMP file\n");
    }

    free(asset->triangles);
    free(asset);
}
