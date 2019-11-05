//
// Created by coldus on 11/4/19.
//

#ifndef object
#define object
#include "additional.h"
#include <stdbool.h>

typedef struct triangle{Point p1; Point p2; Point p3} triangle;
typedef struct din_point_array{Point* points; int size} din_point_array;
typedef struct din_triangle_array{triangle* triangles; int size} triangle_array;

void init_point_array(din_point_array* din_array, int size) {
    din_array->points = (Point*) calloc(size, sizeof(Point));
    din_array->size = size;
}

bool resize_point_array(din_point_array *din_array, int new_size) {
    Point *new_point = (Point*) malloc(new_size * sizeof(Point));
    if (new_point == NULL) {
        return false;
    }
    int min = new_size < din_array->size ? new_size : din_array->size;
    for (int i = 0; i < min; i++) {
        new_point[i] = din_array->points[i];
    }
    free(din_array->points);
    din_array->points = new_point;
    din_array->size = new_size;
    return true;
}

void free_point_array(din_point_array* din_array) {
    free(din_array->points);
}

//-----------------------------------------------------------------

void init_triangle_array(triangle_array* din_array, int size) {
    din_array->triangles = (triangle*) calloc(size, sizeof(triangle));
    din_array->size = size;
}

bool resize_triangle_array(triangle_array *din_array, int new_size) {
    triangle *new_tri = (Point*) malloc(new_size * sizeof(triangle));
    if (new_tri == NULL) {
        return false;
    }
    int min = new_size < din_array->size ? new_size : din_array->size;
    for (int i = 0; i < min; i++) {
        new_tri[i] = din_array->triangles[i];
    }
    free(din_array->triangles);
    din_array->triangles = new_tri;
    din_array->size = new_size;
    return true;
}

void free_triangle_array(triangle_array* din_array) {
    free(din_array->triangles);
}

//----------------------------------------------------------------

typedef struct Object{triangle_array triangleArray} Object;

void free_object(Object* obj) {
    free(obj);
}

void load_object_from_file(char *filename, Object *obj) {
    //Haromszogek
    triangle_array tri_array;
    init_triangle_array(&tri_array, 0);

    //Referencia pont
    //Point p = obj->position;

    //Pontok
    din_point_array pArray;
    init_point_array(&pArray, 0);

    FILE *fp;

    fp = fopen(filename, "r");

    if (fp == NULL) {
        perror("Fajl megnyitasa sikertelen");
    }

    char str[10], str2[10], str3[10], str4[10];

    while (fscanf(fp, "%s %s %s %s", str, str2, str3, str4) != EOF) {
        if (str[0] == 'p') {
            int size = ++pArray.size;
            resize_point_array(&pArray, size);
            pArray.points[size-1].posX = (int) strtol(str2, NULL, 10);
            pArray.points[size-1].posY = (int) strtol(str3, NULL, 10);
            pArray.points[size-1].posZ = (int) strtol(str4, NULL, 10);
            //printf("pont\n");
        } else if (strstr(str, "tri") != NULL) {
            resize_triangle_array(&tri_array, ++tri_array.size);
            int a,b,c;

            a = (int) strtol(str2+1, NULL, 10);
            b = (int) strtol(str3+1, NULL, 10);
            c = (int) strtol(str4+1, NULL, 10);

            tri_array.triangles[tri_array.size-1].p1 = pArray.points[a-1];
            tri_array.triangles[tri_array.size-1].p2 = pArray.points[b-1];
            tri_array.triangles[tri_array.size-1].p3 = pArray.points[c-1];
        }
    }

    obj->triangleArray.triangles = tri_array.triangles;

    fclose(fp);
    free_triangle_array(&tri_array);
    free_point_array(&pArray);
}

#endif