#include <stdbool.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

typedef struct Point{double posX; double posY; double posZ} Point;
typedef struct triangle{Point p1; Point p2; Point p3; int r; int g; int b} triangle;
typedef struct din_point_array{Point* points; int size} din_point_array;
typedef struct din_triangle_array{triangle* triangles; int size} triangle_array;
typedef struct Model{triangle_array triangleArray} Model;
typedef struct Object{Model model; Point location; double angle_from_x_axis; double angle_from_y_axis; double angle_from_z_axis} Object;

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
    triangle *new_tri = (triangle*) malloc(new_size * sizeof(triangle));
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

                printf("r: %d g: %d b: %d\n", tri_array.triangles[tri_array.size-1].r, tri_array.triangles[tri_array.size-1].g, tri_array.triangles[tri_array.size-1].b);

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


int main() {
    Model m;
    load_model_from_file("/home/coldus/Desktop/cube.txt", &m);

    /*for(int i = 0; i < m.triangleArray.size; i++)
        printf("%d\n",m.triangleArray.triangles->r);*/

    free_model(&m);

    return 0;
}