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
    raytracer_scene(settings, asset->tcount, asset->triangles, pixels);

    if (bmp_export("output.bmp", HEIGHT, WIDTH, pixels) != 0) {
        printf("Error exporting BMP file\n");
    }

    free(asset->triangles);
    free(asset);
}
