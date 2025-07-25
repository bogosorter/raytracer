#include <math.h>
#include <stdio.h>
#include "raytracer.h"

void raytracer_scene(raytracer_settings_t settings, object_t *object, uint8_t *pixels) {
    double plane_width = tan(settings.fov_angle / 2.0) * 2;
    double plane_height = plane_width * settings.height / settings.width;

    vector_t right = vector_cross(settings.forward, settings.up);
    right = vector_normalize(right);

    // Point P is the top left point of the screen
    vector_t p = vector_sum(settings.camera, vector_scale(settings.forward, 1));
    p = vector_sum(p, vector_scale(settings.up, plane_height / 2.0));
    p = vector_sum(p, vector_scale(right, -plane_width / 2.0));
    
    for (int i = 0; i < settings.height; i++) {
        for (int j = 0; j < settings.width; j++) {

            if (j == 0 && i % 20 == 0) {
                printf("Rendering row %d of %d\n", i, settings.height);
                printf("Progress: %.2f%%\n", (double)i / settings.height * 100);
            }

            vector_t point = vector_sum(p, vector_scale(settings.up, -i * plane_height / settings.height));
            point = vector_sum(point, vector_scale(right, j * plane_width / settings.width));

            ray_t ray = {
                settings.camera,
                vector_subtract(point, settings.camera)
            };

            int index = (i * settings.width + j) * 3;
            pixels[index] = 0x00;
            pixels[index + 1] = 0x00;
            pixels[index + 2] = 0x00;

            double closest = -1;

            int tcount = object_triangle_count(object);
            triangle_data_t **triangles = object_get_triangles(object);

            for (int k = 0; k < tcount; k++) {
                double t = geometry_shoot_triangle(&ray, triangles[k]);
                if (t >= 0 && (closest == -1 || t < closest)) {
                    closest = t;

                    double luminosity = fabs(vector_dot(geometry_get_normal(triangles[k]), settings.light));
                    if (luminosity < 0) luminosity = 0;
                    luminosity = settings.ambient_luminosity + luminosity * (1 - settings.ambient_luminosity);
                    uint8_t l = luminosity * 255;
                    
                    pixels[index] = l;
                    pixels[index + 1] = l;
                    pixels[index + 2] = l;
                }
            }            
        }
    }
}

