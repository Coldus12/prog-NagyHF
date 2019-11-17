//
// Created by coldus on 11/17/19.
//
#ifndef MAP
#define MAP

#include <stdlib.h>
#include <stdio.h>
#include "object.h"

typedef struct object_list{Object obj; struct object_list *next} object_list;
typedef struct model_list{Model model; char name[50]; struct model_list *next} model_list;

object_list* add_to_object_list(object_list *head, Object obj) {
    object_list *newItem = (object_list*) malloc(sizeof(object_list));
    newItem->obj = obj;
    newItem->next = head;

    return newItem;
}

void free_object_list(object_list *head) {
    object_list* iter = head;
    while (iter != NULL) {
        object_list *next = iter->next;
        free(iter);
        iter = next;
    }
}

//----------------------------------------------------------------------------------------------------------------------

model_list* add_to_model_list(model_list *head, char* name,Model model) {
    model_list *newItem = (model_list*) malloc(sizeof(model_list));
    newItem->model = model;
    newItem->next = head;

    return newItem;
}

void free_model_list(model_list *head) {
    model_list* iter = head;
    while (iter != NULL) {
        model_list *next = iter->next;
        free(iter);
        iter = next;
    }
}

bool does_model_list_contain(model_list *head, char *name) {
    model_list *current = head;
    while (current->next != NULL) {
        if (current->name == name) {
            return true;
        }
    }
    return false;
}

Model load_from_list(model_list *head, char *name) {
    model_list *current = head;
    while(current != NULL) {
        if (current->name == name) {
            return current->model;
        }
    }

}

//----------------------------------------------------------------------------------------------------------------------

void load_map_from_file(char *filename, object_list *objectList) {
    FILE *fp = fopen(filename, "r");

    char *string = (char*) malloc(100*sizeof(char));
    char name[50];
    double posX, posY, posZ, size, rotX, rotY, rotZ;
    int b;
    model_list *mod_head = NULL;

    while(fgets(string, sizeof(string),fp)) {
        b = sscanf(string, "%s %lf %lf %lf %lf %lf %lf %lf", name, &posX, &posY, &posZ, &size, &rotX, &rotY, &rotZ);

        if (!does_model_list_contain(mod_head, name)) {
            Model mod;
            load_model_from_file("/home/coldus/Desktop/cube.txt", &mod);
            add_to_model_list(mod_head, name, mod);
        }

        Object obj;
        obj.location.posX = posX;
        obj.location.posY = posY;
        obj.location.posZ = posZ;
        load_Model_into_Object(&obj, load_from_list(mod_head, name));
        rotate_Object_around_Point(obj.location, &obj, rotX, rotY, rotZ);

        objectList = add_to_object_list(objectList, obj);
    }

    free(string);
    fclose(fp);
}

#endif