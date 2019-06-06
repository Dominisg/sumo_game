//
// Created by dominis on 05.06.19.
//

#include <iostream>
#include "Server.h"
#include "../common/messages.h"

sf::Packet& operator <<(sf::Packet& packet,  Player_State& sumo)
{
    return packet << sumo.velocity_x << sumo.velocity_y << sumo.angle;
}

sf::Packet& operator >>(sf::Packet& packet, Player_State& sumo)
{
    return packet >> sumo.velocity_x >> sumo.velocity_y >> sumo.angle;
}

Server::Server() {
    if (socket.bind(2137) != sf::Socket::Done)
    {
        // error...
    }
    socket.setBlocking(false);
    selector.add(socket);
}

void Server::perform() {
    sf::Packet packet_received;
    sf::Packet packet_to_send;
    sf::IpAddress sender;
    unsigned short port;
    Player_State player = {};
    sf::Uint8 message;
    int result;

    bool is_running;

    while(true) {

        if ((result = socket.receive(packet_received, sender, port)) != sf::Socket::Done) {
            if (result == sf::Socket::NotReady){
                break;
            }
        } else {

            IP_Endpoint from_endpoint;
            packet_received >> message;
//
//            std::cout << "Received x=" << player.velocity_x << "y=" << player.velocity_y << "angle=" << player.angle
//                      << "from" << sender << " on port " << port << std::endl;

            switch ((Client_Message) message) {
                case Client_Message::Join: {
                    sf::Int16 slot = -1;
                    for (sf::Int16 i = 0; i < MAX_CLIENTS; ++i) {
                        if (client_endpoints[i].in_use == 0) {
                            slot = i;
                            break;
                        }
                    }

                    packet_to_send << (sf::Uint8) Server_Message::Join_Result;

                    if (slot != -1) {
                        packet_to_send << sf::Uint8(1);
                        packet_to_send << sf::Uint8(slot);

                        if (socket.send(packet_to_send, sender, port) == sf::Socket::Done) {
                            client_endpoints[slot].address = sender;
                            client_endpoints[slot].port = port;
                            time_since_heard_from_clients[slot].restart();
                            client_objects[slot] = {};
                            client_inputs[slot] = {};
                        }
                    } else {
                        packet_to_send << sf::Uint8(0);
                        socket.send(packet_to_send, sender, port);
                    }
                }
                    break;

                case Client_Message::Leave: {
                    sf::Int16 slot;
                    packet_received >> slot;

                    if (client_endpoints[slot].address == sender and client_endpoints[slot].port == port) {
                        client_endpoints[slot] = {};
                    }
                }
                    break;

                case Client_Message::Input: {
                    sf::Int16 slot;
                    packet_received >> slot;


                    if (client_endpoints[slot].address == sender and client_endpoints[slot].port == port) {
                        sf::Uint8 input;
                        packet_received >> input;

                        client_inputs[slot].up = input & 0x1;
                        client_inputs[slot].down = input & 0x2;
                        client_inputs[slot].left = input & 0x4;
                        client_inputs[slot].right = input & 0x8;

                        time_since_heard_from_clients[slot].restart();
                    }
                }
                    break;

                case Client_Message::Start: {
                    // ...
                }
                    break;
            }
            //Here state update



        }
    }
    sendBack();
}

void Server::sendBack(){
    sf::Packet packet_to_send;
    packet_to_send << (sf::Int8)Server_Message::State;

    for( sf::Uint16 i = 0; i < MAX_CLIENTS; ++i )
    {
        if( client_endpoints[i].in_use )
        {
            packet_to_send<<client_objects[i];
        }
    }

    for( sf::Uint16 i = 0; i < MAX_CLIENTS; ++i )
    {
        if( client_endpoints[i].in_use )
        {
            socket.send(packet_to_send, client_endpoints[i].address, client_endpoints[i].port);
        }
    }
}