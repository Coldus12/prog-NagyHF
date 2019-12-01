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
#include "menu.h"
#include "player.h"
#include "records.h"

typedef struct rect{Point p1; Point p2; Point p3; Point p4} Rect;

/*! A CheckWinCondition függvény tulajdonképpen csak azt vizsgálja meg, hogy egy
 * pont benne van-e egy másik négy pont által meghatározott téglalapban.
 * Hátránya az az, hogy semmi mást nem vizsgál, így azt sem tudja/tudhatja,
 * hogy a játékos jó irányból lépett-e a "célnégyszögbe".
 * */
bool checkWinCondition(Point p, Rect rect) {
    if ((rect.p1.posX < p.posX) && (p.posX < rect.p2.posX) && (rect.p1.posZ < p.posZ) && (p.posZ < rect.p3.posZ)) {
        return true;
    } else
        return false;
}

/*! A játék főfüggvénye.
 * Ez a függvény az, ami az egész játékot kezeli. A függvény úgy működik, hogy
 * miután elindították a játékot, ez betölti a megfelelő pályát, és a hozzá tartozó
 * láthatatlan falakat.
 * Majd figyeli a megfelelő gombnyomásokat, melyeket a beállítás.txt nevű fájlból olvas ki.
 * Ezek után kirajzoltatja a háromszögeket a megfelelő sorrendben.
 * */
void startGame(SDL_Renderer *renderer, SDL_Window *window, int SCREEN_WIDTH, int SCREEN_HEIGHT, char* path_to_mod_ist, char* path_to_map) {
    //                                          Irányítás betöltése
    //------------------------------------------------------------------------------------------------------------------
    Controls controls;
    load_controls(&controls);

    //                                              Játékos
    //------------------------------------------------------------------------------------------------------------------
    Point play_location = {680,100,470};
    Player player = initPlayer(play_location,650,"auto.txt",SCREEN_WIDTH, SCREEN_HEIGHT, 0);

    //                                          Map és társai
    //------------------------------------------------------------------------------------------------------------------
    model_list *mod_list;
    map *mapy = NULL;
    Point location = {300,100,300};

    invis_wall* belso = load_invis_wall_from_file("korpalya_belso_invis.txt", location, 20);
    invis_wall* kulso = load_invis_wall_from_file("korpalya_kulso_invis.txt",location,20);

    mod_list = load_model_list(path_to_mod_ist);
    mapy = load_map_from_file(path_to_map, mapy, *mod_list);
    double degree = (M_PI/180.0);

    //                                                  RenderList
    //------------------------------------------------------------------------------------------------------------------
    RList *head = NULL;

    head = addObjectToRenderList(head, player.thirdPersonView,&player.playerObject);
    head = addMapToRenderList(head,player.thirdPersonView,mapy);

    Point last_position;
    double secsPerFrame = 1.0/50.0;
    double time;
    bool keep_running = true;
    bool is_game_won = false;

    clock_t start = clock();
    while (keep_running) {
        clock_t start = clock();
        for(SDL_Event ev; SDL_PollEvent(&ev);) {
            if(ev.type == SDL_QUIT) {
                keep_running = false;
            } else if (ev.type == SDL_KEYDOWN) {
                int keycode = ev.key.keysym.sym;
                if (keycode == SDLK_ESCAPE) {
                    keep_running = false;
                } else if (keycode == controls.forward) {
                    if (player.velocity <= 10)
                        player.velocity += 0.25;
                } else if (keycode == controls.backward) {
                    if (player.velocity > 0)
                        player.velocity -= 0.25;
                } else if (keycode == controls.left) {
                    player.direction += 5*degree;
                } else if (keycode == controls.right) {
                    player.direction += -5*degree;
                }
            }
        }

        Point car;
        car.posX = player.location.posX;
        car.posY = player.location.posY;
        car.posZ = player.location.posZ;

        car.posY += 10;

        rotate_Point_around_Point(player.location,&car,0,player.direction,0);

        if (point_inside_invis_walls(belso, player.location) || point_outside_invis_walls(kulso,player.location) || point_inside_invis_walls(belso, car) || point_outside_invis_walls(kulso, car)) {
            player.velocity = 0;
            player.location = last_position;
        } else {
            last_position.posX = player.location.posX;
            last_position.posY = player.location.posY;
            last_position.posZ = player.location.posZ;
        }

        updatePlayer(&player);

        Rect r = {692, 100, 298, 740, 100,298,692, 100, 300, 740, 100, 300};
        if (checkWinCondition(player.location, r)){
            keep_running = false;
            is_game_won = true;
        }

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);
        head = render_RList(head, player.thirdPersonView, renderer);
        SDL_RenderPresent(renderer);
        SDL_Delay(secsPerFrame);
    }

    clock_t stop = clock();
    if (is_game_won) {
        time = (((double) stop-start)/CLOCKS_PER_SEC);
        printf("Time: %.3lf\n",(((double) stop-start)/CLOCKS_PER_SEC));
        save_time(time,"map2.txt");
    }


    free_invis_wall(belso);
    free_invis_wall(kulso);
    free_model_list(mod_list);
    free_object_list(mapy);

    free_player(&player);

    freeList(head);
}

#endif