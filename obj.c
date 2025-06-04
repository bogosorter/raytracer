#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include "obj.h"

int count_numbers(const char *str);
void read_vertices(char *line, int *vertices);

int obj_load(char *filename, asset_t **asset) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Couldn't open input file %s", filename);
        return -1;
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

    *asset = malloc(sizeof(asset_t));
    (*asset)->tcount = tcount;
    (*asset)->triangles = malloc(tcount * sizeof(triangle_t));
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
                (*asset)->triangles[current_triangle].a = vertices[indexes[0] - 1];
                (*asset)->triangles[current_triangle].b = vertices[indexes[i - 1] - 1];
                (*asset)->triangles[current_triangle].c = vertices[indexes[i] - 1];
                current_triangle++;
            }
        }

        // Ignore other types of lines
    }

    fclose(file);
    free(vertices);
    return 0;
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
