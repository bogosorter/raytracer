#ifndef GEOMETRY_H
#define GEOMETRY_H

#include "vector.h"

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

typedef struct triangle_cache triangle_cache_t;

typedef struct {
    vector_t origin;
    vector_t direction;
} ray_t;

double shoot_sphere(ray_t ray, sphere_t sphere);
double shoot_plane(ray_t ray, plane_t plane);
triangle_cache_t *triangle_cache_create(const triangle_t triangle);
vector_t triangle_cache_get_normal(const triangle_cache_t *cache);
double shoot_triangle(ray_t ray, triangle_cache_t *cache);

#endif // VECTOR_H
