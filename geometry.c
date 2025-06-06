#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "geometry.h"

#define TOLERANCE 1e-6

typedef struct {
    vector_t point;
    vector_t normal;
} plane_t;

typedef struct {
    vector_t start;
    vector_t end;
} hitbox_t;

struct triangle_data {
    triangle_t triangle;
    vector_t ab;
    vector_t ac;
    vector_t bc;
    vector_t barycentric_a;
    vector_t barycentric_b;
    vector_t normal;
    hitbox_t hitbox;
    plane_t plane;
};

double geometry_shoot_plane(ray_t *ray, plane_t *plane);
bool geometry_shoot_hitbox(ray_t *ray, hitbox_t *hitbox);

triangle_data_t *geometry_get_data(triangle_t *triangle) {
    triangle_data_t *data = malloc(sizeof(triangle_data_t));

    data->triangle = *triangle;
    data->ab = vector_subtract(triangle->b, triangle->a);
    data->ac = vector_subtract(triangle->c, triangle->a);
    data->bc = vector_subtract(triangle->c, triangle->b);

    data->barycentric_a = vector_subtract(data->ac, vector_project(data->ac, data->bc));
    data->barycentric_b = vector_subtract(data->bc, vector_project(data->bc, data->ac));
    
    data->normal = vector_normalize(vector_cross(data->ab, data->ac));
    
    data->plane.point = triangle->a;
    data->plane.normal = data->normal;

    data->hitbox.start.x = fmin(fmin(triangle->a.x, triangle->b.x), triangle->c.x);
    data->hitbox.start.y = fmin(fmin(triangle->a.y, triangle->b.y), triangle->c.y);
    data->hitbox.start.z = fmin(fmin(triangle->a.z, triangle->b.z), triangle->c.z);

    data->hitbox.end.x = fmax(fmax(triangle->a.x, triangle->b.x), triangle->c.x);
    data->hitbox.end.y = fmax(fmax(triangle->a.y, triangle->b.y), triangle->c.y);
    data->hitbox.end.z = fmax(fmax(triangle->a.z, triangle->b.z), triangle->c.z);

    return data;
}

vector_t geometry_get_normal(triangle_data_t *data) {
    return data->normal;
}

double geometry_shoot_triangle(ray_t *ray, triangle_data_t *cache) {
    if (!geometry_shoot_hitbox(ray, &cache->hitbox)) return -1;
    
    double t = geometry_shoot_plane(ray, &cache->plane);
    if (t < 0) return -1;
    vector_t hit = vector_sum(ray->origin, vector_scale(ray->direction, t));

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

double geometry_shoot_plane(ray_t *ray, plane_t *plane) {
    vector_t w = vector_subtract(plane->point, ray->origin);
    double a = vector_dot(w, plane->normal);
    double b = vector_dot(ray->direction, plane->normal);

    // Ray is parallel to the plane
    if (fabs(b) < TOLERANCE) return -1;
    return a / b;
}

bool geometry_shoot_hitbox(ray_t *ray, hitbox_t *hitbox) {
    double tmin = (hitbox->start.x - ray->origin.x) / ray->direction.x;
    double tmax = (hitbox->end.x - ray->origin.x) / ray->direction.x;
    if (tmin > tmax) {
        double temp = tmin;
        tmin = tmax;
        tmax = temp;
    }
    if (tmax < 0) return false;

    double tymin = (hitbox->start.y - ray->origin.y) / ray->direction.y;
    double tymax = (hitbox->end.y - ray->origin.y) / ray->direction.y;
    if (tymin > tymax) {
        double temp = tymin;
        tymin = tymax;
        tymax = temp;
    }
    if (tymin > tmin) tmin = tymin;
    if (tymax < tmax) tmax = tymax;
    if (tmin > tmax || tmax < 0) return false;

    double tzmin = (hitbox->start.z - ray->origin.z) / ray->direction.z;
    double tzmax = (hitbox->end.z - ray->origin.z) / ray->direction.z;
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
