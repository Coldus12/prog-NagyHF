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

typedef struct MenuProperties{SDL_Window *window; SDL_Renderer *renderer; int SCREEN_WIDTH; int SCREEN_HEIGHT} MenuProperties;
typedef struct int_list{int integer; struct int_list *next} int_list;
typedef struct controls{int forward; int backward; int left; int right} Controls;

//                                              int_list
//----------------------------------------------------------------------------------------------------------------------

int_list* add_to_list(int_list* head, int new) {
    int_list *newItem = malloc(sizeof(int_list));

    newItem->integer = new;
    newItem->next = NULL;

    if (head==NULL)
        return newItem;
    int_list *iter = head;
    while (iter->next !=NULL) {
        iter = iter->next;
    }
    iter->next = newItem;
    return head;
}

void free_int_list(int_list *head) {
    int_list *iter = head;
    while (iter != NULL) {
        int_list* temp = iter->next;
        free(iter);
        iter = temp;
    }

}

void print_list(int_list* head) {
    int_list* iter = head;
    while(iter != NULL) {
        printf("%d ", iter->integer);
        iter = iter->next;
    }
    printf("\n");
}

int_list* remove_last_item(int_list* head) {
    int_list *prev = NULL;
    int_list *iter = head;

    while(iter->next->next != NULL) {
        iter = iter->next;
    }

    free(iter->next);
    iter->next = NULL;
    return head;
}

int merge_int_list(int_list* head) {
    char egybe[100] = {0};
    int_list* iter = head;
    while (iter != NULL) {
        char new[10];
        sprintf(new, "%d", iter->integer);
        strcat(egybe, new);
        iter = iter->next;
    }

    return (int) strtol(egybe, NULL, 10);
}

//                             Dicsőséglista beolvasása, kiírása, mentése
//----------------------------------------------------------------------------------------------------------------------

int_list* read_reacords_from_file(char* path, char* name_of_the_map) {
    FILE *fp = fopen(path, "r");

    char line[100];
    while (fgets(line, sizeof(line),fp) != NULL) {

    }

    fclose(fp);
}

//                                  Felbontás betöltése és mentése
//----------------------------------------------------------------------------------------------------------------------

void load_resolutions(int *width, int *height) {
    FILE *fp = fopen("beallitasok.txt","r");

    char lines[8][100];
    char line[100];

    int i = 0;
    while(fgets(line,sizeof(line), fp) != NULL) {
        strcpy(lines[i],line);
        //MI?!?
        //printf("%s",line);
        //printf("%s",lines[i]);
        i++;
    }

    for (int e = 0; e < 8; e++) {
        //printf("hey\n");
        if (strstr(lines[e], "Width") != NULL) {
            char tmp[100];
            int end = 2;
            strncpy(tmp, lines[e]+7, sizeof(lines[e])-7);
            for (int j = 0; j < (int) sizeof(tmp); j ++) {
                //Az idézőjel ASCII kódja 34
                if (tmp[j] == 34)
                    end = j-1;
            }
            strncpy(tmp, tmp, end);
            *width = (int) strtol(tmp, NULL, 10);
            printf("Width = %d\n", *width);

        } else if (strstr(lines[e], "Height") != NULL) {
            char tmp[100];
            int end = 2;
            strncpy(tmp, lines[e]+8, sizeof(lines[e])-8);
            for (int j = 0; j < (int) sizeof(tmp); j ++) {
                //Az idézőjel ASCII kódja 34
                if (tmp[j] == 34)
                    end = j-1;
            }
            strncpy(tmp, tmp,  end);
            *height = (int) strtol(tmp, NULL, 10);
            printf("Height = %d\n", *height);
        } else {
            //printf("%s",lines[e]);
        }
    }

    fclose(fp);
}

