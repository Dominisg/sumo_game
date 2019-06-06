//
// Created by dominis on 05.06.19.
//

#include <iostream>
#include "Server.h"

sf::Packet& operator <<(sf::Packet& packet,  Sumo& sumo)
{
    return packet << sumo.velocity_x << sumo.velocity_y << sumo.angle;
}

sf::Packet& operator >>(sf::Packet& packet, Sumo& sumo)
{
    return packet >> sumo.velocity_x >> sumo.velocity_y >> sumo.angle;
}

Server::Server() {
    if (socket.bind(2137) != sf::Socket::Done)
    {
        // error...
    }
    selector.add(socket);
}

void Server::receive() {
    sf::Packet packet;
    sf::IpAddress sender;
    unsigned short port;
    Sumo player = {};


    if (selector.wait(sf::seconds(2.0f)))
    {
        if (socket.receive(packet, sender, port) != sf::Socket::Done)
        {
            std::cout << "Coś jebło\n";
        }
        else {
            packet >> player;
            std::cout << "Received x=" << player.velocity_x << "y=" << player.velocity_y << "angle=" << player.angle
                      << "from" << sender << " on port " << port << std::endl;
        }
    }
    else
    {
        std::cout << "Nic nie przyszło!\n";
        // timeout reached, nothing was received...
    }
}

sf::Socket::Status receiveWithTimeout(sf::TcpSocket& socket, sf::Packet& packet, sf::Time timeout)
{
    sf::SocketSelector selector;
    selector.add(socket);
    if (selector.wait(timeout))
        return socket.receive(packet);
    else
        return sf::Socket::NotReady;
};