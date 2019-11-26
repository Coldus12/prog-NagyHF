//Using SDL and standard IO

#include <SDL.h>
#include <stdio.h>
#include "game.h"
#include "menu.h"

//Debugmalloc:
#include "debugmalloc-impl.h"
#include "debugmalloc.h"

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 960;

int main( int argc, char* args[] ) {
    enum {
        start,
        settings,
        exit_window
    };

    //                                           SDL ablak létrehozása
    //------------------------------------------------------------------------------------------------------------------
    SDL_Window* window = NULL;
    SDL_Surface* screenSurface = NULL;
    if( SDL_Init( SDL_INIT_VIDEO ) < 0 ) {
        printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
    } else {
        window = SDL_CreateWindow( "3D graphics engine test", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
        if( window == NULL ) {
            printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
        } else {
            screenSurface = SDL_GetWindowSurface( window );
            SDL_FillRect( screenSurface, NULL, SDL_MapRGB( screenSurface->format, 0, 0, 0 ) );
            SDL_UpdateWindowSurface( window );
        }
    }
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

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
                            sel = 3;
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
                        switch (sel%3) {
                            case start:
                                startGame(renderer, window, SCREEN_WIDTH, SCREEN_HEIGHT);
                                keep_running = false;
                                break;
                            case settings:
                                break;
                            case exit_window:
                                break;
                            default:
                                sel = 3;
                                break;
                        }
                        keep_running = false;
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


    SDL_Quit();

    return 0;
}