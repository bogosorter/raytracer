#ifndef GEOMETRY_H
#define GEOMETRY_H

#include "vector.h"

typedef struct {
    vector_t a;
    vector_t b;
    vector_t c;
} triangle_t;

typedef struct {
    vector_t origin;
    vector_t direction;
} ray_t;

typedef struct triangle_data triangle_data_t;

triangle_data_t *geometry_get_data(triangle_t *triangle);
vector_t geometry_get_normal(triangle_data_t *data);
double geometry_shoot_triangle(ray_t *ray, triangle_data_t *cache);

#endif // VECTOR_H
