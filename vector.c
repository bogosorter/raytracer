#include <math.h>
#include "vector.h"

vector_t vector_normalize(vector_t v) {
    double magnitude = sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
    vector_t result;
    result.x = v.x / magnitude;
    result.y = v.y / magnitude;
    result.z = v.z / magnitude;
    return result;
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

double vector_dot(vector_t a, vector_t b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}
