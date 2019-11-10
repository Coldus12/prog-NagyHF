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
    Point vetulet;
    //vetulet.posZ = cam.location.posZ + cam.distanceFromPlane;
    vetulet.posZ = 0;

    double relativY = cam.distanceFromPlane*((p.posY - cam.location.posY)/(p.posZ - cam.location.posZ));
    double relativX = cam.distanceFromPlane*((p.posX - cam.location.posX)/(p.posZ - cam.location.posZ));

    //Szamolas
    vetulet.posY = ((double) cam.planeSizeY/2) - relativY;
    vetulet.posX = ((double) cam.planeSizeX/2) + relativX;

    return vetulet;
}

void renderTriangle(triangle triangle, Camera cam, SDL_Renderer *SDL_renderer) {
    Point p1, p2, p3;

    /*p1 = interRenderPoint(cam, rotate_Point_around_Point_wo_change(cam.location, triangle.p1, cam.rotX, cam.rotY, cam.rotZ));
    p2 = interRenderPoint(cam, rotate_Point_around_Point_wo_change(cam.location, triangle.p2, cam.rotX, cam.rotY, cam.rotZ));
    p3 = interRenderPoint(cam, rotate_Point_around_Point_wo_change(cam.location, triangle.p3, cam.rotX, cam.rotY, cam.rotZ));*/
    p1 = interRenderPoint(cam, triangle.p1);
    p2 = interRenderPoint(cam, triangle.p2);
    p3 = interRenderPoint(cam, triangle.p3);

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
