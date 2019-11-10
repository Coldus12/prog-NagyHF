//
// Created by coldus on 11/5/19.
//
#ifndef renderer
#define renderer

#include "object.h"
#include "additional.h"

void renderTriangle(triangle triangle, Camera cam, SDL_Renderer *SDL_renderer) {
    Point p1, p2, p3;
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