void save_new_resolution(int new_width, int new_height) {
    FILE *fp = fopen("beallitasok.txt","r");
    FILE *temp = fopen("beallitasok.tmp","w");

    //lines original_lines[10];
    char lines[8][100];
    char line[100];
    int i = 0;

    //A beallitasok fajl csak 8 soros, szoval elvileg, hacsak nem irtak bele kezzel olyan dolgokat
    //amiknek nem ott van a helyuk, akkor a 10 sor-nak elegnek kellene lennie.
    while(fgets(line,sizeof(line), fp) != NULL) {
        strcpy(lines[i],line);
        //printf("%s\n", line);
        //strcpy(lines[i], line);
        i++;
    }

    for (int e = 0; e < 8; e++) {
        if (strstr(lines[e], "Width") != NULL) {
            char new_line[100];
            sprintf(new_line, "Width=\"%d\"\n",new_width);
            strcpy(lines[e], new_line);
        } else if (strstr(lines[e], "Height") != NULL) {
            char new_line[100];
            sprintf(new_line, "Height=\"%d\"\n",new_height);
            strcpy(lines[e], new_line);
        }
        fprintf(temp, "%s", lines[e]);
    }

    fclose(temp);
    fclose(fp);

    remove("beallitasok.txt");
    rename("beallitasok.tmp", "beallitasok.txt");
    int a, b;
    //load_resolutions(&a,&b);
}

//                                  Irányítás betöltése és mentése
//----------------------------------------------------------------------------------------------------------------------

void save_controls(Controls controls) {
    FILE *fp = fopen("beallitasok.txt","r");
    FILE *temp = fopen("beallitasok.tmp","w");

    //lines original_lines[10];
    char lines[8][100];
    char line[100];
    int i = 0;

    //A beallitasok fajl csak 8 soros, szoval elvileg, hacsak nem irtak bele kezzel olyan dolgokat
    //amiknek nem ott van a helyuk, akkor a 10 sor-nak elegnek kellene lennie.
    while(fgets(line,sizeof(line), fp) != NULL) {
        strcpy(lines[i],line);
        i++;
    }

    for (int e = 0; e < 8; e++) {
        if (strstr(lines[e], "moveForward") != NULL) {
            char new_line[100];
            sprintf(new_line, "moveForward=\"%d\"\n",controls.forward);
            strcpy(lines[e], new_line);
        } else if (strstr(lines[e], "moveBackward") != NULL) {
            char new_line[100];
            sprintf(new_line, "moveBackward=\"%d\"\n",controls.backward);
            strcpy(lines[e], new_line);
        } else if (strstr(lines[e], "moveLeft") != NULL) {
            char new_line[100];
            sprintf(new_line, "moveLeft=\"%d\"\n",controls.left);
            strcpy(lines[e], new_line);
        } else if (strstr(lines[e], "moveRight") != NULL) {
            char new_line[100];
            sprintf(new_line, "moveRight=\"%d\"\n",controls.right);
            strcpy(lines[e], new_line);
        }
        fprintf(temp, "%s", lines[e]);
    }

    fclose(temp);
    fclose(fp);

    remove("beallitasok.txt");
    rename("beallitasok.tmp", "beallitasok.txt");
}

void load_controls(Controls* controls) {
    FILE *fp = fopen("beallitasok.txt", "r");
    char line[100];
    while(fgets(line, sizeof(line),fp) != NULL) {
        char tmp[100];
        if (strstr(line, "moveForward")) {
            strncpy(tmp, line+13, sizeof(line)-13);
            int end = 2;
            for (int i = 0; i < (int) sizeof(tmp); i ++) {
                //Az idézőjel ASCII kódja 34
                if (tmp[i] == 34)
                    end = i-1;
            }
            strncpy(tmp,tmp,end);
            controls->forward = (int) strtol(tmp,NULL,10);
            //strcat(forward,SDL_GetKeyName(keycode));
        } else if (strstr(line, "moveBackward")) {
            strncpy(tmp, line+14, sizeof(line)-14);
            int end = 2;
            for (int i = 0; i < (int) sizeof(tmp); i ++) {
                //Az idézőjel ASCII kódja 34
                if (tmp[i] == 34)
                    end = i-1;
            }
            strncpy(tmp,tmp,end);
            controls->backward = (int) strtol(tmp,NULL,10);
            //strcat(backward,SDL_GetKeyName(keycode));
        } else if (strstr(line, "moveLeft")) {
            strncpy(tmp, line+10, sizeof(line)-10);
            int end = 2;
            for (int i = 0; i < (int) sizeof(tmp); i ++) {
                //Az idézőjel ASCII kódja 34
                if (tmp[i] == 34)
                    end = i-1;
            }
            strncpy(tmp,tmp,end);
            controls->left = (int) strtol(tmp,NULL,10);
            //strcat(left,SDL_GetKeyName(keycode));
        } else if (strstr(line, "moveRight")) {
            strncpy(tmp, line+11, sizeof(line)-11);
            int end = 2;
            for (int i = 0; i < (int) sizeof(tmp); i ++) {
                //Az idézőjel ASCII kódja 34
                if (tmp[i] == 34)
                    end = i-1;
            }
            strncpy(tmp,tmp,end);
            controls->right = (int) strtol(tmp,NULL,10);
            //strcat(right,SDL_GetKeyName(keycode));
        }
    }

    //printf("%s %s %s %s\n", forward, backward, left, right);
    fclose(fp);
}

