//Using SDL and standard IO

#include <SDL.h>
#include <stdio.h>
#include <stdbool.h>
#include <SDL2_gfxPrimitives.h>
#include <math.h>
#include <stdlib.h>
#include "additional.h"
#include "object.h"
#include "renderer.h"


//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

enum KeyPressSurfaces
{
    KEY_PRESS_SURFACE_DEFAULT,
    KEY_PRESS_SURFACE_UP,
    KEY_PRESS_SURFACE_DOWN,
    KEY_PRESS_SURFACE_LEFT,
    KEY_PRESS_SURFACE_RIGHT,
    KEY_PRESS_SURFACE_TOTAL
};

int main( int argc, char* args[] ) {
    //The window we'll be rendering to
    SDL_Window* window = NULL;

    //The surface contained by the window
    SDL_Surface* screenSurface = NULL;

    //Initialize SDL
    if( SDL_Init( SDL_INIT_VIDEO ) < 0 ) {
        printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
    } else {
        //Create window
        window = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
        if( window == NULL ) {
            printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
        } else {
            //Get window surface
            screenSurface = SDL_GetWindowSurface( window );

            //Fill the surface white
            SDL_FillRect( screenSurface, NULL, SDL_MapRGB( screenSurface->format, 0xFF, 0xFF, 0xFF ) );

            //Update the surface
            SDL_UpdateWindowSurface( window );
        }
    }

    Camera cam;

    cam.location.posX = 200;
    cam.location.posY = 300;
    cam.location.posZ = 0;
    cam.distanceFromPlane = 500;
    cam.planeSizeX = SCREEN_WIDTH;
    cam.planeSizeY = SCREEN_HEIGHT;

    //Objektum innen
    //------------------------------------------------------------------------------------------------------------------
    //Object cube;
    //load_object_from_file("/home/coldus/Desktop/cube.txt", &cube);
    Point point;
    int x = 300,y = 100,z = 300;

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    for(bool keep_running = true; keep_running;) {
        for(SDL_Event ev; SDL_PollEvent(&ev);) {
            if(ev.type == SDL_QUIT) {
                keep_running = false;
            } else if (ev.type == SDL_KEYDOWN) {
                switch (ev.key.keysym.sym) {
                    case SDLK_UP:
                        //printf("shit");
                        z+=10;
                        break;
                    case SDLK_DOWN:
                        z-=10;
                        break;
                    case SDLK_LEFT:
                        x-=10;
                        break;
                    case SDLK_RIGHT:
                        x+=10;
                        break;
                    case SDLK_m:
                        //printf("HEEY!\n");
                        y+=10;
                        break;
                    case SDLK_n:
                        //printf("HOE!\n");
                        y-=10;
                        break;
                    case SDLK_p:
                        cam.distanceFromPlane++;
                        printf("%d\n", cam.distanceFromPlane);
                        break;
                    case SDLK_o:
                        cam.distanceFromPlane--;
                        printf("%d\n", cam.distanceFromPlane);
                        break;
                }
            }
        }

        point.posX = x;
        point.posY = y;
        point.posZ = z;

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);

        triangle tri;
        tri.p1 = point;

        tri.p2.posX = point.posX +100;
        tri.p2.posY = point.posY;
        tri.p2.posZ = point.posZ +50;

        tri.p3.posX = point.posX +100;
        tri.p3.posY = point.posY - 50;
        tri.p3.posZ = point.posZ;

        renderTriangle(tri, cam, renderer);
        //renderObject(cube, cam, renderer);
        //renderRect(cam, renderer, point);
        //renderCube(cam, renderer, point);
        SDL_RenderPresent(renderer);
    }

    //Destroy window
    SDL_DestroyWindow( window );

    //Quit SDL subsystems
    SDL_Quit();

    return 0;
}