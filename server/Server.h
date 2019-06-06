//
// Created by dominis on 05.06.19.
//

#ifndef SUMO_PR_SERVER_H
#define SUMO_PR_SERVER_H
#include <SFML/Network.hpp>


class Server {
    sf::UdpSocket socket;
    sf::SocketSelector selector;
public:
    Server();
    void receive();
    sf::Socket::Status receiveWithTimeout(sf::TcpSocket& socket, sf::Packet& packet, sf::Time timeout);

};

typedef struct{
    float velocity_x,velocity_y;
    sf::Int16 angle;
}Sumo;


#endif //SUMO_PR_SERVER_H