//                                          Szöveg kiíró függvény
//----------------------------------------------------------------------------------------------------------------------

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

//                                        Menüket kirajzoló függvények
//----------------------------------------------------------------------------------------------------------------------

void drawMenu(MenuProperties mp, int sel) {
    SDL_GetWindowSize(mp.window, &mp.SCREEN_WIDTH, &mp.SCREEN_HEIGHT);

    enum {
        start,
        settings,
        records,
        exit_window
    };

    SDL_RenderClear(mp.renderer);
    SDL_SetRenderDrawColor(mp.renderer, 0, 0,0, 255);

    switch(sel%4) {
        case start:
            drawString("Start",mp.SCREEN_WIDTH/2,mp.SCREEN_HEIGHT/2-110,0,255,0,70,mp.renderer);
            drawString("Settings",mp.SCREEN_WIDTH/2,mp.SCREEN_HEIGHT/2-40,255,0,0,70,mp.renderer);
            drawString("Records",mp.SCREEN_WIDTH/2,mp.SCREEN_HEIGHT/2+40,255,0,0,70,mp.renderer);
            drawString("Exit",mp.SCREEN_WIDTH/2,mp.SCREEN_HEIGHT/2+110,255,0,0,70,mp.renderer);
            break;
        case settings:
            drawString("Start",mp.SCREEN_WIDTH/2,mp.SCREEN_HEIGHT/2-110,255,0,0,70,mp.renderer);
            drawString("Settings",mp.SCREEN_WIDTH/2,mp.SCREEN_HEIGHT/2-40,0,255,0,70,mp.renderer);
            drawString("Records",mp.SCREEN_WIDTH/2,mp.SCREEN_HEIGHT/2+40,255,0,0,70,mp.renderer);
            drawString("Exit",mp.SCREEN_WIDTH/2,mp.SCREEN_HEIGHT/2+110,255,0,0,70,mp.renderer);
            break;
        case records:
            drawString("Start",mp.SCREEN_WIDTH/2,mp.SCREEN_HEIGHT/2-110,255,0,0,70,mp.renderer);
            drawString("Settings",mp.SCREEN_WIDTH/2,mp.SCREEN_HEIGHT/2-40,255,0,0,70,mp.renderer);
            drawString("Records",mp.SCREEN_WIDTH/2,mp.SCREEN_HEIGHT/2+40,0,255,0,70,mp.renderer);
            drawString("Exit",mp.SCREEN_WIDTH/2,mp.SCREEN_HEIGHT/2+110,255,0,0,70,mp.renderer);
            break;
        case exit_window:
            drawString("Start",mp.SCREEN_WIDTH/2,mp.SCREEN_HEIGHT/2-110,255,0,0,70,mp.renderer);
            drawString("Settings",mp.SCREEN_WIDTH/2,mp.SCREEN_HEIGHT/2-40,255,0,0,70,mp.renderer);
            drawString("Records",mp.SCREEN_WIDTH/2,mp.SCREEN_HEIGHT/2+40,255,0,0,70,mp.renderer);
            drawString("Exit",mp.SCREEN_WIDTH/2,mp.SCREEN_HEIGHT/2+110,0,255,0,70,mp.renderer);
            break;
        default:

            break;
    }
    SDL_RenderPresent(mp.renderer);

}

