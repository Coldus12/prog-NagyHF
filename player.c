//
// Created by coldus on 11/28/19.
//

#ifndef player
#define player

#include "renderer.h"

//A pont és a vektor jelen esetbenkét egymással felcserélhető fogalom, a program pontnak hívja a játékos
//irányvektorát is, pedig valójában az vektor, és akként és van kezelve.
typedef struct Player{Camera firstPersonView; Camera thirdPersonView; Object playerObject; double velocity; double direction; Point location; double third_person_dist} Player;

Player initPlayer(Point playerLocation, double thirdPersonView_dist, char* path_to_player_model, int screen_width, int screen_height, double direction) {
    double degree = M_PI/180.0;
    Player new_player;
    new_player.direction = direction;
    new_player.velocity = 0;
    new_player.third_person_dist = thirdPersonView_dist;

    Model player_model;
    load_model_from_file(path_to_player_model, &player_model);

    Object player_obj;
    player_obj.location = playerLocation;
    load_Model_into_Object(&player_obj, player_model, 5);
    //rotate_Object_around_Point(player_obj.location, &player_obj, 0, 180*degree, 0);

    player_obj.angle_from_y_axis = 180*degree;

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
    new_player.thirdPersonView.location.posY += thirdPersonView_dist;
    new_player.thirdPersonView.viewDistance = -1;
    new_player.thirdPersonView.rotZ = 0;
    new_player.thirdPersonView.rotY = 0;
    new_player.thirdPersonView.rotX = -90*degree;
    new_player.thirdPersonView.distanceFromPlane = 1000;
    new_player.thirdPersonView.planeSizeX = screen_width;
    new_player.thirdPersonView.planeSizeY = screen_height;

    new_player.location = playerLocation;

    free_model(&player_model);
    return new_player;
}

void updatePlayer(Player *play) {
    //A velocity-t egy (0,1)-es vektorként tekintve
    //play->location.posX += play->velocity*
    play->location.posX += play->velocity*(-sin(play->direction));
    play->location.posZ += play->velocity*(cos(play->direction));

    play->thirdPersonView.location = play->location;
    play->thirdPersonView.location.posY += play->third_person_dist;

    play->firstPersonView.location = play->location;
    move_Object_to_Point(&play->playerObject,play->location);

    if (play->direction != play->playerObject.angle_from_y_axis) {
        double dAngle = play->playerObject.angle_from_y_axis - play->direction;
        rotate_Object_around_Point(play->playerObject.location, &play->playerObject, 0, dAngle, 0);
        play->playerObject.angle_from_y_axis = play->direction;
    }
    //rotate_Object_around_Point(play->playerObject.location, &play->playerObject,0,play->direction*(M_PI/180.0),0);
}

void free_player(Player *player1) {
    free_object(&player1->playerObject);
}

#endif