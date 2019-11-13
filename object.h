//
// Created by coldus on 11/4/19.
//

#ifndef NAGYHF_OBJECT_H
#define NAGYHF_OBJECT_H

#include <stdbool.h>

typedef struct Point{double posX; double posY; double posZ} Point;
typedef struct triangle{Point p1; Point p2; Point p3; int r; int g; int b} triangle;
typedef struct din_point_array{Point* points; int size} din_point_array;
typedef struct din_triangle_array{triangle* triangles; int size} triangle_array;

//-----------------------------------------------------------------

void init_point_array(din_point_array* din_array, int size);
bool resize_point_array(din_point_array* din_array, int new_size);
void free_point_array(din_point_array* din_array);

//-----------------------------------------------------------------

void init_triangle_array(triangle_array* din_array, int size);
bool resize_triangle_array(triangle_array* din_array, int new_size);
void free_triangle_array(triangle_array* din_array);

//-----------------------------------------------------------------

typedef struct Model{triangle_array triangleArray} Model;
void free_model(Model *mod);
void load_model_from_file(char* filename, Model *mod);

//-----------------------------------------------------------------

typedef struct Object{Model model; Point location; double angle_from_x_axis; double angle_from_y_axis; double angle_from_z_axis} Object;
void move_Object_to_Point(Object *obj, Point point);
void load_Model_into_Object(Object *obj, Model model);
void free_object(Object *obj);

//----------------------------------------------------------------

double dist_btw_Points(Point p1, Point p2);
Point weightPoint_of_triangle(triangle tri);
void rotate_Point_around_Points_xAxis(Point center, Point *rotatedPoint, double rotX);
void rotate_Point_around_Points_yAxis(Point center, Point *rotatedPoint, double rotY);
void rotate_Point_around_Points_zAxis(Point center, Point *rotatedPoint, double rotZ);
Point rotate_Point_around_Point_wo_change(Point center, Point rotatedPoint, double rotX, double rotY, double rotZ);
void rotate_Point_around_Point(Point center, Point *rotatedPoint, double rotX, double rotY, double rotZ);
void rotate_Object_around_Point(Point center, Object *obj, double rotX, double rotY, double rotZ);

//----------------------------------------------------------------


#endif //NAGYHF_OBJECT_H
