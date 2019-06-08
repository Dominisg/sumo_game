//
// Created by domins on 06.06.19.
//

#ifndef SUMO_PR_MESSAGES_H
#define SUMO_PR_MESSAGES_H

#include <SFML/Config.hpp>
#define MAX_CLIENTS 2

enum SCREENSIZE{
    X = 1280,
    Y = 800
};


typedef struct {
    float x;
    float y;
    sf::Int16 angle;
}Position;

constexpr float POSITION_OFFSET_X = SCREENSIZE ::X /2 - 0.25* 300;
constexpr float POSITION_OFFSET_Y = SCREENSIZE ::Y /2 - 0.25* 300;


constexpr Position DEFAULT_POSITIONS[MAX_CLIENTS]={{POSITION_OFFSET_X  + 180.0f,POSITION_OFFSET_Y - 180.0f,230},
                                                   {POSITION_OFFSET_X + 40.0f, POSITION_OFFSET_Y - 100.0f,60}};
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
