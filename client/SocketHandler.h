//
// Created by domins on 06.06.19.
//

#ifndef SUMO_PR_SOCKETHANDLER_H
#define SUMO_PR_SOCKETHANDLER_H
#include <SFML/Network/UdpSocket.hpp>
#include "Sumo.h"
#include "../common/messages.h"


class SocketHandler {
    sf::UdpSocket socket;
    sf::IpAddress host;
    unsigned short port;
    sf::Int16 slot;

public:
    SocketHandler(sf::IpAddress host,unsigned short port);
    void sendInput(Player_Input g_input);
    void recieve(Sumo** players);
    bool join();
    sf::Int16 getPlayerId();

};


#endif //SUMO_PR_SOCKETHANDLER_H
