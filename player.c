//
// Created by coldus on 11/28/19.
//

#ifndef player
#define player

#include "renderer.h"

//A pont és a vektor jelen esetbenkét egymással felcserélhető fogalom, a program pontnak hívja a játékos
//irányvektorát is, pedig valójában az vektor, és akként és van kezelve.
typedef struct Player{Camera firstPersonView; Camera thirdPersonView; Object playerObject; double speed; double direction} Player;

Player initPlayer(Point playerLocation, double thirdPersonView_dist, char* path_to_player_model, int screen_width, int screen_height, double direction) {
    Player new_player;
    new_player.direction = direction;
    new_player.speed = 0;

    Model player_model;
    load_model_from_file(path_to_player_model, &player_model);

    Object player_obj;
    player_obj.location = playerLocation;
    load_Model_into_Object(&player_obj, player_model, 1);

    new_player.playerObject = player_obj;
    new_player.firstPersonView.location = playerLocation;
    new_player.firstPersonView.viewDistance = -1;
    new_player.firstPersonView.rotZ = 0;
    new_player.firstPersonView.rotY = 0;
    new_player.firstPersonView.rotX = 0;
    new_player.firstPersonView.distanceFromPlane = 1000;
    new_player.firstPersonView.planeSizeX = screen_width;
    new_player.firstPersonView.planeSizeY = screen_height;

    new_player.thirdPersonView.location = playerLocation;
    new_player.thirdPersonView.location.posZ += thirdPersonView_dist;
    new_player.thirdPersonView.viewDistance = -1;
    new_player.thirdPersonView.rotZ = 0;
    new_player.thirdPersonView.rotY = 0;
    new_player.thirdPersonView.rotX = 0;
    new_player.thirdPersonView.distanceFromPlane = 1000;
    new_player.thirdPersonView.planeSizeX = screen_width;
    new_player.thirdPersonView.planeSizeY = screen_height;

    free_model(&player_model);
    return new_player;
}

void free_player(Player *player1) {
    free_object(&player1->playerObject);
}

#endif