void draw_settings_menu(MenuProperties mp, int sel) {
    SDL_GetWindowSize(mp.window, &mp.SCREEN_WIDTH, &mp.SCREEN_HEIGHT);

    enum {
        display_settings,
        controls,
        back
    };


    SDL_RenderClear(mp.renderer);
    SDL_SetRenderDrawColor(mp.renderer, 0, 0,0, 255);

    switch(sel%3) {
        case display_settings:
            drawString("Display",mp.SCREEN_WIDTH/2,mp.SCREEN_HEIGHT/2-110,0,255,0,70,mp.renderer);
            drawString("Controls",mp.SCREEN_WIDTH/2,mp.SCREEN_HEIGHT/2-40,255,0,0,70,mp.renderer);
            drawString("Back",mp.SCREEN_WIDTH/2,mp.SCREEN_HEIGHT/2+40,255,0,0,70,mp.renderer);
            break;
        case controls:
            drawString("Display",mp.SCREEN_WIDTH/2,mp.SCREEN_HEIGHT/2-110,255,0,0,70,mp.renderer);
            drawString("Controls",mp.SCREEN_WIDTH/2,mp.SCREEN_HEIGHT/2-40,0,255,0,70,mp.renderer);
            drawString("Back",mp.SCREEN_WIDTH/2,mp.SCREEN_HEIGHT/2+40,255,0,0,70,mp.renderer);
            break;
        case back:
            drawString("Display",mp.SCREEN_WIDTH/2,mp.SCREEN_HEIGHT/2-110,255,0,0,70,mp.renderer);
            drawString("Controls",mp.SCREEN_WIDTH/2,mp.SCREEN_HEIGHT/2-40,255,0,0,70,mp.renderer);
            drawString("Back",mp.SCREEN_WIDTH/2,mp.SCREEN_HEIGHT/2+40,0,255,0,70,mp.renderer);
            break;
        default:

            break;
    }
    SDL_RenderPresent(mp.renderer);
}

void draw_display_settings(MenuProperties mp, int sel) {
    SDL_GetWindowSize(mp.window, &mp.SCREEN_WIDTH, &mp.SCREEN_HEIGHT);

    enum {
        display,
        back,
        fullscreen_yes,
        fullscreen_no
    };

    SDL_RenderClear(mp.renderer);
    SDL_SetRenderDrawColor(mp.renderer,0,0,0,255);
    char width[50] = "Width: ";
    char height[50] = "Height: ";
    char w[32];
    char h[32];

    sprintf(w, "%d", mp.SCREEN_WIDTH);
    sprintf(h, "%d", mp.SCREEN_HEIGHT);
    strcat(width, w);
    strcat(height, h);

    drawString("Jelenlegi felbontásod:",mp.SCREEN_WIDTH/4, mp.SCREEN_HEIGHT/4, 200, 200, 200, 25, mp.renderer);
    drawString(width, 20*mp.SCREEN_WIDTH/64, 20*mp.SCREEN_HEIGHT/64, 100, 100, 100, 18, mp.renderer);
    drawString(height, 20*mp.SCREEN_WIDTH/64, 24*mp.SCREEN_HEIGHT/64, 100, 100, 100, 18, mp.renderer);
    drawString("Szeretnél rajta változtatni??", mp.SCREEN_WIDTH/4, mp.SCREEN_HEIGHT/2, 200, 200, 200, 25, mp.renderer);
    drawString("Legyen teljes képernyős?", mp.SCREEN_WIDTH/4, 20*mp.SCREEN_HEIGHT/32, 200, 200, 200, 25, mp.renderer);

    switch(sel) {
        case display:
            drawString("Igen",17*mp.SCREEN_WIDTH/64, 18*mp.SCREEN_HEIGHT/32, 100, 255, 100, 20, mp.renderer);
            drawString("Nem",24*mp.SCREEN_WIDTH/64, 18*mp.SCREEN_HEIGHT/32, 100, 100, 100, 20, mp.renderer);

            drawString("Igen",17*mp.SCREEN_WIDTH/64, 22*mp.SCREEN_HEIGHT/32, 100, 100, 100, 20, mp.renderer);
            drawString("Nem",24*mp.SCREEN_WIDTH/64, 22*mp.SCREEN_HEIGHT/32, 100, 100, 100, 20, mp.renderer);
            break;
        case back:
            drawString("Igen",17*mp.SCREEN_WIDTH/64, 18*mp.SCREEN_HEIGHT/32, 100, 100, 100, 20, mp.renderer);
            drawString("Nem",24*mp.SCREEN_WIDTH/64, 18*mp.SCREEN_HEIGHT/32, 100, 255, 100, 20, mp.renderer);

            drawString("Igen",17*mp.SCREEN_WIDTH/64, 22*mp.SCREEN_HEIGHT/32, 100, 100, 100, 20, mp.renderer);
            drawString("Nem",24*mp.SCREEN_WIDTH/64, 22*mp.SCREEN_HEIGHT/32, 100, 100, 100, 20, mp.renderer);
            break;
        case fullscreen_yes:
            drawString("Igen",17*mp.SCREEN_WIDTH/64, 18*mp.SCREEN_HEIGHT/32, 100, 100, 100, 20, mp.renderer);
            drawString("Nem",24*mp.SCREEN_WIDTH/64, 18*mp.SCREEN_HEIGHT/32, 100, 100, 100, 20, mp.renderer);

            drawString("Igen",17*mp.SCREEN_WIDTH/64, 22*mp.SCREEN_HEIGHT/32, 100, 255, 100, 20, mp.renderer);
            drawString("Nem",24*mp.SCREEN_WIDTH/64, 22*mp.SCREEN_HEIGHT/32, 100, 100, 100, 20, mp.renderer);
            break;
        case fullscreen_no:
            drawString("Igen",17*mp.SCREEN_WIDTH/64, 18*mp.SCREEN_HEIGHT/32, 100, 100, 100, 20, mp.renderer);
            drawString("Nem",24*mp.SCREEN_WIDTH/64, 18*mp.SCREEN_HEIGHT/32, 100, 100, 100, 20, mp.renderer);

            drawString("Igen",17*mp.SCREEN_WIDTH/64, 22*mp.SCREEN_HEIGHT/32, 100, 100, 100, 20, mp.renderer);
            drawString("Nem",24*mp.SCREEN_WIDTH/64, 22*mp.SCREEN_HEIGHT/32, 100, 255, 100, 20, mp.renderer);
            break;
        default:
            break;
    }
    SDL_RenderPresent(mp.renderer);
}

