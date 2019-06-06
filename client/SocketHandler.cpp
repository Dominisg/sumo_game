//
// Created by domins on 06.06.19.
//

#include <iostream>
#include "SocketHandler.h"
#include "Sumo.h"


sf::Packet& operator <<(sf::Packet& packet,  Sumo& sumo)
{
    return packet << sumo.getVelocity().x << sumo.getVelocity().y << sumo.getAngle();
}

sf::Packet& operator >>(sf::Packet& packet, Sumo& sumo)
{
    float x,y;
    sf::Int16 angle;

    packet >> x >> y >> angle;
    sumo.setAngle(angle);
    sumo.setVelocity(sf::Vector2f(x,y));
    return packet;
}

SocketHandler::SocketHandler(sf::IpAddress host,unsigned short port) {
    this->host = host;
    this->port = port;
}

void SocketHandler::send(Sumo& sumo){
    sf::Packet packet;

    packet << sumo;
    if (socket.send(packet, host, port) != sf::Socket::Done)
    {
        std::cout<<"nie wysłałem";
    }

}