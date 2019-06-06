//
// Created by domins on 06.06.19.
//

#include <iostream>
#include "SocketHandler.h"
#include "Sumo.h"
#include "../common/messages.h"

sf::Packet& operator >>(sf::Packet& packet, Sumo& sumo)
{
    float vx,vy,x,y;
    sf::Int16 angle;

    packet >> vx >> vy >> angle>>x>>y;
    sumo.setAngle(angle);
    sumo.setVelocity(sf::Vector2f(vx,vy));
    sumo.getSprite().setPosition(x,y);
    sumo.getContour().setPosition(x,y+25);
    return packet;
}

SocketHandler::SocketHandler(sf::IpAddress host,unsigned short port) {
    this->host = host;
    this->port = port;
    socket.setBlocking(false);

}

void SocketHandler::send(Sumo& sumo){
//    sf::Packet packet;
//
//    packet << sumo;
//    if (socket.send(packet, host, port) != sf::Socket::Done)
//    {
//        std::cout<<"Err";
//    }

}

bool SocketHandler::join(){
    sf::Packet packet;
    sf::Uint8 message = -1;
    sf::Int8 result;
    sf::Int16 slot;

    packet << (sf::Uint8) Client_Message::Join;
    if (socket.send(packet, host, port) != sf::Socket::Done)
    {
        std::cout<<"Error while joining";
        return false;
    }

    while(message != (sf::Int8) Server_Message::Join_Result){
        sf::SocketSelector selector;
        selector.add(socket);
        if (selector.wait(sf::seconds(5.0f))) {
            socket.receive(packet, host, port);
            packet >> message;
            if(message == (sf::Uint8) Server_Message::Join_Result){
                packet >> result;
                if(result == 1){
                    packet >> slot;
                    this->slot = slot;
                    return true;
                }
                else{
                    return false;
                }
            }

        }
        else
            return false;
    }
return false;
}

void SocketHandler::recieve(Sumo **players) {
    sf::Packet packet;
    sf::Uint8 message = -1;
    sf::Uint16 idx;
    while (socket.receive(packet,host,port))
    {
        packet>>message;
        switch ((Server_Message)message)
        {
            case Server_Message::State:
            {
                while(!packet.endOfPacket()) {
                    packet >> idx;
                    packet >> *players[idx];
                }
            }
                break;
            default:
                break;
        }
    }
}