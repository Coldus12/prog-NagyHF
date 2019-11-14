//
// Created by coldus on 11/4/19.
//

#ifndef object
#define object

#include <stdbool.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

//Debugmalloc:
#include "debugmalloc-impl.h"
#include "debugmalloc.h"

typedef struct Point{double posX; double posY; double posZ} Point;
typedef struct triangle{Point p1; Point p2; Point p3; int r; int g; int b} triangle;
typedef struct din_point_array{Point* points; int size} din_point_array;
typedef struct din_triangle_array{triangle* triangles; int size} triangle_array;
typedef struct din_bool_array{bool* boolean; int size} din_bool_array;
typedef struct Model{triangle_array triangleArray} Model;
typedef struct Object{Model model; Point location; double angle_from_x_axis; double angle_from_y_axis; double angle_from_z_axis} Object;


//                                  din_bool_array
//----------------------------------------------------------------------------------------------------------------------

void init_bool_array(din_bool_array* din_array, int size) {
    din_array->boolean = (bool*) malloc(size * sizeof(Point));
    din_array->size = size;
}

bool resize_bool_array(din_bool_array *din_array, int new_size) {
    bool *new_bool = (bool*) malloc(new_size * sizeof(bool));
    if (new_bool == NULL) {
        return false;
    }
    //int min = new_size < din_array->size ? new_size : din_array->size;
    for (int i = 0; i < din_array->size-1; i++) {
        new_bool[i] = din_array->boolean[i];
    }
    free(din_array->boolean);
    din_array->boolean = new_bool;
    din_array->size = new_size;
    return true;
}

void free_bool_array(din_bool_array* din_array) {
    free(din_array->boolean);
}

//                                      din_point_array
//----------------------------------------------------------------------------------------------------------------------

void init_point_array(din_point_array* din_array, int size) {
    din_array->points = (Point*) malloc(size * sizeof(Point));
    din_array->size = size;
}

bool resize_point_array(din_point_array *din_array, int new_size) {
    Point *new_point = (Point*) malloc(new_size * sizeof(Point));
    if (new_point == NULL) {
        return false;
    }


    //int min = new_size < din_array->size ? new_size : din_array->size;


    for (int i = 0; i < din_array->size-1; i++) {
        new_point[i] = din_array->points[i]; //<--
    }

    free(din_array->points);
    din_array->points = new_point;
    din_array->size = new_size;
    return true;
}

void free_point_array(din_point_array* din_array) {
    free(din_array->points);
}

//                                         din_triangle_array
//----------------------------------------------------------------------------------------------------------------------

void init_triangle_array(triangle_array* din_array, int size) {
    din_array->triangles = (triangle*) malloc(size * sizeof(triangle));
    din_array->size = size;
}

