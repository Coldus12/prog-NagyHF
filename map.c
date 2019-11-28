//
// Created by coldus on 11/17/19.
//
#ifndef MAP
#define MAP

#include <stdlib.h>
#include <stdio.h>
#include "object.h"

//Debugmalloc:
#include "debugmalloc-impl.h"
#include "debugmalloc.h"

typedef struct object_list{Object obj; struct object_list *next} map;
typedef struct model_list{Model model; char name[50]; struct model_list *next} model_list;
typedef struct invis_wall{Point location; Point p; struct invis_wall* next; double size} invis_wall;

//        Függvény, ami eldönti, hogy egy pont jobbra vagy balra van két másik pontoz képest (Jobb körüljárással)
//----------------------------------------------------------------------------------------------------------------------
bool is_it_left(Point first, Point second, Point pointInQuestion) {
    if (first.posX != second.posX) {
        double m = ((double) second.posZ - first.posZ) / (double) (second.posX - first.posX);
        //y-y1 = m(x-x1)
        //mx1-y1=mx-y
        //x,y eldontendo x és y-ja
        //x1,y1 elso pont x és y-ja
        double konstans = m * first.posX - first.posZ;
        if (first.posX < second.posX) {
            if(konstans > (m * pointInQuestion.posX - pointInQuestion.posZ)) {
                return false;
            /*} else if (konstans == (m * pointInQuestion.posX - pointInQuestion.posZ)) {
                //printf("benne van shit\n");
                return true;*/
            } else {
                return true;
            }
        } else {
            if(konstans < (m * pointInQuestion.posX - pointInQuestion.posZ)) {
                return false;
            /*} else if (konstans == (m * pointInQuestion.posX - pointInQuestion.posZ)) {
                //printf("benne van shit\n");
                return true;*/
            } else {
                return true;
            }
        }
    } else {
        if (pointInQuestion.posX < first.posX)
            return true;
        else
            return false;
    }
}

//                                         invis_wall függvényei
//----------------------------------------------------------------------------------------------------------------------
invis_wall* add_point_to_inviswall(invis_wall* head, Point new_point, Point location, double size) {
    invis_wall* new_item = malloc(sizeof(invis_wall));

    new_point.posX *= size;
    new_point.posY *= size;
    new_point.posZ *= size;

    new_point.posX += location.posX;
    new_point.posY += location.posY;
    new_point.posZ += location.posZ;

    new_item->size = size;
    new_item->p = new_point;
    new_item->next = NULL;

    if (head == NULL)
        return new_item;

    invis_wall* iter = head;
    while (iter->next != NULL) {
        iter = iter->next;
    }
    iter->next = new_item;
    return head;
}

void free_invis_wall(invis_wall* head) {
    invis_wall* iter = head;
    while (iter != NULL) {
        invis_wall* tmp = iter->next;
        free(iter);
        iter = tmp;
    }
}

bool point_inside_invis_walls(invis_wall* head, Point p) {
    invis_wall* iter = head;

    while (iter != NULL) {
        //printf("X: %.0lf Y: %.0lf Z: %.0lf",iter->p.posX,iter->p.posY, iter->p.posZ);
        if (iter->next != NULL) {
            if (!is_it_left(iter->p, iter->next->p, p))
                return false;
        } else {
            if (!is_it_left(iter->p, head->p, p))
                return false;
        }
        iter = iter->next;
    }
    return true;
}

bool point_outside_invis_walls(invis_wall* head, Point p) {
    invis_wall* iter = head;
    while (iter != NULL) {
        if (iter->next != NULL) {
            if (!is_it_left(iter->p, iter->next->p, p))
                //printf("mé\n");
                return true;
        } else {
            if (!is_it_left(iter->p, head->p, p))
                return true;
        }
        iter = iter->next;
    }
    return false;
}

invis_wall* load_invis_wall_from_file(char* path, Point location, double size) {
    invis_wall* head = NULL;
    FILE *fp = fopen(path,"r");
    if (fp == NULL) {
        printf("Nem sikerült a fájl megnyitása");
        return NULL;
    }

    char line[100];
    while (fgets(line,sizeof(line), fp) != NULL) {
        Point new_point;
        char posx[20];
        char posz[20];
        sscanf(line, "%s %s", posx, posz);
        //printf("%s %s\n",posx,posz);
        new_point.posY = 0;
        new_point.posX = strtod(posx,NULL);
        new_point.posZ = strtod(posz,NULL);
        //printf("%.0lf %.0lf\n", new_point.posX, new_point.posZ);

        head = add_point_to_inviswall(head,new_point, location, size);
    }

    fclose(fp);
    return head;
}

invis_wall* rotate_invis_wall(invis_wall* head, double rotX, double rotY, double rotZ) {
    for (invis_wall* iter = head; iter != NULL; iter = iter->next) {
        rotate_Point_around_Point(iter->location,&iter->p,rotX,rotY,rotZ);
    }
    return head;
}

