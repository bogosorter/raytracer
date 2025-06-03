#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include "bmp.h"
#include "vector.h"

#define FOV_ANGLE M_PI / 3
#define HEIGHT    1080
#define WIDTH     1920
#define DEPTH     10
#define AMBIENT_LUMINOSITY 0.05
#define INTENSITY 0.8
#define TOLERANCE 1e-6


typedef struct {
    vector_t center;
    double radius;
} sphere_t;

typedef struct {
    vector_t point;
    vector_t normal;
} plane_t;

typedef struct {
    vector_t a;
    vector_t b;
    vector_t c;
} triangle_t;

typedef struct {
    vector_t origin;
    vector_t direction;
} ray_t;

double shoot_sphere(ray_t ray, sphere_t sphere) {
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

double shoot_plane(ray_t ray, plane_t plane) {
    vector_t w = vector_subtract(plane.point, ray.origin);
    double a = vector_dot(w, plane.normal);
    double b = vector_dot(ray.direction, plane.normal);

    // Ray is parallel to the plane
    if (fabs(b) < TOLERANCE) return -1;
    return a / b;
}

double shoot_triangle(ray_t ray, triangle_t triangle) {
    vector_t ab = vector_subtract(triangle.b, triangle.a);
    vector_t ac = vector_subtract(triangle.c, triangle.a);
    vector_t bc = vector_subtract(triangle.c, triangle.b);
    
    // Define the triangle's plane
    vector_t normal = vector_cross(ab, ac);
    plane_t plane = { triangle.a, normal };
    double t = shoot_plane(ray, plane);
    if (t < 0) return -1;
    vector_t hit = vector_sum(ray.origin, vector_multiply(ray.direction, t));

    // Check if the hit point is inside the triangle using barycentric coordinates
    
    // First baricentric coordinate
    vector_t v = vector_subtract(ac, vector_project(ac, bc));
    vector_t ah = vector_subtract(hit, triangle.a);
    double a = 1 - vector_dot(v, ah) / vector_dot(v, ac);
    if (a < 0 || a > 1) return -1;

    // Second baricentric coordinate
    v = vector_subtract(bc, vector_project(bc, ac));
    vector_t bh = vector_subtract(hit, triangle.b);
    double b = 1 - vector_dot(v, bh) / vector_dot(v, bc);
    if (b < 0 || b > 1) return -1;

    double c = 1 - a - b;
    if (c < 0) return -1;

    return t;
}

int main() {
    double plane_width = tan(FOV_ANGLE / 2.0) * 2;
    double plane_height = plane_width * HEIGHT / WIDTH;

    vector_t up_left = {
        - plane_width / 2,
        plane_height / 2,
        -1
    };

    triangle_t triangle = {
        {-5, 3, -20},
        {-4, -1, -9},
        {4, -3, -10}
    };

    vector_t source = {1, 1, 1};
    source = vector_normalize(source);

    vector_t origin = {0, 0, 0};

    vector_t ab = vector_subtract(triangle.b, triangle.a);
    vector_t ac = vector_subtract(triangle.c, triangle.a);
    vector_t normal = vector_cross(ab, ac);
    normal = vector_normalize(normal);

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

            double t = shoot_triangle(ray, triangle);
            if (t < 0) {
                int index = (i * WIDTH + j) * 3;
                pixels[index] = t >= 0? 0xff : 0x00;
                pixels[index + 1] = t >= 0? 0xff : 0x00;
                pixels[index + 2] = t >= 0? 0xff : 0x00;
                continue;
            }

            double luminosity = fabs(vector_dot(normal, source));
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
