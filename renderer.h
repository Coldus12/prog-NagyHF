//
// Created by coldus on 11/5/19.
//

#ifndef NAGYHF_RENDERER_H
#define NAGYHF_RENDERER_H

#include <SDL_render.h>
#include <SDL2_gfxPrimitives.h>

#include "object.h"

typedef struct Camera{Point location; int viewDistance; int planeSizeX; int planeSizeY; int distanceFromPlane; double rotX; double rotY; double rotZ} Camera;
Point interRenderPoint(Camera cam, Point p);
void renderTriangle(triangle triangle, Camera cam, SDL_Renderer *SDL_renderer);
void renderObject(Object object, Camera cam, SDL_Renderer *SDL_renderer);


#endif //NAGYHF_RENDERER_H
