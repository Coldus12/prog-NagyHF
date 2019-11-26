//
// Created by coldus on 11/26/19.
//

#ifndef NAGYHF_MENU_H
#define NAGYHF_MENU_H

#include <stdlib.h>
#include <stdio.h>
#include <SDL.h>
#include <SDL2_gfxPrimitives.h>

void drawString(char *string, int x, int y, int red, int green, int blue, int fontSize, SDL_Renderer *renderer);
void drawMenu(SDL_Renderer *renderer, int width, int height, short sel);
void init_menu(SDL_Window *window, SDL_Renderer *renderer, int SCREEN_WIDTH, int SCREEN_HEIGHT);

#endif //NAGYHF_MENU_H
