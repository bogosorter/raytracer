#include <math.h>
#include "raytracer.h"

void raytracer_scene(camera_settings_t settings, int tcount, triangle_t *triangles, uint8_t *pixels) {
    double plane_width = tan(settings.fov_angle / 2.0) * 2;
    double plane_height = plane_width * settings.height / settings.width;

    vector_t origin = {0, 0, 4};

    // Point P is the top left point of the screen
    vector_t p = {
        - plane_width / 2 + origin.x,
        plane_height / 2 + origin.y,
        origin.z - 1
    };

    vector_t normals[tcount];
    for (int i = 0; i < tcount; i++) {
        vector_t ab = vector_subtract(triangles[i].b, triangles[i].a);
        vector_t ac = vector_subtract(triangles[i].c, triangles[i].a);
        normals[i] = vector_cross(ab, ac);
        normals[i] = vector_normalize(normals[i]);
    }
    
    for (int i = 0; i < settings.height; i++) {
        for (int j = 0; j < settings.width; j++) {

            vector_t point = {
                p.x + j * plane_width / settings.width,
                p.y - i * plane_height / settings.height,
                p.z
            };

            ray_t ray = {
                origin,
                vector_subtract(point, origin)
            };

            int index = (i * settings.width + j) * 3;
            pixels[index] = 0x00;
            pixels[index + 1] = 0x00;
            pixels[index + 2] = 0x00;

            double closest = -1;

            for (int k = 0; k < tcount; k++) {
                double t = shoot_triangle(ray, triangles[k]);
                if (t >= 0 && (closest == -1 || t < closest)) {
                    closest = t;

                    double luminosity = fabs(vector_dot(normals[k], settings.light));
                    if (luminosity < 0) luminosity = settings.ambient_luminosity;
                    else luminosity = settings.ambient_luminosity + (1 - settings.ambient_luminosity) * luminosity;
                    uint8_t l = luminosity * 255;
                    
                    pixels[index] = l;
                    pixels[index + 1] = l;
                    pixels[index + 2] = l;
                }
            }            
        }
    }
}

