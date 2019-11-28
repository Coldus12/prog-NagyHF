//
// Created by coldus on 11/26/19.
//

//Using SDL and standard IO

#ifndef game
#define game

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

void startGame(SDL_Renderer *renderer, SDL_Window *window, int SCREEN_WIDTH, int SCREEN_HEIGHT, char* path_to_mod_ist, char* path_to_map) {
    //                                          Map és társai
    //------------------------------------------------------------------------------------------------------------------
    model_list *mod_list;
    map *mapy = NULL;
    Point location = {300,100,300};
    invis_wall* belso = load_invis_wall_from_file("korpalya_belso_invis.txt", location, 10);
    invis_wall* kulso = load_invis_wall_from_file("korpalya_kulso_invis.txt",location,30);
    //mod_list = load_model_list("/home/coldus/Desktop/models.txt");
    //mapy = load_map_from_file("/home/coldus/Desktop/map.txt", mapy, *mod_list);
    mod_list = load_model_list(path_to_mod_ist);
    mapy = load_map_from_file(path_to_map, mapy, *mod_list);

    //                                          Camera inicializálása
    //------------------------------------------------------------------------------------------------------------------
    Camera cam;

    cam.location.posX = 330;
    cam.location.posY = 100;
    cam.location.posZ = 80;
    cam.distanceFromPlane = 700;
    cam.planeSizeX = SCREEN_WIDTH;
    cam.planeSizeY = SCREEN_HEIGHT;
    //cam.viewDistance = 1000;
    //Így nincs renderDistance gyakorlatilag
    cam.viewDistance = -1;
    //                                          Objektumok és Modelek
    //------------------------------------------------------------------------------------------------------------------
    /*Model cube;
    load_model_from_file("/home/coldus/Desktop/hegy2.txt", &cube);
    //Point point;

    Object cube1, cube2, cube3, cube4;
    cube1.location.posX = 300;
    cube1.location.posY = 100;
    cube1.location.posZ = 300;
    load_Model_into_Object(&cube1, cube,10);*/

    //                             Objektum, és a kamera kezdeti értékeinek beállítása
    //------------------------------------------------------------------------------------------------------------------
    int x = 0,y = 0,z = 0;
    int a = 0, b = 0, c = 0;

    double degree = (M_PI/180.0);
    Point point = {300, 100, 300};
    Point p2 = {300+60, 100 ,300}, p3 = {300+60, 100, 300+60}, p4 = {300, 100, 300+60};
    cam.rotX = 0*degree;
    cam.rotY = 0*degree;
    cam.rotZ = 0*degree;

    //move_Object_to_Point(&cube1,point);

    //                                                  RenderList
    //------------------------------------------------------------------------------------------------------------------
    RList *head = NULL;

    //head = addObjectToRenderList(head, cam, &cube1);
    head = addMapToRenderList(head,cam,mapy);

    double rot = 0;

    double secsPerFrame = 1.0/50.0;
    bool keep_running = true;
    while (keep_running) {
        double most = time(NULL);
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
                    case SDLK_ESCAPE:
                        keep_running = false;
                        break;
                }
            }
        }

        /*if (rot < 360)
            rot += 0.0001;
        else
            rot = 0;*/

        //Kamera mozgatása a térben
        if (point_inside_invis_walls(belso, cam.location) || point_outside_invis_walls(kulso,cam.location)) {
            cam.location.posX -= x;
            cam.location.posY -= y;
            cam.location.posZ -= z;
            //printf("bünn!!\n");
        } else {
            cam.location.posX += x;
            cam.location.posY += y;
            cam.location.posZ += z;
            //printf("benn\n");
        }

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

        usleep(secsPerFrame*1000000);
    }

    print_invis(belso);
    //free_model(&cube);
    //free_object(&cube1);

    SDL_DestroyWindow(window);

    free_invis_wall(belso);
    free_invis_wall(kulso);
    free_model_list(mod_list);
    free_object_list(mapy);

    freeList(head);
}

#endif