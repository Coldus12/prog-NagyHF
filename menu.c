//
// Created by coldus on 11/26/19.
//

#ifndef menu
#define menu

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <SDL.h>
#include <SDL2_gfxPrimitives.h>
#include <SDL_ttf.h>

#include "game.h"

//Debugmalloc:
#include "debugmalloc-impl.h"
#include "debugmalloc.h"

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
    TTF_CloseFont(font);
    SDL_FreeSurface(text);
    SDL_DestroyTexture(text_t);
}

void drawMenu(SDL_Renderer *renderer, int width, int height, short sel) {
    enum {
        start,
        settings,
        records,
        exit_window
    };

    SDL_RenderClear(renderer);

    switch(sel%4) {
        case start:
            drawString("Start",width/2,height/2-110,0,255,0,70,renderer);
            drawString("Settings",width/2,height/2-40,255,0,0,70,renderer);
            drawString("Records",width/2,height/2+40,255,0,0,70,renderer);
            drawString("Exit",width/2,height/2+110,255,0,0,70,renderer);
            break;
        case settings:
            drawString("Start",width/2,height/2-110,255,0,0,70,renderer);
            drawString("Settings",width/2,height/2-40,0,255,0,70,renderer);
            drawString("Records",width/2,height/2+40,255,0,0,70,renderer);
            drawString("Exit",width/2,height/2+110,255,0,0,70,renderer);
            break;
        case records:
            drawString("Start",width/2,height/2-110,255,0,0,70,renderer);
            drawString("Settings",width/2,height/2-40,255,0,0,70,renderer);
            drawString("Records",width/2,height/2+40,0,255,0,70,renderer);
            drawString("Exit",width/2,height/2+110,255,0,0,70,renderer);
            break;
        case exit_window:
            drawString("Start",width/2,height/2-110,255,0,0,70,renderer);
            drawString("Settings",width/2,height/2-40,255,0,0,70,renderer);
            drawString("Records",width/2,height/2+40,255,0,0,70,renderer);
            drawString("Exit",width/2,height/2+110,0,255,0,70,renderer);
            break;
        default:

            break;
    }

    //stringRGBA(renderer, 500, 500, "Nah", 255, 0, 0, 255);
    //SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);

}

void draw_settings_menu(SDL_Renderer *renderer, int width, int height, short sel) {
    enum {
        display_settings,
        controlls,
        back
    };


    SDL_RenderClear(renderer);

    switch(sel%3) {
        case display_settings:
            drawString("Display",width/2,height/2-110,0,255,0,70,renderer);
            drawString("Controls",width/2,height/2-40,255,0,0,70,renderer);
            drawString("Back",width/2,height/2+40,255,0,0,70,renderer);
            break;
        case controlls:
            drawString("Display",width/2,height/2-110,255,0,0,70,renderer);
            drawString("Controls",width/2,height/2-40,0,255,0,70,renderer);
            drawString("Back",width/2,height/2+40,255,0,0,70,renderer);
            break;
        case back:
            drawString("Display",width/2,height/2-110,255,0,0,70,renderer);
            drawString("Controls",width/2,height/2-40,255,0,0,70,renderer);
            drawString("Back",width/2,height/2+40,0,255,0,70,renderer);
            break;
        default:

            break;
    }

    //stringRGBA(renderer, 500, 500, "Nah", 255, 0, 0, 255);
    //SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
}

void settings_menu(SDL_Window *window, SDL_Renderer *renderer, int SCREEN_WIDTH, int SCREEN_HEIGHT) {
    SDL_RenderClear(renderer);

    enum {
        display_settings,
        controlls,
        back
    };

    short sel = 1;
    bool keep_running = true;
    SDL_Event event;
    while(keep_running) {
        draw_settings_menu(renderer, SCREEN_WIDTH, SCREEN_HEIGHT, sel);
        //SDL_RenderClear(renderer);
        //SDL_SetRenderDrawColor(renderer, 0, 0, 0,255);

        while(SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                keep_running = false;
            } else if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                    case SDLK_UP:
                        if (sel == 1) {
                            sel = 3;
                        } else {
                            sel--;
                        }
                        draw_settings_menu(renderer, SCREEN_WIDTH, SCREEN_HEIGHT, sel);
                        break;
                    case SDLK_DOWN:
                        sel++;
                        draw_settings_menu(renderer, SCREEN_WIDTH, SCREEN_HEIGHT, sel);
                        break;
                    case SDLK_RETURN:
                        switch (sel%3) {
                            case display_settings:
                                break;
                            case controlls:
                                drawString("Your current controls are:", SCREEN_WIDTH/2+200, SCREEN_HEIGHT/2, 100, 100, 100, 20, renderer);

                                break;
                            case back:
                                keep_running = false;
                                break;
                            default:
                                sel = 3;
                                break;
                        }
                        break;
                    default:
                        printf("%d", SDL_KEYDOWN);
                        printf("whaaa\n");
                        break;
                }
            }
        }
        //SDL_RenderPresent(renderer);
    }

    SDL_RenderPresent(renderer);
}

void init_menu(SDL_Window *window, SDL_Renderer *renderer, int SCREEN_WIDTH, int SCREEN_HEIGHT) {
    enum {
        start,
        settings,
        records,
        exit_window
    };

    short sel = 1;
    bool keep_running = true;
    SDL_Event event;
    while(keep_running) {
        drawMenu(renderer, SCREEN_WIDTH, SCREEN_HEIGHT, sel);
        //SDL_RenderClear(renderer);
        //SDL_SetRenderDrawColor(renderer, 0, 0, 0,255);

        while(SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                keep_running = false;
            } else if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                    case SDLK_UP:
                        if (sel == 1) {
                            sel = 4;
                        } else {
                            sel--;
                        }
                        drawMenu(renderer, SCREEN_WIDTH, SCREEN_HEIGHT, sel);
                        break;
                    case SDLK_DOWN:
                        sel++;
                        drawMenu(renderer, SCREEN_WIDTH, SCREEN_HEIGHT, sel);
                        break;
                    case SDLK_RETURN:
                        switch (sel%4) {
                            case start:
                                startGame(renderer, window, SCREEN_WIDTH, SCREEN_HEIGHT);
                                keep_running = false;
                                break;
                            case settings:
                                settings_menu(window, renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
                                break;
                            case records:
                                break;
                            case exit_window:
                                keep_running = false;
                                break;
                            default:
                                sel = 4;
                                break;
                        }
                        break;
                    default:
                        printf("%d", SDL_KEYDOWN);
                        printf("whaaa\n");
                        break;
                }
            }
        }
        //SDL_RenderPresent(renderer);
    }
}

#endif