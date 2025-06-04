#ifndef OBJ_H
#define OBJ_H

#include "geometry.h"

typedef struct {
    int tcount;
    triangle_t *triangles;
} asset_t;

int obj_load(char *filename, asset_t **asset);

#endif // OBJ_H
