//
// Created by coldus on 11/26/19.
//

#ifndef menu
#define menu

#include <stdlib.h>
#include <stdio.h>
#include <SDL.h>
#include <SDL2_gfxPrimitives.h>
#include <SDL_ttf.h>

void drawString(char *string, int x, int y, int red, int green, int blue, int fontSize, SDL_Renderer *renderer) {
    TTF_Init();
    TTF_Font *font = TTF_OpenFont("/home/coldus/Desktop/Roboto-Regular.ttf", fontSize);
    SDL_Surface *text;
    SDL_Texture *text_t;
    SDL_Color color = {red,green,blue};
    SDL_Rect text_rect = {x, y, 0, 0};
    text = TTF_RenderUTF8_Solid(font, string, color);
    text_t = SDL_CreateTextureFromSurface(renderer, text);
    text_rect.w = text->w;
    text_rect.h = text->h;
    text_rect.x -= text->w/2;
    text_rect.y -= text->h/2;
    SDL_RenderCopy(renderer, text_t, NULL, &text_rect);
}

void drawMenu(SDL_Renderer *renderer, int width, int height, short sel) {
    enum {
        start,
        settings,
        exit_window
    };

    SDL_RenderClear(renderer);

    switch(sel%3) {
        case start:
            drawString("Start",width/2,height/2-60,0,255,0,70,renderer);
            drawString("Settings",width/2,height/2,255,0,0,70,renderer);
            drawString("Exit",width/2,height/2+60,255,0,0,70,renderer);
            break;
        case settings:
            drawString("Start",width/2,height/2-60,255,0,0,70,renderer);
            drawString("Settings",width/2,height/2,0,255,0,70,renderer);
            drawString("Exit",width/2,height/2+60,255,0,0,70,renderer);
            break;
        case exit_window:
            drawString("Start",width/2,height/2-60,255,0,0,70,renderer);
            drawString("Settings",width/2,height/2,255,0,0,70,renderer);
            drawString("Exit",width/2,height/2+60,0,255,0,70,renderer);
            break;
        default:

            break;
    }

    //stringRGBA(renderer, 500, 500, "Nah", 255, 0, 0, 255);
    //SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);

}

#endif