#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

typedef struct Point{int posX; int posY; int posZ} Point;
typedef struct din_point_array{Point *points; int size} din_point_array;

void init_point_array(din_point_array* din_array, int size) {
    din_array->size = size;
    din_array->points = (Point*) malloc(size * sizeof(Point));
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

void free_point_array(din_point_array *din_array) {
    free(din_array->points);
}

int main() {
    //Pontok
    din_point_array pArray;
    init_point_array(&pArray, 0);

    FILE *fp;

    fp = fopen("/home/coldus/Desktop/cube.txt", "r");

    if (fp == NULL) {
        perror("Fajl megnyitasa sikertelen");
        return NULL;
    }

    char str[10], str2[10], str3[10], str4[10];
    int p = pArray.size;

    while (fscanf(fp, "%s %s %s %s", str, str2, str3, str4) != EOF) {
        if (str[0] == 'p') {
            int size = ++pArray.size;
            resize_point_array(&pArray, size);
            pArray.points[p].posX = (int) strtol(str2, NULL, 10);
            pArray.points[p].posY = (int) strtol(str3, NULL, 10);
            pArray.points[p].posZ = (int) strtol(str4, NULL, 10);
        }
    }

    //for (int i = 0; i<pArray.size;i++)
    //    pArray.points[i].x = 10;

    printf("%d\n",pArray.size);

    /*(pArray.points[0]).x = 3;
    (pArray.points[1]).x = 3;
    (pArray.points[2]).x = 3;
    (pArray.points[3]).x = 3;
    (pArray.points[4]).x = 3;
    (pArray.points[5]).x = 3;
    (pArray.points[6]).x = 3;
    (pArray.points[7]).x = 3;*/

    fclose(fp);
    free_point_array(&pArray);

    return 0;
}