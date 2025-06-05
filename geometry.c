#include <stdlib.h>
#include <stdbool.h>
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

bool shoot_orthogonal_box(ray_t ray, vector_t start, vector_t end) {
    double tmin = (start.x - ray.origin.x) / ray.direction.x;
    double tmax = (end.x - ray.origin.x) / ray.direction.x;
    if (tmin > tmax) {
        double temp = tmin;
        tmin = tmax;
        tmax = temp;
    }
    if (tmax < 0) return false;

    double tymin = (start.y - ray.origin.y) / ray.direction.y;
    double tymax = (end.y - ray.origin.y) / ray.direction.y;
    if (tymin > tymax) {
        double temp = tymin;
        tymin = tymax;
        tymax = temp;
    }
    if (tymin > tmin) tmin = tymin;
    if (tymax < tmax) tmax = tymax;
    if (tmin > tmax || tmax < 0) return false;

    double tzmin = (start.z - ray.origin.z) / ray.direction.z;
    double tzmax = (end.z - ray.origin.z) / ray.direction.z;
    if (tzmin > tzmax) {
        double temp = tzmin;
        tzmin = tzmax;
        tzmax = temp;
    }
    if (tzmin > tmin) tmin = tzmin;
    if (tzmax < tmax) tmax = tzmax;
    if (tmin > tmax || tmax < 0) return false;
    return true;
}

struct triangle_cache {
    triangle_t triangle;
    vector_t ab; // Edge vector AB
    vector_t ac; // Edge vector AC
    vector_t bc; // Edge vector BC
    vector_t barycentric_a; // Barycentric coordinate vector for A
    vector_t barycentric_b; // Barycentric coordinate vector for B
    vector_t normal; // Normal vector of the triangle
    plane_t plane; // Plane of the triangle

    vector_t hitbox_start; // Start point of the hitbox
    vector_t hitbox_end; // End point of the hitbox
};

triangle_cache_t *triangle_cache_create(const triangle_t triangle) {
    triangle_cache_t *cache = malloc(sizeof(triangle_cache_t));

    cache->triangle = triangle;
    cache->ab = vector_subtract(triangle.b, triangle.a);
    cache->ac = vector_subtract(triangle.c, triangle.a);
    cache->bc = vector_subtract(triangle.c, triangle.b);

    cache->barycentric_a = vector_subtract(cache->ac, vector_project(cache->ac, cache->bc));
    cache->barycentric_b = vector_subtract(cache->bc, vector_project(cache->bc, cache->ac));
    
    cache->normal = vector_normalize(vector_cross(cache->ab, cache->ac));
    
    cache->plane.point = triangle.a;
    cache->plane.normal = cache->normal;

    cache->hitbox_start.x = fmin(fmin(triangle.a.x, triangle.b.x), triangle.c.x);
    cache->hitbox_start.y = fmin(fmin(triangle.a.y, triangle.b.y), triangle.c.y);
    cache->hitbox_start.z = fmin(fmin(triangle.a.z, triangle.b.z), triangle.c.z);

    cache->hitbox_end.x = fmax(fmax(triangle.a.x, triangle.b.x), triangle.c.x);
    cache->hitbox_end.y = fmax(fmax(triangle.a.y, triangle.b.y), triangle.c.y);
    cache->hitbox_end.z = fmax(fmax(triangle.a.z, triangle.b.z), triangle.c.z);

    return cache;
}

vector_t triangle_cache_get_normal(const triangle_cache_t *cache) {
    return cache->normal;
}

double shoot_triangle(ray_t ray, triangle_cache_t *cache) {
    double t = shoot_plane(ray, cache->plane);
    if (t < 0) return -1;
    vector_t hit = vector_sum(ray.origin, vector_multiply(ray.direction, t));

    // Check if the hit point is inside the triangle using barycentric coordinates
    
    // First baricentric coordinate
    vector_t ah = vector_subtract(hit, cache->triangle.a);
    double a = 1 - vector_dot(cache->barycentric_a, ah) / vector_dot(cache->barycentric_a, cache->ac);
    if (a < 0) return -1;

    // Second baricentric coordinate
    vector_t bh = vector_subtract(hit, cache->triangle.b);
    double b = 1 - vector_dot(cache->barycentric_b, bh) / vector_dot(cache->barycentric_b, cache->bc);
    if (b < 0) return -1;

    double c = 1 - a - b;
    if (c < 0) return -1;

    return t;
}
