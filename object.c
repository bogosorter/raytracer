#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include "object.h"

struct object {
    int tcount;
    triangle_data_t **triangles;
};

int count_numbers(const char *str);
void read_vertices(char *line, int *vertices);

object_t *object_load(char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Couldn't open input file %s", filename);
        return NULL;
    }

    int vcount = 0;
    int tcount = 0;

    char line[1024];

    // First pass to determine the number of vertices and triangles
    while (fgets(line, sizeof(line), file)) {
        if (line[0] == 'v' && line[1] == ' ') vcount++;
        else if (line[0] == 'f' && line[1] == ' ') {
            // Count number of vertices in face
            tcount += count_numbers(line) - 2;
        }

        // Ignore other types of lines
    }

    // Second pass to read the data
    rewind(file);

    object_t *object = malloc(sizeof(object_t));
    object->tcount = tcount;
    object->triangles = malloc(tcount * sizeof(triangle_data_t*));
    vector_t *vertices = malloc(vcount * sizeof(vector_t));

    int current_triangle = 0;
    int current_vertex = 0;
    while (fgets(line, sizeof(line), file)) {
        if (line[0] == 'v' && line[1] == ' ') {
            double x, y, z;
            sscanf(line, "v %lf %lf %lf", &x, &y, &z);

            vertices[current_vertex].x = x;
            vertices[current_vertex].y = y;
            vertices[current_vertex].z = z;
            current_vertex++;
        }
        else if (line[0] == 'f' && line[1] == ' ') {
            int vertex_count = count_numbers(line);
            int indexes[vertex_count];
            read_vertices(line, indexes);

            for (int i = 2; i < vertex_count; i++) {
                triangle_t triangle;
                triangle.a = vertices[indexes[0] - 1];
                triangle.b = vertices[indexes[i - 1] - 1];
                triangle.c = vertices[indexes[i] - 1];
                object->triangles[current_triangle] = geometry_get_data(&triangle);
                current_triangle++;
            }
        }

        // Ignore other types of lines
    }

    fclose(file);
    free(vertices);
    return object;
}

void object_free(object_t *object) {
    for (int i = 0; i < object->tcount; i++) {
        free(object->triangles[i]);
    }
    free(object->triangles);
}

int count_numbers(const char *str) {
    int count = 0;
    const char *p = str;

    while (*p) {
        if (*p == 'f') { // skip the initial 'f'
            while (*p && !isspace(*p)) p++;
            continue;
        }

        if (isdigit(*p)) {
            count++;
            while (*p && *p != ' ') p++; // skip to next token
        } else {
            p++;
        }
    }

    return count;
}

void read_vertices(char *line, int *vertices) {
    int i = 0;
    char *token = strtok(line, " ");
    token = strtok(NULL, " "); // Skip "f"

    while (token != NULL) {
        char *slash = strchr(token, '/');
        if (slash) *slash = '\0'; // truncate at first slash
        vertices[i++] = atoi(token);
        token = strtok(NULL, " ");
    }
}

int object_triangle_count(object_t *object) {
    return object->tcount;
}
triangle_data_t **object_get_triangles(object_t *object) {
    return object->triangles;
}

