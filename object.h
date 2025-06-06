#ifndef OBJECT_H
#define OBJECT_H

#include "geometry.h"

typedef struct object object_t;

object_t *object_load(char *filename);
void object_free(object_t *object);

int object_triangle_count(object_t *object);
triangle_data_t **object_get_triangles(object_t *object);

#endif // OBJECT_H