invis_wall* move_invis_wall(invis_wall* head, Point new_location) {
    for (invis_wall* iter = head; iter != NULL; iter = iter->next) {
        double dx = new_location.posX-iter->location.posX;
        double dy = new_location.posY-iter->location.posY;
        double dz = new_location.posZ-iter->location.posZ;

        iter->p.posX += dx;
        iter->p.posY += dy;
        iter->p.posZ += dz;

        iter->location = new_location;
    }
    return head;
}

invis_wall* resize_invis_wall(invis_wall* head, double new_size) {
    for (invis_wall* iter = head; iter != NULL; iter = iter->next) {
        iter->p.posX -= iter->location.posX;
        iter->p.posY -= iter->location.posY;
        iter->p.posZ -= iter->location.posZ;

        iter->p.posX *= (new_size/iter->size);
        iter->p.posY *= (new_size/iter->size);
        iter->p.posZ *= (new_size/iter->size);

        iter->p.posX += iter->location.posX;
        iter->p.posY += iter->location.posY;
        iter->p.posZ += iter->location.posZ;

        iter->size = new_size;
    }
    return head;
}

void print_invis(invis_wall* head) {
    for (invis_wall* iter = head; iter != NULL; iter = iter->next) {
        printf("%.0lf %.0lf\n",iter->p.posX, iter->p.posZ);
    }
}

//                                             map függvényei
//----------------------------------------------------------------------------------------------------------------------

map* add_object_to_map(map *head, Object obj) {
    map *newItem = (map*) malloc(sizeof(map));
    newItem->obj = obj;
    newItem->next = head;

    return newItem;
}

void free_object_list(map *head) {
    map* iter = head;
    while (iter != NULL) {
        map *next = iter->next;
        free_object(&iter->obj);
        free(iter);
        iter = next;
    }
}

//                                          model_list függvényei
//----------------------------------------------------------------------------------------------------------------------

model_list* add_to_model_list(model_list *head, char* name, Model model) {
    model_list *newItem = (model_list*) malloc(sizeof(model_list));
    newItem->model = model;
    strcpy(newItem->name, name);
    newItem->next = head;

    return newItem;
}

model_list* load_model_list(char *path_to_location) {
    char line[200];
    FILE *fp = fopen(path_to_location,"r");
    model_list* head = NULL;
    char name[50];
    char path[150];

    while(fgets(line, sizeof(line), fp)) {
        Model model;
        //char *name = (char*) malloc(50*sizeof(char));
        //char *path = (char*) malloc(150*sizeof(char));

        sscanf(line, "%s %s", name, path);
        load_model_from_file(path, &model);
        head = add_to_model_list(head, name, model);
    }

    fclose(fp);
    return head;
}

void free_model_list(model_list *head) {
    model_list* iter = head;
    while (iter != NULL) {
        model_list *next = iter->next;
        free_model(&iter->model);
        free(iter);
        iter = next;
    }
}

bool does_model_list_contain(model_list *head, char *name) {
    model_list *current = head;
    while (current != NULL) {
        if (strcmp(current->name, name) == 0) {
            return true;
        }
        current = current->next;
    }
    return false;
}

Model load_from_list(model_list *head, char *name) {
    model_list *current = head;
    while(current != NULL) {
        if (strcmp(name, current->name) == 0) {
            return current->model;
        }
        current = current->next;
    }

}

//                                     Map betöltésének függvénye
//----------------------------------------------------------------------------------------------------------------------

map* load_map_from_file(char *filename, map *map, model_list modelList) {
    FILE *fp = fopen(filename, "r");

    //char *string = (char*) malloc(200*sizeof(char));
    //char *string = calloc(100, sizeof(char));
    char string[200];
    char name[50] = {0};
    char posX[50], posY[50], posZ[50], size[50], rotX[50], rotY[50], rotZ[50];
    //double posX, posY, posZ, size, rotX, rotY, rotZ;
    int b;

    while(fgets(string, sizeof(string),fp) != NULL) {
        sscanf(string, "%s %s %s %s %s %s %s %s", name, posX, posY, posZ, size, rotX, rotY, rotZ);
        printf("%s\n",string);
        if (does_model_list_contain(&modelList, name)) {
            //printf("positions: x: %s y: %s z: %s size: %s rotX: %s rotY: %s rotZ: %s\n", posX, posY, posZ, size, rotX, rotY, rotZ);
            Object obj;
            obj.location.posX = (double) strtol(posX, NULL, 10);
            obj.location.posY = (double) strtol(posY, NULL, 10);
            obj.location.posZ = (double) strtol(posZ, NULL, 10);

            load_Model_into_Object(&obj, load_from_list(&modelList, name), strtod(size, NULL));

            rotate_Object_around_Point(obj.location, &obj, strtod(rotX, NULL), strtod(rotY,NULL), strtod(rotZ, NULL));

            map = add_object_to_map(map, obj);
        }
    }

    //free(string);
    fclose(fp);
    return map;
}

#endif