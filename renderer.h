//
// Created by coldus on 11/5/19.
//

#ifndef NAGYHF_RENDERER_H
#define NAGYHF_RENDERER_H

#include "object.h"
#include "additional.h"

void renderTriangle(triangle triangle, Camera cam, SDL_Renderer *SDL_renderer);
void renderObject(Object object, Camera cam, SDL_Renderer *SDL_renderer);

#endif //NAGYHF_RENDERER_H
