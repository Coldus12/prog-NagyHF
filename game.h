//
// Created by coldus on 11/26/19.
//

#ifndef NAGYHF_GAME_H
#define NAGYHF_GAME_H
#include <SDL.h>
#include <stdio.h>
#include <stdbool.h>
#include <SDL2_gfxPrimitives.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include "object.h"
#include "renderer.h"

//Debugmalloc:
#include "debugmalloc-impl.h"
#include "debugmalloc.h"

void startGame(SDL_Renderer *renderer, SDL_Window *window, int SCREEN_WIDTH, int SCREEN_HEIGHT, char* path_to_mod_ist, char* path_to_map);

#endif //NAGYHF_GAME_H