void draw_edit(MenuProperties mp, int_list* head, bool width) {
    SDL_GetWindowSize(mp.window, &mp.SCREEN_WIDTH, &mp.SCREEN_HEIGHT);

    SDL_RenderClear(mp.renderer);
    SDL_SetRenderDrawColor(mp.renderer, 0, 0, 0, 255);

    char current[40];
    if (width) {
        drawString("Az új width legyen:", mp.SCREEN_WIDTH/3, mp.SCREEN_HEIGHT/3, 100, 100, 100, 20, mp.renderer);
    } else {
        drawString("Az új height legyen:", mp.SCREEN_WIDTH/3, mp.SCREEN_HEIGHT/3, 100, 100, 100, 20, mp.renderer);
    }
    sprintf(current, "%d", merge_int_list(head));
    drawString(current, 3*mp.SCREEN_WIDTH/6, 3*mp.SCREEN_HEIGHT/6, 200, 200, 200, 30, mp.renderer);

    SDL_RenderPresent(mp.renderer);
}

void draw_control_settings(MenuProperties mp, int sel, Controls controls, bool setup) {
    enum {
        forward_s,
        backward_s,
        left_s,
        right_s,
        back
    };

    SDL_RenderClear(mp.renderer);
    SDL_SetRenderDrawColor(mp.renderer,0,0,0,255);

    char forward[100] = "Előre: ";
    char backward[100] = "Hátra: ";
    char left[100] = "Balra: ";
    char right[100] = "Jobbra: ";

    strcat(forward,SDL_GetKeyName(controls.forward));
    strcat(backward,SDL_GetKeyName(controls.backward));
    strcat(left,SDL_GetKeyName(controls.left));
    strcat(right,SDL_GetKeyName(controls.right));

    if (!setup) {
        drawString("Jelenlegi beállításaid:",mp.SCREEN_WIDTH/4, mp.SCREEN_HEIGHT/4, 200, 200, 200, 25, mp.renderer);
        drawString(forward, 20*mp.SCREEN_WIDTH/64, 20*mp.SCREEN_HEIGHT/64, 100, 100, 100, 18, mp.renderer);
        drawString(backward, 20*mp.SCREEN_WIDTH/64, 24*mp.SCREEN_HEIGHT/64, 100, 100, 100, 18, mp.renderer);
        drawString(left, 20*mp.SCREEN_WIDTH/64, 28*mp.SCREEN_HEIGHT/64, 100, 100, 100, 18, mp.renderer);
        drawString(right, 20*mp.SCREEN_WIDTH/64, 32*mp.SCREEN_HEIGHT/64, 100, 100, 100, 18, mp.renderer);
        drawString("Szeretnél rajtuk változtatni?", mp.SCREEN_WIDTH/4, 36*mp.SCREEN_HEIGHT/64, 200, 200, 200, 25, mp.renderer);

        switch (sel%2) {
            case 0:
                drawString("Igen",17*mp.SCREEN_WIDTH/64, 40*mp.SCREEN_HEIGHT/64, 100, 100, 100, 20, mp.renderer);
                drawString("Nem",24*mp.SCREEN_WIDTH/64, 40*mp.SCREEN_HEIGHT/64, 100, 255, 100, 20, mp.renderer);
                break;
            case 1:
                drawString("Igen",17*mp.SCREEN_WIDTH/64, 40*mp.SCREEN_HEIGHT/64, 100, 255, 100, 20, mp.renderer);
                drawString("Nem",24*mp.SCREEN_WIDTH/64, 40*mp.SCREEN_HEIGHT/64, 100, 100, 100, 20, mp.renderer);
                break;
        }
    } else {
        drawString("Jelenlegi beállításaid:",mp.SCREEN_WIDTH/4, mp.SCREEN_HEIGHT/4, 200, 200, 200, 25, mp.renderer);
        drawString(forward, 20*mp.SCREEN_WIDTH/64, 20*mp.SCREEN_HEIGHT/64, 100, 100, 100, 18, mp.renderer);
        drawString(backward, 20*mp.SCREEN_WIDTH/64, 24*mp.SCREEN_HEIGHT/64, 100, 100, 100, 18, mp.renderer);
        drawString(left, 20*mp.SCREEN_WIDTH/64, 28*mp.SCREEN_HEIGHT/64, 100, 100, 100, 18, mp.renderer);
        drawString(right, 20*mp.SCREEN_WIDTH/64, 32*mp.SCREEN_HEIGHT/64, 100, 100, 100, 18, mp.renderer);
        drawString("Vissza", mp.SCREEN_WIDTH/4, 36*mp.SCREEN_HEIGHT/64, 100, 100, 100, 25, mp.renderer);
        switch (sel%5) {
            case forward_s:
                drawString(forward, 20*mp.SCREEN_WIDTH/64, 20*mp.SCREEN_HEIGHT/64, 200, 200, 200, 18, mp.renderer);
                break;
            case backward_s:
                drawString(backward, 20*mp.SCREEN_WIDTH/64, 24*mp.SCREEN_HEIGHT/64, 200, 200, 200, 18, mp.renderer);
                break;
            case left_s:
                drawString(left, 20*mp.SCREEN_WIDTH/64, 28*mp.SCREEN_HEIGHT/64, 200, 200, 200, 18, mp.renderer);
                break;
            case right_s:
                drawString(right, 20*mp.SCREEN_WIDTH/64, 32*mp.SCREEN_HEIGHT/64, 200, 200, 200, 18, mp.renderer);
                break;
            case back:
                drawString("Vissza", mp.SCREEN_WIDTH/4, 36*mp.SCREEN_HEIGHT/64, 200, 200, 200, 25, mp.renderer);
                break;
        }
    }

    SDL_RenderPresent(mp.renderer);
}



