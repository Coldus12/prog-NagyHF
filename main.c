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

/*TODO
 * Nincs peldanyositva a "model", igy annak a pontjait
 * irja at az object minden egyes valtoztatasnal
 * ez az azert problema, mert igy ha meg egy object.-et
 * letrehozok ugyanazon modellel, akkor nem lesznek masok a
 * az objectpontjainak koordinatai
 * */

#define M_PI acos(-1.0)

//Nem null pointert free-elek!
//Ez a double free
//Thas a problem mate

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
    Model cube;
    load_model_from_file("/home/coldus/Desktop/cube.txt", &cube);
    Point point;

    Object cube1, cube2, cube3, cube4;
    cube1.location.posX = 300;
    cube1.location.posY = 100;
    cube1.location.posZ = 300;

    cube2.location.posX = 300;
    cube2.location.posY = 100;
    cube2.location.posZ = 300;

    cube3.location.posX = 300;
    cube3.location.posY = 100;
    cube3.location.posZ = 300;

    cube4.location.posX = 300;
    cube4.location.posY = 100;
    cube4.location.posZ = 300;

    load_Model_into_Object(&cube1, cube);
    load_Model_into_Object(&cube2, cube);
    load_Model_into_Object(&cube3, cube);
    load_Model_into_Object(&cube4, cube);

    //                              Distance test
    //------------------------------------------------------------------------------------------------------------------

    /*Point a = {0, 40, 30};
    Point b = {0, 0, 0};
    printf("tavolsag a-b: %d\n", dist_btw_Points(a, b));*/

    //------------------------------------------------------------------------------------------------------------------
    int x = 300,y = 100,z = 300;
    int a = 0, b = 0, c = 0;

    double degree = (M_PI/180);
    //Point point;

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
                    case SDLK_u:
                        a++;
                        printf("a: %d\n", a);
                        break;
                    case SDLK_i:
                        a--;
                        break;
                    case SDLK_z:
                        b++;
                        break;
                    case SDLK_t:
                        b--;
                        break;
                    case SDLK_r:
                        c++;
                        break;
                    case SDLK_e:
                        c--;
                        break;
                }
            }
        }

        point.posX = x;
        point.posY = y;
        point.posZ = z;

        //rotate_Point_around_Point(cam.location, &point,  a*degree, 0*degree, 0*degree);

        Point p2 = { x+60, y, z};
        Point p3 = {x + 60, y, z+60};
        Point p4 = {x, y, z+60};

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);

        //rotate_Point_around_Point(point, &p2, 0*degree, 0*degree, a*degree);
        rotate_Object_around_Point(cube2.location, &cube2, 10*a*degree,10*b*degree,10*c*degree);
        a = 0; b = 0; c = 0;

        /*renderpoint(cam,renderer, point);
        renderpoint(cam,renderer, p2);
        renderpoint(cam,renderer, p3);
        renderpoint(cam,renderer, p4);*/

        move_Object_to_Point(&cube1,point);
        move_Object_to_Point(&cube2, p2);
        move_Object_to_Point(&cube3, p3);
        move_Object_to_Point(&cube4, p4);


        renderObject(cube1, cam, renderer);
        renderObject(cube2, cam, renderer);
        renderObject(cube3, cam, renderer);
        renderObject(cube4, cam, renderer);

        SDL_RenderPresent(renderer);
    }

    //Destroy window
    SDL_DestroyWindow( window );

    //free(cube1.model.triangleArray.triangles);

    free_object(&cube1);
    free_object(&cube2);
    free_object(&cube3);
    free_object(&cube4);
    free_model(&cube);
    printf("BYE\n");
    //Quit SDL subsystems
    SDL_Quit();

    return 0;
}