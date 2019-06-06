//
// Created by domins on 06.06.19.
//

#ifndef SUMO_PR_SOCKETHANDLER_H
#define SUMO_PR_SOCKETHANDLER_H
#include <SFML/Network/UdpSocket.hpp>
#include "Sumo.h"


class SocketHandler {
    sf::UdpSocket socket;
    sf::IpAddress host;
    unsigned short port;
    sf::Int16 slot;

public:
    SocketHandler(sf::IpAddress host,unsigned short port);
    void send(Sumo& sumo);
    void recieve(Sumo** players);
    bool join();

};


#endif //SUMO_PR_SOCKETHANDLER_H
