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

    if (p1.posZ != -1 && p2.posZ != -1 && p3.posZ != -1) {
        filledTrigonRGBA(SDL_renderer, p1.posX, p1.posY, p2.posX, p2.posY, p3.posX, p3.posY, tri.r, tri.g, tri.b, 255);
        //trigonRGBA(SDL_renderer, p1.posX, p1.posY, p2.posX, p2.posY, p3.posX, p3.posY, tri.r, tri.g, tri.b, 255);
    }
}

//Külön RenderList-en kívűli renderelés
void renderObject(Object object, Camera cam, SDL_Renderer *SDL_renderer) {
    //order_tri_array(cam, &object);
    int a = object.model.triangleArray.size;
    for (int i = 0; i < a; i++) {
        //printf("%d ",i);
        renderTriangle(object.model.triangleArray.triangles[i], cam, SDL_renderer);
    }
}

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

//                                    Lista rendezése, és hozzátartozó függvények
//----------------------------------------------------------------------------------------------------------------------

RList* getListItem(RList* head, int posInList) {
    RList* current = head;
    for (int i = 0; i < posInList; i++) {
        current = current->next;
    }
    return current;
}

RList* swap(RList *head, int pos1, int pos2) {
    RList *ret = head;
    RList *prev1, *prev2, *c1, *c2;

    if (abs(pos1-pos2) > 1) {
        if (pos1 == 0) {
            prev2 = getListItem(head, pos2-1);
            c1 = head;
            c2 = prev2->next;
            ret = c2;

            prev2->next = c1;

            RList* temp = c1->next;
            c1->next = c2->next;
            c2->next = temp;
        } else if (pos2 == 0) {
            prev1 = getListItem(head, pos1-1);
            c2 = head;
            c1 = prev1->next;
            ret = c1;

            prev1->next = c2;

            RList* temp = c2->next;
            c2->next = c1->next;
            c1->next = temp;
        } else {
            prev1 = getListItem(head, pos1-1);
            prev2 = getListItem(head, pos2-1);

            c1 = prev1->next;
            c2 = prev2->next;

            RList* temp = c1->next;
            c1->next = c2->next;
            c2->next = temp;

            prev1->next = c2;
            prev2->next = c1;
            ret = head;
        }
    } else if (pos1 == pos2) {
        return  head;
    } else {
        if (pos1 == 0 || pos2 == 0) {
            c1 = getListItem(head, 0);
            c2 = c1->next;

            //RList *temp;
            c1->next = c2->next;
            c2->next = c1;
            head = c2;

            return head;
        } else {
            if (pos1 > pos2) {
                prev2 = getListItem(head, pos2-1);
            } else {
                prev2 = getListItem(head, pos1-1);
            }

            c1 = prev2->next->next;
            c2 = prev2->next;

            RList *temp = c1->next;
            c1->next = c2;
            c2->next = temp;

            prev2->next = c1;
            return head;
        }
    }

    return ret;
}

RList* bubble_sort_by_dist(RList *head) {
    int size = 0;
    RList *current = head;
    while (current != NULL) {
        size++;
        current = current->next;
    }

    for (int i = size-1; i > 0; --i) {
        for (int j = 0; j < i; ++j) {
            if (getListItem(head, i)->dist >= getListItem(head, j)->dist) {
                head = swap(head, i, j);
            }
        }
    }

    return head;
}

RList* update_distances(RList* head, Camera cam) {
    RList *current = head;
    while (current != NULL) {
        current->dist = dist_btw_Points(cam.location, weightPoint_of_triangle(*current->tri));
        current = current->next;
    }
    return head;
}

//                                      RList render függvénye
//----------------------------------------------------------------------------------------------------------------------

RList* render_RList(RList *head, Camera cam, SDL_Renderer *SDL_Renderer) {
    head = update_distances(head, cam);
    head = bubble_sort_by_dist(head);
    RList *current = head;
    while (current != NULL) {
        if (current->dist > cam.viewDistance) {
            return head;
        }
        renderTriangle(*current->tri, cam, SDL_Renderer);
        current = current->next;
    }

    return head;
}


#endif
