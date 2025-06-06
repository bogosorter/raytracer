#ifndef RAYTRACER_H
#define RAYTRACER_H

#include <stdint.h>
#include "object.h"
#include "geometry.h"

typedef struct {
    int height;
    int width;
    double fov_angle;
    double ambient_luminosity;
    vector_t light;
    vector_t camera;
    vector_t forward;
    vector_t up;
} raytracer_settings_t;

void raytracer_scene(raytracer_settings_t settings, object_t *object, uint8_t *pixels);

#endif // RAYTRACER_H
