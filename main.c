//Using SDL and standard IO

#include <SDL.h>
#include <stdio.h>
#include <stdbool.h>
#include <SDL2_gfxPrimitives.h>
#include <math.h>
#include <stdlib.h>
#include "object.h"
#include "renderer.h"

//Debugmalloc:
#include "debugmalloc-impl.h"
#include "debugmalloc.h"

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 960;

int main( int argc, char* args[] ) {

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

    //                                          Map és társai
    //------------------------------------------------------------------------------------------------------------------
    model_list *mod_list;
    map *mapy = NULL;
    //mod_list = load_model_list("/home/coldus/Desktop/models.txt");
    //mapy = load_map_from_file("/home/coldus/Desktop/map.txt", mapy, *mod_list);
    mod_list = load_model_list("models.txt");
    mapy = load_map_from_file("map.txt", mapy, *mod_list);

    //                                          Camera inicializálása
    //------------------------------------------------------------------------------------------------------------------
    Camera cam;

    cam.location.posX = 330;
    cam.location.posY = 100;
    cam.location.posZ = 000;
    cam.distanceFromPlane = 700;
    cam.planeSizeX = SCREEN_WIDTH;
    cam.planeSizeY = SCREEN_HEIGHT;
    cam.viewDistance = 1000;

    //                                          Objektumok és Modelek
    //------------------------------------------------------------------------------------------------------------------
    /*Model cube;
    load_model_from_file("/home/coldus/Desktop/fa2.txt", &cube);
    //Point point;

    Object cube1, cube2, cube3, cube4;
    cube1.location.posX = 300;
    cube1.location.posY = 100;
    cube1.location.posZ = 300;*/

    /*cube2.location.posX = 300;
    cube2.location.posY = 100;
    cube2.location.posZ = 300;

    cube3.location.posX = 300;
    cube3.location.posY = 100;
    cube3.location.posZ = 300;

    cube4.location.posX = 300;
    cube4.location.posY = 100;
    cube4.location.posZ = 300;*/

    //load_Model_into_Object(&cube1, cube);
    /*load_Model_into_Object(&cube2, cube);
    load_Model_into_Object(&cube3, cube);
    load_Model_into_Object(&cube4, cube);*/

    //                             Objektum, és a kamera kezdeti értékeinek beállítása
    //------------------------------------------------------------------------------------------------------------------
    int x = 0,y = 0,z = 0;
    int a = 0, b = 0, c = 0;

    double degree = (M_PI/180);
    Point point = {300, 100, 300};
    Point p2 = {300+60, 100 ,300}, p3 = {300+60, 100, 300+60}, p4 = {300, 100, 300+60};
    cam.rotX = 0*degree;
    cam.rotY = 0*degree;
    cam.rotZ = 0*degree;

    //move_Object_to_Point(&cube1,point);
    /*move_Object_to_Point(&cube2, p2);
    move_Object_to_Point(&cube3, p3);
    move_Object_to_Point(&cube4, p4);*/

    //                                                  RenderList
    //------------------------------------------------------------------------------------------------------------------
    RList *head = NULL;

    map *current = mapy;
    while (current != NULL) {
        printf("location: x: %.0lf y: %.0lf z: %.0lf\n",current->obj.location.posX,current->obj.location.posY, current->obj.location.posZ);
        current = current->next;
    }

    //head = addObjectToRenderList(head, cam, &cube1);
    head = addMapToRenderList(head,cam,mapy);
    /*head = addObjectToRenderList(head, cam, &cube2);
    head = addObjectToRenderList(head, cam, &cube3);
    head = addObjectToRenderList(head, cam, &cube4);*/

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    double rot = 0;

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
                        y+=10;
                        break;
                    case SDLK_n:
                        y-=10;
                        break;
                    case SDLK_u:
                        a++;
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

        /*if (rot < 360)
            rot += 0.0001;
        else
            rot = 0;*/

        //Kamera mozgatása a térben
        cam.location.posX += x;
        cam.location.posY += y;
        cam.location.posZ += z;

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);

        //rotate_Object_around_Point(cube1.location,&cube1, 0, rot*degree, 0);

        //Kamera forgatása
        cam.rotX = a*10*degree;
        cam.rotY = b*10*degree;
        cam.rotZ = c*10*degree;

        head = render_RList(head, cam, renderer);

        SDL_RenderPresent(renderer);

        /*Az x, y, z értékeket 0-ba állítom, mert, ha nem állítanám,
         * akkor egy gombnyomás után folyamatosan növelnék
         * a koordináták értékét.
         */
        x = 0;
        y = 0;
        z = 0;

    }

    SDL_DestroyWindow(window);

    free_model_list(mod_list);
    free_object_list(mapy);

    freeList(head);

    //free_object(&cube1);
    /*free_object(&cube2);
    free_object(&cube3);
    free_object(&cube4);*/
    //free_model(&cube);

    SDL_Quit();

    return 0;
}