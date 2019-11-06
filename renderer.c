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

    /*lineRGBA(SDL_renderer,p1.posX,p1.posY,p2.posX,p2.posY,0,0,0,255);
    lineRGBA(SDL_renderer,p2.posX,p2.posY,p3.posX,p3.posY,0,0,0,255);
    lineRGBA(SDL_renderer,p3.posX,p3.posY,p1.posX,p1.posY,0,0,0,255);*/
    //filledTrigonRGBA(SDL_renderer, p1.posX, p1.posY, p2.posX, p2.posY, p3.posX, p3.posY, 0, 0, 0, 255);
    trigonRGBA(SDL_renderer, p1.posX, p1.posY, p2.posX, p2.posY, p3.posX, p3.posY, 0, 0, 0, 255);
}

void renderObject(Object object, Camera cam, SDL_Renderer *SDL_renderer) {
    /*for(int i = 0; i < object.triangleArray.size; i++) {
        renderTriangle(object.triangleArray.triangles[i], cam, SDL_renderer);
        //printf("teszt: %d\n", object.triangleArray.triangles[i].p1.posZ);
        a++;
    }*/
    int a = object.model.triangleArray.size;
    for (int i = 0; i < a; i++) {
        //printf("%d ",i);
        renderTriangle(object.model.triangleArray.triangles[i], cam, SDL_renderer);
    }
    //printf("\n");
    //printf("es megint lefut\n");
    //printf("%d\n", object.triangleArray.size);
    //printf("%d\n",a);
}


#endif
