//
// Created by coldus on 11/4/19.
//

#ifndef NAGYHF_OBJECT_H
#define NAGYHF_OBJECT_H
#include "additional.h"

typedef struct triangle{Point p1; Point p2; Point p3} triangle;
typedef struct din_point_array{Point* points; int size} din_point_array;
typedef struct din_triangle_array{triangle* triangles; int size} triangle_array;

//----------------------------------------------------------------

void init_point_array(din_point_array* din_array, int size);
bool resize_point_array(din_point_array* din_array, int new_size);
void free_point_array(din_point_array* din_array);

//-----------------------------------------------------------------

void init_triangle_array(triangle_array* din_array, int size);
bool resize_triangle_array(triangle_array* din_array, int new_size);
void free_triangle_array(triangle_array* din_array);

//-----------------------------------------------------------------

typedef struct Object{triangle_array triangleArray} Object;
//void free_object(Object* obj);
void load_object_from_file(char* filename, Object *obj);

#endif //NAGYHF_OBJECT_H
