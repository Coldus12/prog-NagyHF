//
// Created by coldus on 11/28/19.
//

#ifndef NAGYHF_PLAYER_H
#define NAGYHF_PLAYER_H

#include "renderer.h"

typedef struct Player{Camera firstPersonView; Camera thirdPersonView; Object playerObject; double speed; double direction} Player;
Player initPlayer(Point playerLocation, double thirdPersonView_dist, char* path_to_player_model, int screen_width, int screen_height, double direction);
void free_player(Player *player1);

#endif //NAGYHF_PLAYER_H
