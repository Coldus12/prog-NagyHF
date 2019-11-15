//
// Created by coldus on 11/5/19.
//

#ifndef NAGYHF_RENDERER_H
#define NAGYHF_RENDERER_H

#include <SDL_render.h>
#include <SDL2_gfxPrimitives.h>

#include "object.h"

//Debugmalloc:
#include "debugmalloc-impl.h"
#include "debugmalloc.h"

typedef struct Camera{Point location; int viewDistance; int planeSizeX; int planeSizeY; int distanceFromPlane; double rotX; double rotY; double rotZ} Camera;
Point interRenderPoint(Camera cam, Point p);
void renderTriangle(triangle tri, Camera cam, SDL_Renderer *SDL_renderer);
void renderObject(Object object, Camera cam, SDL_Renderer *SDL_renderer);

//                                                   RenderList
//----------------------------------------------------------------------------------------------------------------------
typedef struct RenderList{triangle *tri; double dist; bool visible; struct RenderList *next} RList;
RList* addtoRenderList(RList *head, triangle *tri, double dist, bool visible);
RList* addObjectRenderList(RList *head, Camera cam, Object *obj);
void freeList(RList *head);
void orderList(RList *head);
void render_RList(RList *head, Camera cam, SDL_Renderer *SDL_Renderer);


#endif //NAGYHF_RENDERER_H
