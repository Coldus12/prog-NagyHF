//Using SDL and standard IO

#include <SDL.h>
#include <stdio.h>
#include "menu.h"
#include "player.h"

//Debugmalloc:
#include "debugmalloc-impl.h"
#include "debugmalloc.h"

/*TODO
 * Dicsőséglista
 *      kiírása
 *      mentése
 * Játék
 *      pálya kiválasztása
 *      kocsi modell
 *      belső nézet
 *      irány-vektor
 *      win-condition
 * */

int main( int argc, char* args[] ) {
    int SCREEN_WIDTH = 1280;
    int SCREEN_HEIGHT = 960;

    load_resolutions(&SCREEN_WIDTH, &SCREEN_HEIGHT);
    /*Point kezodhelyzet = {0, 0, 0};
    Player player = initPlayer(kezodhelyzet,300,"fa.txt",SCREEN_WIDTH,SCREEN_HEIGHT, 0);
    //printf("player lofasz: %d\n",player.thirdPersonView.planeSizeY);
    free_player(&player);*/

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

    init_main_menu(window, renderer, SCREEN_WIDTH, SCREEN_HEIGHT);

    SDL_Quit();

    return 0;
}