//                                  Felbontás megváltoztását kezelő függvény
//----------------------------------------------------------------------------------------------------------------------

void edit_display_settings(MenuProperties mp) {
    SDL_GetWindowSize(mp.window, &mp.SCREEN_WIDTH, &mp.SCREEN_HEIGHT);

    SDL_Event event;
    bool keep_running = true;
    bool width = true;
    int_list *head = NULL;
    int new_width, new_height;

    while(keep_running) {
        draw_edit(mp, head, width);
        while(SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                keep_running = false;
            } else if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {

                    case SDLK_0:
                        head = add_to_list(head,0);
                        //print_list(head);
                        draw_edit(mp, head, width);
                        break;
                    case SDLK_1:
                        head = add_to_list(head,1);
                        draw_edit(mp, head, width);
                        break;
                    case SDLK_2:
                        head = add_to_list(head,2);
                        draw_edit(mp, head, width);
                        break;
                    case SDLK_3:
                        head = add_to_list(head,3);
                        draw_edit(mp, head, width);
                        break;
                    case SDLK_4:
                        head = add_to_list(head,4);
                        draw_edit(mp, head, width);
                        break;
                    case SDLK_5:
                        head = add_to_list(head,5);
                        draw_edit(mp, head, width);
                        break;
                    case SDLK_6:
                        head = add_to_list(head,6);
                        draw_edit(mp, head, width);
                        break;
                    case SDLK_7:
                        head = add_to_list(head,7);
                        draw_edit(mp, head, width);
                        break;
                    case SDLK_8:
                        head = add_to_list(head,8);
                        draw_edit(mp, head, width);
                        break;
                    case SDLK_9:
                        head = add_to_list(head,9);
                        draw_edit(mp, head, width);
                        break;
                    case SDLK_RETURN:
                        draw_edit(mp, head,width);
                        int temp = merge_int_list(head);
                        free_int_list(head);
                        head = NULL;
                        if (!width) {
                            keep_running = false;
                            new_height = temp;
                            //printf("%d\n", new_height);
                        } else {
                            width = false;
                            new_width = temp;
                            //printf("%d\n", new_width);
                        }
                        break;
                    case SDLK_BACKSPACE:
                        head = remove_last_item(head);
                        draw_edit(mp,head, width);
                        break;
                }
            }
        }
    }

    save_new_resolution(new_width, new_height);
    SDL_SetWindowSize(mp.window, new_width, new_height);
}

