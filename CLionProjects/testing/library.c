#include "library.h"

#include <stdio.h>

typedef struct din_point_array{Point* points; int size} din_point_array;
typedef struct din_triangle_array{triangle* triangles; int size} triangle_array;

void init_point_array(din_point_array* din_array, int size) {
    din_array->points = (Point*) calloc(size, sizeof(Point));
    din_array->size = size;
}

void resize_point_array(din_point_array* din_array, int new_size) {
    din_array->points = (Point*) realloc(din_array->points, new_size);
    din_array->size = new_size;
}

void free_point_array(din_point_array* din_array) {
    free(din_array->points);
}

void hello(void) {
    printf("Hello, World!\n");
}