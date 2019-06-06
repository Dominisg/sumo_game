//
// Created by domins on 06.06.19.
//

#ifndef SUMO_PR_MESSAGES_H
#define SUMO_PR_MESSAGES_H

#include <SFML/Config.hpp>
#define MAX_CLIENTS 4


constexpr float MAX_VELOCITY = 6.f;
constexpr float D_VELOCITY = 0.25f;
constexpr float FRICTION = 0.04f;
constexpr int ANGLE_ROTATION = 5;

enum class Client_Message : sf::Uint8
{
    Join,      // tell server we're new here
    Leave,      // tell server we're leaving
    Input,
    Start // tell server our user input
};

enum class Server_Message : sf::Uint8
{
    Join_Result,// tell client they're accepted/rejected
    State,// tell client game state
    Init, // tell client beginning settings
    End  // tell client that game has ended
};

struct Player_Input
{
    bool up, down, left, right;
};

#endif //SUMO_PR_MESSAGES_H
