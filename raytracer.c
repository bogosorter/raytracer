#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include "bmp.h"

#define FOV_ANGLE M_PI / 3
#define HEIGHT    1080
#define WIDTH     1920
#define DEPTH     10
#define AMBIENT_LUMINOSITY 0.05
#define INTENSITY 0.8

typedef struct {
    double x;
    double y;
    double z;
} vector_t;

typedef struct {
    vector_t center;
    double radius;
} sphere_t;

typedef struct {
    vector_t origin;
    vector_t direction;
} ray_t;

double vector_dot(vector_t a, vector_t b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

vector_t vector_subtract(vector_t a, vector_t b) {
    vector_t result;
    result.x = a.x - b.x;
    result.y = a.y - b.y;
    result.z = a.z - b.z;
    return result;
}

vector_t vector_sum(vector_t a, vector_t b) {
    vector_t result;
    result.x = a.x + b.x;
    result.y = a.y + b.y;
    result.z = a.z + b.z;
    return result;
}

vector_t vector_multiply(vector_t v, double k) {
    vector_t result;
    result.x = v.x * k;
    result.y = v.y * k;
    result.z = v.z * k;
    return result;
}

vector_t vector_normalize(vector_t v) {
    double magnitude = sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
    vector_t result;
    result.x = v.x / magnitude;
    result.y = v.y / magnitude;
    result.z = v.z / magnitude;
    return result;
}

double shoot(ray_t ray, sphere_t sphere) {
    vector_t oc = vector_subtract(ray.origin, sphere.center);
    double a = vector_dot(ray.direction, ray.direction);
    double b = 2 * vector_dot(oc, ray.direction);
    double c = vector_dot(oc, oc) - sphere.radius * sphere.radius;

    double discriminant = b * b - 4 * a * c;
    if (discriminant < 0) return -1;
    
    double t1 = (-b - sqrt(discriminant)) / (2 * a);
    if (t1 > 0) return t1;

    double t2 = (-b + sqrt(discriminant)) / (2 * a);
    if (t2 > 0) return t2;

    return -1;
}

int main() {
    double plane_width = tan(FOV_ANGLE / 2.0) * 2;
    double plane_height = plane_width * HEIGHT / WIDTH;

    vector_t up_left = {
        - plane_width / 2,
        plane_height / 2,
        -1
    };

    sphere_t sphere = {
        {0, 0, -4},
        1
    };

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

            double t = shoot(ray, sphere);
            if (t < 0) {
                int index = (i * WIDTH + j) * 3;
                pixels[index] = t >= 0? 0xff : 0x00;
                pixels[index + 1] = t >= 0? 0xff : 0x00;
                pixels[index + 2] = t >= 0? 0xff : 0x00;
                continue;
            }

            vector_t hit = vector_multiply(point, t);
            vector_t normal = vector_subtract(hit, sphere.center);
            normal = vector_normalize(normal);
            double luminosity = vector_dot(normal, source);
            if (luminosity < 0) luminosity = AMBIENT_LUMINOSITY;
            else luminosity = AMBIENT_LUMINOSITY + (1 - AMBIENT_LUMINOSITY) * INTENSITY * luminosity;
            uint8_t l = luminosity * 255;
            int index = (i * WIDTH + j) * 3;
            pixels[index] = l;
            pixels[index + 1] = l;
            pixels[index + 2] = l;
        }
    }

    if (bmp_export("output.bmp", HEIGHT, WIDTH, pixels) != 0) {
        printf("Error exporting BMP file\n");
    }
}
