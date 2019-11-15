//
// Created by coldus on 11/5/19.
//
#ifndef renderer
#define renderer

#include <SDL_render.h>
#include <SDL2_gfxPrimitives.h>

#include "object.h"

//Debugmalloc:
#include "debugmalloc-impl.h"
#include "debugmalloc.h"

typedef struct Camera{Point location; int viewDistance; int planeSizeX; int planeSizeY; int distanceFromPlane; double rotX; double rotY; double rotZ} Camera;

/* Ha egy pont túl megy a síkon, akkor nem a renderelést
 * kellene megszüntetni, hanem a pont-kamera egyenes és a
 * kamera síkjának metszéspontjában kellene a pont képének
 * lennie, így az "torlódna" a síkon, és nem kell azzal
 * törődni, hogy túl ment a síkon, és így már a trigont
 * amiben van sem lehet renderelni.
 * */

Point interRenderPoint(Camera cam, Point p) {

    Point rotated;
    rotated = p;

    rotate_Point_around_Point(cam.location, &rotated, cam.rotX, cam.rotY, cam.rotZ);

    Point vetulet;
    vetulet.posZ = 0;

    double relativY = cam.distanceFromPlane*((rotated.posY - cam.location.posY)/(rotated.posZ - cam.location.posZ));
    double relativX = cam.distanceFromPlane*((rotated.posX - cam.location.posX)/(rotated.posZ - cam.location.posZ));

    if (rotated.posZ - cam.location.posZ <= 0) {
        vetulet.posZ = -1;
    } else {
        //Szamolas
        vetulet.posY = ((double) cam.planeSizeY/2) - relativY;
        vetulet.posX = ((double) cam.planeSizeX/2) + relativX;
    }

    return vetulet;
}

void renderTriangle(triangle tri, Camera cam, SDL_Renderer *SDL_renderer) {
    Point p1, p2, p3;

    p1 = interRenderPoint(cam, tri.p1);
    p2 = interRenderPoint(cam, tri.p2);
    p3 = interRenderPoint(cam, tri.p3);

    //printf("%d %d %d\n", tri.r, tri.g, tri.b);

    if (p1.posZ != -1 && p2.posZ != -1 && p3.posZ != -1)
        filledTrigonRGBA(SDL_renderer, p1.posX, p1.posY, p2.posX, p2.posY, p3.posX, p3.posY, tri.r, tri.g, tri.b, 255);
    //trigonRGBA(SDL_renderer, p1.posX, p1.posY, p2.posX, p2.posY, p3.posX, p3.posY, tri.r, tri.g, tri.b, 255);
}

void renderObject(Object object, Camera cam, SDL_Renderer *SDL_renderer) {
    //order_tri_array(cam, &object);
    int a = object.model.triangleArray.size;
    for (int i = 0; i < a; i++) {
        //printf("%d ",i);
        renderTriangle(object.model.triangleArray.triangles[i], cam, SDL_renderer);
    }
}

/*void correctly_render_object(Object object, Camera cam, SDL_Renderer *SDL_renderer) {
    RenderList *head = NULL;

    for (int i = 0; i < object.model.triangleArray.size; i++) {
        head = addtoList(head, object.model.triangleArray.triangles[i], dist_btw_Points(cam.location, weightPoint_of_triangle(object.model.triangleArray.triangles[i])), true);
    }

    freeList(head);
}*/

//                                              RenderList
//----------------------------------------------------------------------------------------------------------------------
typedef struct RenderList{triangle *tri; double dist; bool visible; struct RenderList *next} RList;

RList* addtoRenderList(RList *head, triangle *tri, double dist, bool visible) {
    RList *newItem = (RList*) malloc(sizeof(RList));
    newItem->tri = tri;
    newItem->dist = dist;
    newItem->visible = visible;
    newItem->next = head;

    return newItem;
}

RList* addObjectRenderList(RList *head, Camera cam, Object *obj) {
    for (int i = 0; i < obj->model.triangleArray.size; i++) {
        head = addtoRenderList(head, &obj->model.triangleArray.triangles[i], dist_btw_Points(cam.location, weightPoint_of_triangle(obj->model.triangleArray.triangles[i])), true);
    }
    return head;
}

void freeList(RList *head) {
    RList *iter = head;
    while (iter != NULL) {
        RList *next = iter->next;
        free(iter);
        iter = next;
    }
}

void orderList(RList *head) {
    RList *current = head;
    while (current != NULL) {
        //gyors rendezés láncolt listán
        current = current->next;
    }
}

void render_RList(RList *head, Camera cam, SDL_Renderer *SDL_Renderer) {
    RList *current = head;
    while (current != NULL) {
        renderTriangle(*current->tri, cam, SDL_Renderer);
        current = current->next;
    }
}


#endif
