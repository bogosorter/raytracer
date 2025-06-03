#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include "bmp.h"
#include "geometry.h"
#include "settings.h"

int main() {
    double plane_width = tan(FOV_ANGLE / 2.0) * 2;
    double plane_height = plane_width * HEIGHT / WIDTH;

    vector_t up_left = {
        - plane_width / 2,
        plane_height / 2,
        -1
    };

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
    vector_t normals[3];
    for (int i = 0; i < 3; i++) {
        vector_t ab = vector_subtract(triangles[i].b, triangles[i].a);
        vector_t ac = vector_subtract(triangles[i].c, triangles[i].a);
        normals[i] = vector_cross(ab, ac);
        normals[i] = vector_normalize(normals[i]);
    }

    vector_t source = {1, 1, 1};
    source = vector_normalize(source);

    vector_t origin = {0, 0, 0};

    uint8_t pixels[HEIGHT * WIDTH * 3];
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {

            vector_t point = {
                up_left.x + j * plane_width / WIDTH,
                up_left.y - i * plane_height / HEIGHT,
                -1
            };

            ray_t ray = {
                origin,
                point
            };

            int index = (i * WIDTH + j) * 3;
            pixels[index] = 0x00;
            pixels[index + 1] = 0x00;
            pixels[index + 2] = 0x00;

            double closest = -1;

            for (int k = 0; k < 3; k++) {
                double t = shoot_triangle(ray, triangles[k]);
                if (t >= 0 && (closest == -1 || t < closest)) {
                    closest = t;

                    double luminosity = fabs(vector_dot(normals[k], source));
                    if (luminosity < 0) luminosity = AMBIENT_LUMINOSITY;
                    else luminosity = AMBIENT_LUMINOSITY + (1 - AMBIENT_LUMINOSITY) * INTENSITY * luminosity;
                    uint8_t l = luminosity * 255;
                    int index = (i * WIDTH + j) * 3;
                    pixels[index] = l;
                    pixels[index + 1] = l;
                    pixels[index + 2] = l;
                }
            }            
        }
    }

    if (bmp_export("output.bmp", HEIGHT, WIDTH, pixels) != 0) {
        printf("Error exporting BMP file\n");
    }
}