bool resize_triangle_array(triangle_array *din_array, int new_size) {
    triangle *new_tri = (triangle*) malloc(new_size * sizeof(triangle));
    if (new_tri == NULL) {
        return false;
    }
    //int min = new_size < din_array->size ? new_size : din_array->size;

    for (int i = 0; i < din_array->size-1; i++) {
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

//                                     Objects, Models, and their functions
//----------------------------------------------------------------------------------------------------------------------

void free_model(Model *mod) {
    free(mod->triangleArray.triangles);
}

void load_model_from_file(char *filename, Model *mod) {
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

    char str[10], str2[10], str3[10], str4[10], str5[10];

    char line[100];
    while(fgets(line,sizeof(line), fp) != NULL) {
        int a = sscanf(line, "%s %s %s %s %s", str, str2, str3, str4, str5);

        if (str[0] == 'p') {
            int size = ++pArray.size;
            resize_point_array(&pArray, size);
            pArray.points[size-1].posX = (int) strtol(str2, NULL, 10);
            pArray.points[size-1].posY = (int) strtol(str3, NULL, 10);
            pArray.points[size-1].posZ = (int) strtol(str4, NULL, 10);
            //printf("pont\n");
        } else if (strstr(str, "tri") != NULL) {
            resize_triangle_array(&tri_array, ++tri_array.size);
            int x,y,z;

            x = (int) strtol(str2+1, NULL, 10);
            y = (int) strtol(str3+1, NULL, 10);
            z = (int) strtol(str4+1, NULL, 10);

            tri_array.triangles[tri_array.size-1].p1 = pArray.points[x-1];
            tri_array.triangles[tri_array.size-1].p2 = pArray.points[y-1];
            tri_array.triangles[tri_array.size-1].p3 = pArray.points[z-1];

            if (a < 5) {
                tri_array.triangles[tri_array.size-1].r = 0;
                tri_array.triangles[tri_array.size-1].g = 0;
                tri_array.triangles[tri_array.size-1].b = 0;
            } else {
                char* temp = (char*) malloc(6*sizeof(char));

                strncpy(temp, str5,2);
                tri_array.triangles[tri_array.size-1].r = (int) strtol(temp, NULL, 16);

                strncpy(temp, str5+2,2);
                tri_array.triangles[tri_array.size-1].g = (int) strtol(temp, NULL, 16);

                strncpy(temp, str5+4,2);
                tri_array.triangles[tri_array.size-1].b = (int) strtol(temp, NULL, 16);

                //printf("r: %d g: %d b: %d\n", tri_array.triangles[tri_array.size-1].r, tri_array.triangles[tri_array.size-1].g, tri_array.triangles[tri_array.size-1].b);

                free(temp);
            }
        }
    }

    init_triangle_array(&mod->triangleArray,tri_array.size);
    for (int i = 0; i < tri_array.size; i++) {
        mod->triangleArray.triangles[i] = tri_array.triangles[i];
    }

    //mod->triangleArray.triangles = tri_array.triangles;

    fclose(fp);
    free_triangle_array(&tri_array);
    free_point_array(&pArray);
}

void load_Model_into_Object(Object *obj, Model model) {
    Model newModel;
    init_triangle_array(&newModel.triangleArray, model.triangleArray.size);

    for (int i = 0; i < newModel.triangleArray.size; i++) {
        newModel.triangleArray.triangles[i] = model.triangleArray.triangles[i];
    }
    //obj->model = model;
    obj->model = newModel;

    for (int i = 0; i < obj->model.triangleArray.size; i++) {
        obj->model.triangleArray.triangles[i].p1.posX += (obj->location.posX);
        obj->model.triangleArray.triangles[i].p1.posY += (obj->location.posY);
        obj->model.triangleArray.triangles[i].p1.posZ += (obj->location.posZ);

        obj->model.triangleArray.triangles[i].p2.posX += (obj->location.posX);
        obj->model.triangleArray.triangles[i].p2.posY += (obj->location.posY);
        obj->model.triangleArray.triangles[i].p2.posZ += (obj->location.posZ);

        obj->model.triangleArray.triangles[i].p3.posX += (obj->location.posX);
        obj->model.triangleArray.triangles[i].p3.posY += (obj->location.posY);
        obj->model.triangleArray.triangles[i].p3.posZ += (obj->location.posZ);
    }
}

void move_Object_to_Point(Object *obj, Point point) {
    Point deltaP;
    deltaP.posX = point.posX - obj->location.posX;
    deltaP.posY = point.posY - obj->location.posY;
    deltaP.posZ = point.posZ - obj->location.posZ;

    for (int i = 0; i < obj->model.triangleArray.size; i++) {
        obj->model.triangleArray.triangles[i].p1.posX += deltaP.posX;
        obj->model.triangleArray.triangles[i].p1.posY += deltaP.posY;
        obj->model.triangleArray.triangles[i].p1.posZ += deltaP.posZ;

        obj->model.triangleArray.triangles[i].p2.posX += deltaP.posX;
        obj->model.triangleArray.triangles[i].p2.posY += deltaP.posY;
        obj->model.triangleArray.triangles[i].p2.posZ += deltaP.posZ;

        obj->model.triangleArray.triangles[i].p3.posX += deltaP.posX;
        obj->model.triangleArray.triangles[i].p3.posY += deltaP.posY;
        obj->model.triangleArray.triangles[i].p3.posZ += deltaP.posZ;
    }

    obj->location = point;
}


void free_object(Object *obj) {
    free(obj->model.triangleArray.triangles);
}

Point weightPoint_of_triangle(triangle tri) {
    Point retP;
    retP.posX = (tri.p1.posX + tri.p2.posX + tri.p3.posX)/3;
    retP.posY = (tri.p1.posY + tri.p2.posY + tri.p3.posY)/3;
    retP.posZ = (tri.p1.posZ + tri.p2.posZ + tri.p3.posZ)/3;

    return retP;
}

double dist_btw_Points(Point p1, Point p2) {
    Point deltaP = {p2.posX - p1.posX, p2.posY - p1.posY, p2.posZ - p1.posZ};
    return  sqrt(deltaP.posX*deltaP.posX + deltaP.posY * deltaP.posY + deltaP.posZ * deltaP.posZ);
}

//                                          Rotation
//----------------------------------------------------------------------------------------------------------------------

Point rotate_Point_around_Point_wo_change(Point center, Point rotatedPoint, double rotX, double rotY, double rotZ) {
    Point returnP;
    double dx = rotatedPoint.posX - center.posX;
    double dy = rotatedPoint.posY - center.posY;
    double dz = rotatedPoint.posZ - center.posZ;

    //X-tengely körüli forgatas:
    returnP.posY = center.posY + dy * cos(rotX) - dz * sin(rotX);
    returnP.posZ = center.posZ + dz * cos(rotX) + dy * sin(rotX);


    //Y-tengely körüli forgatas:
    returnP.posX = center.posX + dx * cos(rotY) + dz * sin(rotY);
    returnP.posZ = center.posZ + dz * cos(rotY) - dx * sin(rotY);


    //Z-tengely körüli forgatas:
    returnP.posX = center.posX + dx * cos(rotZ) - dy * sin(rotZ);
    returnP.posY = center.posY + dx * sin(rotZ) + dy * cos(rotZ);

    return returnP;
}

void rotate_Point_around_Points_xAxis(Point center, Point *rotatedPoint, double rotX) {
    double dy = rotatedPoint->posY - center.posY;
    double dz = rotatedPoint->posZ - center.posZ;

    rotatedPoint->posY -= center.posY;
    rotatedPoint->posZ -= center.posZ;

    //X-tengely körüli forgatas:
    if (rotX != 0) {
        rotatedPoint->posY = dy * cos(rotX) - dz * sin(rotX);
        rotatedPoint->posZ = dz * cos(rotX) + dy * sin(rotX);
    }

    rotatedPoint->posY += center.posY;
    rotatedPoint->posZ += center.posZ;
}

void rotate_Point_around_Points_yAxis(Point center, Point *rotatedPoint, double rotY) {
    double dx = rotatedPoint->posX - center.posX;
    double dz = rotatedPoint->posZ - center.posZ;

    rotatedPoint->posX -= center.posX;
    rotatedPoint->posZ -= center.posZ;

    //Y-tengely körüli forgatas:
    if (rotY != 0) {
        rotatedPoint->posX = dx * cos(rotY) + dz * sin(rotY);
        rotatedPoint->posZ = dz * cos(rotY) - dx * sin(rotY);
    }

    rotatedPoint->posX += center.posX;
    rotatedPoint->posZ += center.posZ;
}

void rotate_Point_around_Points_zAxis(Point center, Point *rotatedPoint, double rotZ) {
    double dx = rotatedPoint->posX - center.posX;
    double dy = rotatedPoint->posY - center.posY;

    rotatedPoint->posX -= center.posX;
    rotatedPoint->posY -= center.posY;

    //Z-tengely körüli forgatas:
    if (rotZ != 0) {
        rotatedPoint->posX = dx * cos(rotZ) - dy * sin(rotZ);
        rotatedPoint->posY = dx * sin(rotZ) + dy * cos(rotZ);
    }

    rotatedPoint->posX += center.posX;
    rotatedPoint->posY += center.posY;
}

void rotate_Point_around_Point(Point center, Point *rotatedPoint, double rotX, double rotY, double rotZ) {
    rotate_Point_around_Points_xAxis(center, rotatedPoint, rotX);
    rotate_Point_around_Points_yAxis(center, rotatedPoint, rotY);
    rotate_Point_around_Points_zAxis(center, rotatedPoint, rotZ);
}

/* A haromszogek minden pontjat a center kozul elforgatjuk
 * a megfelelo szogben egyenkent, es igy az egesz objektum elfordul majd
 * */

void rotate_Object_around_Point(Point center, Object *obj, double rotX, double rotY, double rotZ) {
    for (int i = 0; i < obj->model.triangleArray.size; i++) {
        rotate_Point_around_Point(center, &obj->model.triangleArray.triangles[i].p1, rotX, rotY, rotZ);
        rotate_Point_around_Point(center, &obj->model.triangleArray.triangles[i].p2, rotX, rotY, rotZ);
        rotate_Point_around_Point(center, &obj->model.triangleArray.triangles[i].p3, rotX, rotY, rotZ);
    }
}
//----------------------------------------------------------------------------------------------------------------------

#endif