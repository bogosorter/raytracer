#ifndef VECTOR_H
#define VECTOR_H

typedef struct {
    double x;
    double y;
    double z;
} vector_t;


vector_t vector_normalize(vector_t v);
vector_t vector_subtract(vector_t a, vector_t b);
vector_t vector_sum(vector_t a, vector_t b);
vector_t vector_scale(vector_t v, double k);
double vector_dot(vector_t a, vector_t b);
vector_t vector_cross(vector_t a, vector_t b);

// Projects vector a along vector b
vector_t vector_project(vector_t a, vector_t b);

#endif // VECTOR_H
