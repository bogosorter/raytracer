#include "geometry.h"
#include "settings.h"

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
    if (a < 0) return -1;

    // Second baricentric coordinate
    v = vector_subtract(bc, vector_project(bc, ac));
    vector_t bh = vector_subtract(hit, triangle.b);
    double b = 1 - vector_dot(v, bh) / vector_dot(v, bc);
    if (b < 0) return -1;

    double c = 1 - a - b;
    if (c < 0) return -1;

    return t;
}
