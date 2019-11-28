//
// Created by coldus on 11/17/19.
//

#ifndef NAGYHF_MAP_H
#define NAGYHF_MAP_H

#include "object.h"

typedef struct object_list{Object obj; struct object_list *next} map;
typedef struct model_list{Model model; char name[50]; struct model_list *next} model_list;
typedef struct invis_wall{Point location; Point p; struct invis_wall* next; double size} invis_wall;

bool is_it_left(Point first, Point second, Point pointInQuestion);
invis_wall* add_point_to_inviswall(invis_wall* head, Point new_point, Point location, double size);
invis_wall* load_invis_wall_from_file(char* path, Point location, double size);
void free_invis_wall(invis_wall* head);
bool point_inside_invis_walls(invis_wall* head, Point p);
bool point_outside_invis_walls(invis_wall* head, Point p);
invis_wall* move_invis_wall(invis_wall* head, Point new_location);
invis_wall* rotate_invis_wall(invis_wall* head, double rotX, double rotY, double rotZ);
invis_wall* resize_invis_wall(invis_wall* head, double new_size);
void print_invis(invis_wall* head);

map* add_object_to_map(map *head, Object obj);
void free_object_list(map *head);
model_list* add_to_model_list(model_list *head, char* name, Model model);
model_list* load_model_list(char *path_to_location);
void free_model_list(model_list *head);
bool does_model_list_contain(model_list *head, char *name);
Model load_from_list(model_list *head, char *name);
map* load_map_from_file(char *filename, map *map, model_list modelList);

#endif //NAGYHF_MAP_H
