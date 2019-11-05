//
// Created by coldus on 10/30/19.
//
#ifndef lofasz
#define lofasz

#include <SDL_render.h>
#include <SDL2_gfxPrimitives.h>

typedef struct Point{int posX; int posY; int posZ} Point;

typedef struct Camera{Point location; int viewDistance; int planeSizeX; int planeSizeY; int distanceFromPlane} Camera;

void renderpoint(Camera cam, SDL_Renderer* renderer, Point point) {
    Point vetulet;
    //vetulet.posZ = cam.location.posZ + cam.distanceFromPlane;

    int relativY = round((double) cam.distanceFromPlane*(((double) point.posY - cam.location.posY)/( (double) point.posZ - cam.location.posZ)));
    int relativX = round((double) cam.distanceFromPlane*(((double) point.posX - cam.location.posX)/( (double) point.posZ - cam.location.posZ)));

    //Szamolas
    vetulet.posY = (int) round(((double) cam.planeSizeY/2) - relativY);
    vetulet.posX = (int) round(((double) cam.planeSizeX/2) + relativX);

    //printf("posX = %d, posY = %d\n",vetulet.posX, vetulet.posY);

    circleRGBA(renderer, vetulet.posX, vetulet.posY, 10, 0, 0, 0, 255);
}

Point interRenderPoint(Camera cam, Point p) {
    Point vetulet;
    //vetulet.posZ = cam.location.posZ + cam.distanceFromPlane;
    vetulet.posZ = 0;

    int relativY = round((double) cam.distanceFromPlane*(((double) p.posY - cam.location.posY)/( (double) p.posZ - cam.location.posZ)));
    int relativX = round((double) cam.distanceFromPlane*(((double) p.posX - cam.location.posX)/( (double) p.posZ - cam.location.posZ)));

    //Szamolas
    vetulet.posY = (int) round(((double) cam.planeSizeY/2) - relativY);
    vetulet.posX = (int) round(((double) cam.planeSizeX/2) + relativX);

    return vetulet;

}

void renderRect(Camera cam, SDL_Renderer* renderer, Point p) {
    Point p2, p3, p4;

    p2.posX = p.posX+20;
    p2.posY = p.posY;
    p2.posZ = p.posZ;

    p3.posX = p.posX;
    p3.posY = p.posY+20;
    p3.posZ = p.posZ;

    p4.posX = p.posX+20;
    p4.posY = p.posY+20;
    p4.posZ = p.posZ;

    Point v, v2, v3, v4;

    v = interRenderPoint(cam, p);
    v2 = interRenderPoint(cam, p2);
    v3 = interRenderPoint(cam, p3);
    v4 = interRenderPoint(cam, p4);

    /*circleRGBA(renderer, v.posX, v.posY, 5, 0, 0, 0, 255);
    circleRGBA(renderer, v2.posX, v2.posY, 5, 0, 0, 0, 255);
    circleRGBA(renderer, v3.posX, v3.posY, 5, 0, 0, 0, 255);
    circleRGBA(renderer, v4.posX, v4.posY, 5, 0, 0, 0, 255);*/

    lineRGBA(renderer,v.posX, v.posY, v2.posX, v2.posY, 0, 0, 0, 255);
    lineRGBA(renderer,v2.posX, v2.posY, v4.posX, v4.posY, 0, 0, 0, 255);
    lineRGBA(renderer,v4.posX, v4.posY, v3.posX, v3.posY, 0, 0, 0, 255);
    lineRGBA(renderer,v3.posX, v3.posY, v.posX, v.posY, 0, 0, 0, 255);
}

void renderCube(Camera cam, SDL_Renderer* renderer, Point p) {
    Point p2, p3, p4, p5, p6 ,p7, p8;
    Point v, v2, v3, v4, v5, v6, v7, v8;

    p2.posX = p.posX+20;
    p2.posY = p.posY;
    p2.posZ = p.posZ;

    p3.posX = p.posX;
    p3.posY = p.posY+20;
    p3.posZ = p.posZ;

    p4.posX = p.posX+20;
    p4.posY = p.posY+20;
    p4.posZ = p.posZ;

    p5.posX = p.posX;
    p5.posY = p.posY;
    p5.posZ = p.posZ+20;

    p6.posX = p.posX+20;
    p6.posY = p.posY;
    p6.posZ = p.posZ+20;

    p7.posX = p.posX;
    p7.posY = p.posY+20;
    p7.posZ = p.posZ+20;

    p8.posX = p.posX+20;
    p8.posY = p.posY+20;
    p8.posZ = p.posZ+20;

    v = interRenderPoint(cam, p);
    v2 = interRenderPoint(cam, p2);
    v3 = interRenderPoint(cam, p3);
    v4 = interRenderPoint(cam, p4);
    v5 = interRenderPoint(cam, p5);
    v6 = interRenderPoint(cam, p6);
    v7 = interRenderPoint(cam, p7);
    v8 = interRenderPoint(cam, p8);

    lineRGBA(renderer,v.posX, v.posY, v2.posX, v2.posY, 0, 0, 0, 255);
    lineRGBA(renderer,v2.posX, v2.posY, v4.posX, v4.posY, 0, 0, 0, 255);
    lineRGBA(renderer,v4.posX, v4.posY, v3.posX, v3.posY, 0, 0, 0, 255);
    lineRGBA(renderer,v3.posX, v3.posY, v.posX, v.posY, 0, 0, 0, 255);

    lineRGBA(renderer,v5.posX, v5.posY, v6.posX, v6.posY, 0, 0, 0, 255);
    lineRGBA(renderer,v6.posX, v6.posY, v8.posX, v8.posY, 0, 0, 0, 255);
    lineRGBA(renderer,v8.posX, v8.posY, v7.posX, v7.posY, 0, 0, 0, 255);
    lineRGBA(renderer,v7.posX, v7.posY, v5.posX, v5.posY, 0, 0, 0, 255);

    lineRGBA(renderer,v.posX, v.posY, v5.posX, v5.posY, 0, 0, 0, 255);
    lineRGBA(renderer,v2.posX, v2.posY, v6.posX, v6.posY, 0, 0, 0, 255);
    lineRGBA(renderer,v3.posX, v3.posY, v7.posX, v7.posY, 0, 0, 0, 255);
    lineRGBA(renderer,v4.posX, v4.posY, v8.posX, v8.posY, 0, 0, 0, 255);
}

#endif
