//
// Created by domins on 06.06.19.
//

#ifndef SUMO_PR_MESSAGES_H
#define SUMO_PR_MESSAGES_H

#include <SFML/Config.hpp>
#define MAX_CLIENTS 4

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

#endif //SUMO_PR_MESSAGES_H
