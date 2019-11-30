//
// Created by coldus on 11/5/19.
//

#ifndef NAGYHF_RENDERER_H
#define NAGYHF_RENDERER_H

#include <SDL_render.h>
#include <SDL2_gfxPrimitives.h>

#include "map.h"

//Debugmalloc:
#include "debugmalloc-impl.h"
#include "debugmalloc.h"

typedef struct Camera{Point location; int viewDistance; int planeSizeX; int planeSizeY; int distanceFromPlane; double rotX; double rotY; double rotZ} Camera;

Point interRenderPoint(Camera cam, Point p);
void renderTriangle(triangle tri, Camera cam, SDL_Renderer *SDL_renderer);
void renderObject(Object object, Camera cam, SDL_Renderer *SDL_renderer);

//                                                   RenderList
//----------------------------------------------------------------------------------------------------------------------
typedef struct RenderList{triangle *tri; double dist; struct RenderList *next} RList;
RList* addtoRenderList(RList *head, triangle *tri, double dist, bool visible);
RList* addObjectToRenderList(RList *head, Camera cam, Object *obj);
RList* addMapToRenderList(RList *head, Camera cam, map *map1);
void freeList(RList *head);
RList* update_distances(RList* head, Camera cam);
RList* render_RList(RList *head, Camera cam, SDL_Renderer *SDL_Renderer);

typedef struct RenderArray{triangle **tri; int size} RenderArray;

void init_render_array(RenderArray* din_array, int size);
bool resize_render_array(RenderArray *din_array, int new_size);
void free_render_array(RenderArray* din_array);
void add_tri_to_RenderArray(RenderArray *ra, triangle* tri, double dist);
void add_Object_to_RenderArray(RenderArray *ra, Camera cam, Object *obj);
void addMapToRenderArray(RenderArray *ra,map *map1, Camera cam);
void quicksort_on_render_array(RenderArray *ra, int min, int max);
void update_distances_on_ra(RenderArray *ra, Camera cam);
void render_ra(RenderArray *ra, Camera cam, SDL_Renderer *SDL_Renderer);


#endif //NAGYHF_RENDERER_H
