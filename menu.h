//
// Created by coldus on 11/26/19.
//

#ifndef NAGYHF_MENU_H
#define NAGYHF_MENU_H

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <SDL.h>
#include <SDL2_gfxPrimitives.h>

typedef struct MenuProperties{SDL_Window *window; SDL_Renderer *renderer; int SCREEN_WIDTH; int SCREEN_HEIGHT} MenuProperties;
typedef struct int_list{int integer; struct int_list *next} int_list;
typedef struct controls{int forward; int backward; int left; int right} Controls;

int_list* add_to_list(int_list* head, int new);
void free_int_list(int_list *head);
void print_list(int_list* head);
int_list* remove_last_item(int_list* head);
int merge_int_list(int_list* head);

void load_resolutions(int *width, int *height);
void save_new_resolution(int new_width, int new_height);

void load_controls(Controls* controls);
void save_controls(Controls controls);

void drawString(char *string, int x, int y, int red, int green, int blue, int fontSize, SDL_Renderer *renderer);
void init_main_menu(SDL_Window *window, SDL_Renderer *renderer, int SCREEN_WIDTH, int SCREEN_HEIGHT);



#endif //NAGYHF_MENU_H
