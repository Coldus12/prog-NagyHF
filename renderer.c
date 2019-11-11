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
    /*Point vetulet;
    vetulet.posZ = 0;

    double relativY = cam.distanceFromPlane*((p.posY - cam.location.posY)/(p.posZ - cam.location.posZ));
    double relativX = cam.distanceFromPlane*((p.posX - cam.location.posX)/(p.posZ - cam.location.posZ));

    //Szamolas
    vetulet.posY = ((double) cam.planeSizeY/2) - relativY;
    vetulet.posX = ((double) cam.planeSizeX/2) + relativX;

     return vetulet;*/

    Point rotated; // = rotate_Point_around_Point_wo_change(cam.location, p, cam.rotX, cam.rotY, cam.rotZ);
    rotated = p;
    rotate_Point_around_Point(cam.location, &rotated, cam.rotX, cam.rotY, cam.rotZ);
    Point vetulet;
    vetulet.posZ = 0;

    double relativY = cam.distanceFromPlane*((rotated.posY - cam.location.posY)/(rotated.posZ - cam.location.posZ));
    double relativX = cam.distanceFromPlane*((rotated.posX - cam.location.posX)/(rotated.posZ - cam.location.posZ));

    //Szamolas
    vetulet.posY = ((double) cam.planeSizeY/2) - relativY;
    vetulet.posX = ((double) cam.planeSizeX/2) + relativX;

    return vetulet;
}

void renderTriangle(triangle tri, Camera cam, SDL_Renderer *SDL_renderer) {
    Point p1, p2, p3;

    /*Point c = {0, 0, 0};

    Point x = {1, 0, 0};
    Point y = {0, 1, 0};
    Point z = {0, 0, 1};

    //Rotating the basis vectors
    rotate_Point_around_Point(c, &x, cam.rotX, cam.rotY, cam.rotZ);
    rotate_Point_around_Point(c, &y, cam.rotX, cam.rotY, cam.rotZ);
    rotate_Point_around_Point(c, &z, cam.rotX, cam.rotY, cam.rotZ);

    double xBase = x.posX + y.posX + z.posX;
    double yBase = x.posY + y.posY + z.posY;
    double zBase = x.posZ + y.posZ + z.posZ;

    triangle visible_tri;
    visible_tri.p1.posX = xBase * tri.p1.posX;
    visible_tri.p1.posY = yBase * tri.p1.posY;
    visible_tri.p1.posZ = zBase * tri.p1.posZ;

    visible_tri.p2.posX = xBase * tri.p2.posX;
    visible_tri.p2.posY = yBase * tri.p2.posY;
    visible_tri.p2.posZ = zBase * tri.p2.posZ;

    visible_tri.p3.posX = xBase * tri.p3.posX;
    visible_tri.p3.posY = yBase * tri.p3.posY;
    visible_tri.p3.posZ = zBase * tri.p3.posZ;*/

    /*p1 = interRenderPoint(cam, rotate_Point_around_Point_wo_change(cam.location, triangle.p1, cam.rotX, cam.rotY, cam.rotZ));
    p2 = interRenderPoint(cam, rotate_Point_around_Point_wo_change(cam.location, triangle.p2, cam.rotX, cam.rotY, cam.rotZ));
    p3 = interRenderPoint(cam, rotate_Point_around_Point_wo_change(cam.location, triangle.p3, cam.rotX, cam.rotY, cam.rotZ));*/

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
