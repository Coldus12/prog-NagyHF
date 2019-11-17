//
// Created by coldus on 11/17/19.
//

#ifndef NAGYHF_MAP_H
#define NAGYHF_MAP_H
typedef struct object_list{Object obj; struct object_list *next} object_list;
typedef struct model_list{Model model; char name[50]; struct model_list *next} model_list;


object_list* add_to_object_list(object_list *head, Object obj);
void free_object_list(object_list *head);
model_list* add_to_model_list(model_list *head, char* name,Model model);
void free_model_list(model_list *head);
bool does_model_list_contain(model_list *head, char *name);
Model load_from_list(model_list *head, char *name);
void load_map_from_file(char *filename, object_list *objectList);

#endif //NAGYHF_MAP_H
