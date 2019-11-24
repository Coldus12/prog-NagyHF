//
// Created by coldus on 11/17/19.
//
#ifndef MAP
#define MAP

#include <stdlib.h>
#include <stdio.h>
#include "object.h"

typedef struct object_list{Object obj; struct object_list *next} map;
typedef struct model_list{Model model; char name[50]; struct model_list *next} model_list;

map* add_object_to_map(map *head, Object obj) {
    printf("eh\n");
    printf("%lf\n",obj.model.triangleArray.triangles[0].p1.posX);
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

model_list* add_to_model_list(model_list *head, char* name,Model model) {
    model_list *newItem = (model_list*) malloc(sizeof(model_list));
    newItem->model = model;
    newItem->next = head;

    return newItem;
}

model_list* load_model_list(char *path_to_location) {
    char line[200] = {0};
    FILE *fp = fopen(path_to_location,"r");
    model_list* head = NULL;

    while(fgets(line, sizeof(line), fp)) {
        Model model;
        char name[50];
        char path[150];

        sscanf(line, "%s %s", name, path);
        printf("%s %s\n", name, path);
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
        printf("%d\n", strcmp(current->name, name));
        printf("%s %s\n", current->name, name);
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

//----------------------------------------------------------------------------------------------------------------------

map* load_map_from_file(char *filename, map *map, model_list modelList) {
    FILE *fp = fopen(filename, "r");

    char *string = (char*) malloc(100*sizeof(char));
    //char *string = calloc(100, sizeof(char));
    char name[50] = {0};
    double posX, posY, posZ, size, rotX, rotY, rotZ;
    int b;

    while(fgets(string, sizeof(string),fp)) {
        printf("why?\n");
        b = sscanf(string, "%s %lf %lf %lf %lf %lf %lf %lf", name, &posX, &posY, &posZ, &size, &rotX, &rotY, &rotZ);
        if (does_model_list_contain(&modelList, name)) {
            printf("heya");
            Object obj;
            obj.location.posX = posX;
            obj.location.posY = posY;
            obj.location.posZ = posZ;

            load_Model_into_Object(&obj, load_from_list(&modelList, name));

            rotate_Object_around_Point(obj.location, &obj, rotX, rotY, rotZ);

            map = add_object_to_map(map, obj);
        }
    }

    free(string);
    fclose(fp);
    return map;
}

#endif