//                               Az irányítást meegváltoztató menü függvénye
//----------------------------------------------------------------------------------------------------------------------

void control_settings(MenuProperties mp, int sel) {
    enum {
        back,
        edit_controls
    };

    enum {
        forward_s,
        backward_s,
        left_s,
        right_s,
        back_s
    };
    Controls controls;
    load_controls(&controls);
    bool setup = false;

    SDL_Event event;
    bool keep_running = true;

    while (keep_running) {
        draw_control_settings(mp, sel, controls, setup);
        while(SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                keep_running = false;
            } else if (event.type == SDL_KEYDOWN) {
                //printf("%d ",event.key.keysym.sym);
                if (!setup) {
                    switch (event.key.keysym.sym) {
                        case SDLK_LEFT:
                            if (sel == back)
                                sel = edit_controls;
                            else
                                sel = back;
                            draw_control_settings(mp, sel, controls, setup);
                            break;
                        case SDLK_RIGHT:
                            if (sel == edit_controls)
                                sel = back;
                            else
                                sel = edit_controls;
                            draw_control_settings(mp, sel, controls, setup);
                            break;
                        case SDLK_RETURN:
                            if(sel == back)
                                keep_running = false;
                            else
                                sel = 0;
                                setup = true;
                                break;
                    }
                } else {
                    switch (event.key.keysym.sym) {
                        case SDLK_UP:
                            if (sel == forward_s)
                                sel = back_s;
                            else
                                sel--;
                            draw_control_settings(mp,sel,controls,setup);
                            break;
                        case SDLK_DOWN:
                            if (sel == back_s)
                                sel = forward_s;
                            else
                                sel++;
                            draw_control_settings(mp,sel,controls,setup);
                            break;
                        case SDLK_RETURN:
                            if (sel == back_s)
                                setup = false;
                            draw_control_settings(mp,sel,controls,setup);
                            save_controls(controls);
                            break;
                        default:
                            switch (sel%5) {
                                case forward_s:
                                    controls.forward = event.key.keysym.sym;
                                    break;
                                case backward_s:
                                    controls.backward = event.key.keysym.sym;
                                    break;
                                case left_s:
                                    controls.left = event.key.keysym.sym;
                                    break;
                                case right_s:
                                    controls.right = event.key.keysym.sym;
                                    break;
                                case back_s:
                                    break;
                            }
                            draw_control_settings(mp,sel,controls,setup);
                            break;
                    }
                }
            }
        }
    }
}

//                                   A megjelenítést kezelő menü függvénye
//----------------------------------------------------------------------------------------------------------------------

