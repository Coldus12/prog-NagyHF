//
// Created by coldus on 10/30/19.
//


#ifndef NAGYHF_ADDITIONAL_H
#define NAGYHF_ADDITIONAL_H

#include <SDL.h>
#include <stdio.h>
#include <stdbool.h>
#include <SDL2_gfxPrimitives.h>
#include <math.h>
#include <stdlib.h>
#include "additional.h"

typedef struct Point{int posX; int posY; int posZ} Point;

typedef struct Camera{Point location; int viewDistance; int planeSizeX; int planeSizeY; int distanceFromPlane} Camera;

void renderpoint(Camera camera, SDL_Renderer *renderer, Point point);

void renderRect(Camera cam, SDL_Renderer* renderer, Point p);

void renderCube(Camera cam, SDL_Renderer* renderer, Point p);

Point interRenderPoint(Camera cam, Point p);

#endif //NAGYHF_ADDITIONAL_H
