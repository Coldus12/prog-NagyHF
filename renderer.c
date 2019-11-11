//
// Created by coldus on 11/5/19.
//
#ifndef renderer
#define renderer

#include <SDL_render.h>
#include <SDL2_gfxPrimitives.h>

#include "object.h"

typedef struct Camera{Point location; int viewDistance; int planeSizeX; int planeSizeY; int distanceFromPlane; double rotX; double rotY; double rotZ} Camera;

Point interRenderPoint(Camera cam, Point p) {

    Point rotated; // = rotate_Point_around_Point_wo_change(cam.location, p, cam.rotX, cam.rotY, cam.rotZ);
    rotated = p;

    //Mivel ittez igy lett megoldva, felteheoleg magat a rotate fuggvenyt 3 reszre kellene osztani,
    //majd azt egybe rakni igy, mint ahogy az itt van.

    /*rotate_Point_around_Point(cam.location, &rotated, cam.rotX, 0, 0);
    rotate_Point_around_Point(cam.location, &rotated, 0, cam.rotY, 0);
    rotate_Point_around_Point(cam.location, &rotated, 0, 0, cam.rotZ);*/

    rotate_Point_around_Point(cam.location, &rotated, cam.rotX, cam.rotY, cam.rotZ);

    Point vetulet;
    vetulet.posZ = 0;

    double relativY = cam.distanceFromPlane*((rotated.posY - cam.location.posY)/-(rotated.posZ - cam.location.posZ));
    double relativX = cam.distanceFromPlane*((rotated.posX - cam.location.posX)/-(rotated.posZ - cam.location.posZ));

    //Szamolas
    vetulet.posY = ((double) cam.planeSizeY/2) - relativY;
    vetulet.posX = ((double) cam.planeSizeX/2) + relativX;

    return vetulet;
}

void renderTriangle(triangle tri, Camera cam, SDL_Renderer *SDL_renderer) {
    Point p1, p2, p3;

    p1 = interRenderPoint(cam, tri.p1);
    p2 = interRenderPoint(cam, tri.p2);
    p3 = interRenderPoint(cam, tri.p3);

    //filledTrigonRGBA(SDL_renderer, p1.posX, p1.posY, p2.posX, p2.posY, p3.posX, p3.posY, 0, 0, 0, 255);
    trigonRGBA(SDL_renderer, p1.posX, p1.posY, p2.posX, p2.posY, p3.posX, p3.posY, 0, 0, 0, 255);
}

void renderObject(Object object, Camera cam, SDL_Renderer *SDL_renderer) {
    int a = object.model.triangleArray.size;
    for (int i = 0; i < a; i++) {
        //printf("%d ",i);
        renderTriangle(object.model.triangleArray.triangles[i], cam, SDL_renderer);
    }
}


#endif
