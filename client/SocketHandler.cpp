//
// Created by domins on 06.06.19.
//

#include <iostream>
#include "SocketHandler.h"

sf::Packet& operator >>(sf::Packet& packet, Sumo& sumo)
{
    float x,y;
    sf::Int16 angle;
    bool didMove;

    packet >> angle >> x >> y >>didMove;
    sumo.setAngle(angle);
    sumo.getSprite().setPosition(x,y);
    sumo.getContour().setPosition(x,y+25);
    sumo.setSumoDidMove(didMove);
    sumo.disable(false);
    return packet;
}

SocketHandler::SocketHandler(sf::IpAddress host,unsigned short port) {
    this->host = host;
    this->port = port;
    socket.setBlocking(false);
    slot = 0xFFFF;

}

void SocketHandler::sendInput(Player_Input g_input){
    sf::Packet packet;
    if (slot != (sf::Int16)0xFFFF)
    {
        packet << (sf::Uint8)Client_Message::Input;

        packet << slot;

        sf::Uint8 input = 	(sf::Uint8)g_input.up |
                         ((sf::Uint8)g_input.down << 1) |
                         ((sf::Uint8)g_input.left << 2) |
                         ((sf::Uint8)g_input.right << 3);

       packet << input;
        if(socket.send(packet, host, port) != sf::Socket::Done)
        {
            std::cout<<("socket_send failed\n");
        }
    }


}

bool SocketHandler::join(){
    sf::Packet packet;
    sf::Uint8 message = -1;
    sf::Int8 result;
    sf::Int16 slot;

    if(this->slot != (sf::Int16)0xFFFF)
        return true;

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

void SocketHandler::sendStart() {
    sf::Packet packet;
    std::cout<<"START"<<std::endl;
    packet << (sf::Uint8) Client_Message::Start;
    packet << slot;
    if (socket.send(packet, host, port) != sf::Socket::Done)
        std::cout<<"Error while starting."<<std::endl;
}

void SocketHandler::sendReady() {
    sf::Packet packet;
    packet << (sf::Uint8) Client_Message::Ready;
    packet << slot;
    if (socket.send(packet, host, port) != sf::Socket::Done)
        std::cout<<"Error while saying that i;m ready."<<std::endl;
}

void SocketHandler::sendLeave() {
    sf::Packet packet;
    packet << (sf::Uint8) Client_Message::Leave;
    packet << slot;
    if (socket.send(packet, host, port) != sf::Socket::Done)
        std::cout<<"Error while saying that i;m ready."<<std::endl;
}

int SocketHandler::receive(Sumo **players) {
    sf::Packet packet;
    sf::Uint8 message = -1;
    sf::Uint8 idx;
    sf::SocketSelector selector;

    selector.add(socket);
    if (selector.wait(sf::microseconds(100)))
    {
        socket.receive(packet,host,port);
        packet>>message;
        switch ((Server_Message)message)
        {
            case Server_Message::State:
            {
                for (sf::Uint8 i=0;i<MAX_CLIENTS;i++) {
                    players[i]->disable(true);
                }
                while(!packet.endOfPacket()) {
                    packet >> idx;
                    packet >> *players[idx];
                }
            }
            break;
            case  Server_Message::Out:{
                packet >> idx;
                players[idx]->inGame(false);
                if (idx == slot){
                    return (int)Server_Message::Out;
                }
            }
            break;
            default:
                break;

        }
    }
}

//zrwaca Server_Message::Inits, gdy należy zacząć rozgrywkę.
int SocketHandler::listenLobby() {
    sf::Packet packet;
    sf::Uint8 message = -1;
    sf::SocketSelector selector;

    selector.add(socket);
    if (selector.wait(sf::microseconds(100)))
    {
        socket.receive(packet,host,port);
        packet>>message;
        switch ((Server_Message)message)
        {
            case Server_Message::Init:
            {
                return (int)Server_Message::Init;
            }
            default:
                break;
        }
    }
    return 1;
}

sf::Int16 SocketHandler::getPlayerId() {
    return slot;
}