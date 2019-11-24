//
// Created by coldus on 11/17/19.
//

#ifndef NAGYHF_MAP_H
#define NAGYHF_MAP_H

#include "object.h"

typedef struct object_list{Object obj; struct object_list *next} map;
typedef struct model_list{Model model; char name[50]; struct model_list *next} model_list;

map* add_object_to_map(map *head, Object obj);
void free_object_list(map *head);
model_list* add_to_model_list(model_list *head, char* name,Model model);
model_list* load_model_list(char *path_to_location);
void free_model_list(model_list *head);
bool does_model_list_contain(model_list head, char *name);
Model load_from_list(model_list head, char *name);
map* load_map_from_file(char *filename, map *map, model_list modelList);

#endif //NAGYHF_MAP_H
