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

int_list* add_to_list(int_list* head, int new);
void free_int_list(int_list *head);
void print_list(int_list* head);
int_list* remove_last_item(int_list* head);
int merge_int_list(int_list* head);

void load_resolutions(int *width, int *height);
void save_new_resolution(int new_width, int new_height);

void drawString(char *string, int x, int y, int red, int green, int blue, int fontSize, SDL_Renderer *renderer);
void drawMenu(MenuProperties mp, int sel);
void draw_settings_menu(MenuProperties mp, int sel);
void draw_display_settings(MenuProperties mp, int sel);
void draw_edit(MenuProperties mp, int_list* head, bool width);
void edit_display_settings(MenuProperties mp);
void display_settings(MenuProperties mp, int sel);
void settings_menu(MenuProperties menuProperties, int sel);
void init_main_menu(SDL_Window *window, SDL_Renderer *renderer, int SCREEN_WIDTH, int SCREEN_HEIGHT);



#endif //NAGYHF_MENU_H