void display_settings(MenuProperties mp, int sel) {
    //SDL_GetWindowSize(mp.window, &mp.SCREEN_HEIGHT, &mp.SCREEN_WIDTH);

    enum {
        display,
        back,
        fullscreen_yes,
        fullscreen_no
    };

    SDL_Event event;
    bool keep_running = true;
    while (keep_running) {
        draw_display_settings(mp, sel);
        while(SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                keep_running = false;
            } else if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {

                    case SDLK_DOWN:
                    case SDLK_UP:
                        switch (sel % 4) {
                            case display:
                                sel = fullscreen_yes;
                                break;
                            case back:
                                sel = fullscreen_no;
                                break;
                            case fullscreen_yes:
                                sel = display;
                                break;
                            case fullscreen_no:
                                sel = back;
                                break;
                            default:
                                sel = 1;
                                break;
                        }
                        draw_display_settings(mp,sel);
                        break;

                    case SDLK_LEFT:
                    case SDLK_RIGHT:
                        switch (sel % 4) {
                            case display:
                                sel = back;
                                break;
                            case back:
                                sel = display;
                                break;
                            case fullscreen_yes:
                                sel = fullscreen_no;
                                break;
                            case fullscreen_no:
                                sel = fullscreen_yes;
                                break;
                            default:
                                sel = 1;
                                break;
                        }
                        draw_display_settings(mp, sel);
                        break;

                    case SDLK_RETURN:
                        switch (sel % 4) {
                            case display:
                                edit_display_settings(mp);
                                break;
                            case back:
                                keep_running = false;
                                break;
                            case fullscreen_yes:
                                SDL_SetWindowFullscreen(mp.window, true);
                                break;
                            case fullscreen_no:
                                SDL_SetWindowFullscreen(mp.window, false);
                                keep_running = false;
                                break;
                            default:
                                sel = 1;
                                break;
                        }
                        break;
                    default:
                        break;
                }
            }
        }
    }
}

//                                        A beállításokat kezelő almenü
//----------------------------------------------------------------------------------------------------------------------

void settings_menu(MenuProperties menuProperties, int sel) {
    //SDL_GetWindowSize(menuProperties.window, &menuProperties.SCREEN_HEIGHT, &menuProperties.SCREEN_WIDTH);
    enum {
        display,
        controls,
        back
    };

    SDL_Event event;
    bool keep_running = true;
    while (keep_running) {
        draw_settings_menu(menuProperties, sel);
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
                        draw_settings_menu(menuProperties, sel);
                        break;
                    case SDLK_DOWN:
                        sel++;
                        draw_settings_menu(menuProperties, sel);
                        break;
                    case SDLK_RETURN:
                        switch (sel % 3) {
                            case display:
                                display_settings(menuProperties, sel);
                                break;
                            case controls:
                                //sel = 1;
                                control_settings(menuProperties, sel);
                                //settings_menu(mp, sel);
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
                        break;
                }
            }
        }
    }
}

//                               Az főmenü függvénye
//----------------------------------------------------------------------------------------------------------------------

void init_main_menu(SDL_Window *window, SDL_Renderer *renderer, int SCREEN_WIDTH, int SCREEN_HEIGHT) {
    //SDL_GetWindowSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);
    enum {
        start,
        settings,
        records,
        exit_window
    };

    MenuProperties mp;
    mp.renderer = renderer;
    mp.window = window;
    mp.SCREEN_HEIGHT = SCREEN_HEIGHT;
    mp.SCREEN_WIDTH = SCREEN_WIDTH;

    int sel = 1;
    int currentmenu = 0;
    bool keep_running = true;
    SDL_Event event;
    while(keep_running) {
        drawMenu(mp, sel);

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
                        drawMenu(mp, sel);
                        break;
                    case SDLK_DOWN:
                        sel++;
                        drawMenu(mp, sel);
                        break;
                    case SDLK_RETURN:
                        switch (sel % 4) {
                            case start:
                                SCREEN_HEIGHT = mp.SCREEN_HEIGHT;
                                SCREEN_WIDTH = mp.SCREEN_WIDTH;
                                startGame(renderer, window, SCREEN_WIDTH, SCREEN_HEIGHT, "models2.txt", "map2.txt");
                                keep_running = false;
                                break;
                            case settings:
                                sel = 1;
                                settings_menu(mp, sel);
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
                        break;
                }
            }
        }
    }
}


#endif