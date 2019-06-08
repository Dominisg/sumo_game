//
// Created by dominis on 05.06.19.
//

#include <iostream>
#include "Server.h"
#include "../common/messages.h"

#define _USE_MATH_DEFINES
#include <math.h>


sf::Packet& operator <<(sf::Packet& packet,  Player_State& sumo)
{
    return packet << sumo.angle << sumo.sprite.getPosition().x<<sumo.sprite.getPosition().y <<sumo.didMove;
}

Server::Server() {
    if (socket.bind(5600) != sf::Socket::Done)
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


    while(true) {

        if ((result = socket.receive(packet_received, sender, port)) != sf::Socket::Done) {
            if (result == sf::Socket::NotReady){
                break;
            }
        } else {
            packet_received >> message;

            switch ((Client_Message) message) {
                case Client_Message::Join: {
                    std::cout<<"Ktoś wchodzi!"<<std::endl;
                    sf::Int16 slot = -1;
                    for (sf::Int16 i = 0; i < MAX_CLIENTS; ++i) {
                        if (client_endpoints[i].in_use == 0) {
                            slot = i;
                            break;
                        }
                    }

                    packet_to_send << (sf::Uint8) Server_Message::Join_Result;

                    if (slot != -1) {
                        std::cout<<"No i wszedł!"<<std::endl;
                        packet_to_send << sf::Uint8(1);
                        packet_to_send << sf::Int16(slot);

                        if (socket.send(packet_to_send, sender, port) == sf::Socket::Done) {
                            client_endpoints[slot].address = sender;
                            client_endpoints[slot].port = port;
                            client_endpoints[slot].in_use = true;
                            time_since_heard_from_clients[slot].restart();
                            client_objects[slot] = {};
                            client_objects[slot].sprite.setPosition(DEFAULT_POSITIONS[slot].x,DEFAULT_POSITIONS[slot].y);
                            client_objects[slot].angle = DEFAULT_POSITIONS[slot].angle;
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

                        //std::cout<< "Ruszonko "<<slot<<std::endl;
                        //if(client_inputs[slot].up) std::cout<<"up"<<std::endl;
                        //if(client_inputs[slot].down) std::cout<<"down"<<std::endl;
                        //if(client_inputs[slot].left) std::cout<<"left"<<std::endl;
                        //if(client_inputs[slot].right) std::cout<<"right"<<std::endl;

                        time_since_heard_from_clients[slot].restart();
                    }
                }
                    break;

                case Client_Message::Start: {
                    started = 1;
                }
                    break;
            }
            updateState();
        }
    }
}

void Server::sendBack(){
    sf::Packet packet_to_send;
    packet_to_send << (sf::Int8)Server_Message::State;

    for( sf::Uint8 i = 0; i < MAX_CLIENTS; ++i )
    {
        if( client_endpoints[i].in_use )
        {
            packet_to_send<<i;
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

void Server::updateState(){

    if (clock.getElapsedTime().asSeconds() > 0.022f){

    for( sf::Uint16 i = 0; i < MAX_CLIENTS; ++i ) {
        if (client_endpoints[i].in_use) {

            for (sf::Uint16 j=i; j< MAX_CLIENTS; ++j)
                client_objects[i].checkForCollision(client_objects[j]);

            client_objects[i].sprite.move(client_objects[i].velocity_x, client_objects[i].velocity_y);


            //(1) z powodu niedok�adno�ci kodowania liczb zmiennoprzecinkowych, wynik trzeba zaokr�gli�
            if (client_objects[i].actual_velocity > -0.2 && client_objects[i].actual_velocity < 0.2)
                client_objects[i].actual_velocity = 0;

            if (client_objects[i].actual_velocity > 0) {
                client_objects[i].actual_velocity -= FRICTION;
            } else if (client_objects[i].actual_velocity < 0) {
                client_objects[i].actual_velocity += FRICTION;
            }

            client_objects[i].didMove = client_objects[i].actual_velocity != 0;

            if (client_inputs[i].down) {
                if (client_objects[i].actual_velocity < MAX_VELOCITY)
                    client_objects[i].actual_velocity += D_VELOCITY;
                client_objects[i].didMove = true;
            }

            if (client_inputs[i].up) {
                if (client_objects[i].actual_velocity > -MAX_VELOCITY)
                    client_objects[i].actual_velocity -= D_VELOCITY;
                client_objects[i].didMove = true;
            }

            if (client_inputs[i].left) {
                client_objects[i].angle -= ANGLE_ROTATION;
                client_objects[i].angle += 360;
                client_objects[i].angle %= 360;
                client_objects[i].didMove = true;
            }

            if (client_inputs[i].right) {
                client_objects[i].angle += ANGLE_ROTATION;
                client_objects[i].angle += 360;
                client_objects[i].angle %= 360;
                client_objects[i].didMove = true;
            }
            client_objects[i].velocity_y =
                    client_objects[i].actual_velocity * cos(((float) client_objects[i].angle / 360.f) * 2 * M_PI);
            client_objects[i].velocity_x =
                    -client_objects[i].actual_velocity * sin(((float) client_objects[i].angle / 360.f) * 2 * M_PI);
        }
    }
        sendBack();
        clock.restart();
    }
}

bool isInterescting(Player_State &a, Player_State &b) {
    //ellipse is estimated with cirlce

    if(a.collision_cooldown.getElapsedTime().asSeconds()<0.3 && b.collision_cooldown.getElapsedTime().asSeconds()<0.3)
        return false;
    float x = a.sprite.getPosition().x - b.sprite.getPosition().x;
    float y = a.sprite.getPosition().y - b.sprite.getPosition().y;
    float dist = x * x + y * y;
    float radius = 32;
    return dist <= (radius + radius )*(radius + radius);

}
void collide(Player_State &a, Player_State &b) {


    float av = abs(a.actual_velocity);
    float bv = abs(b.actual_velocity);

    a.collision_cooldown.restart();
    b.collision_cooldown.restart();

    float tmp = a.actual_velocity;
    a.actual_velocity = b.actual_velocity;
    b.actual_velocity = tmp;

    float tmp_x = a.velocity_x;
    float tmp_y = b.velocity_y;
    a.velocity_x = b.velocity_x;
    a.velocity_y = b.velocity_y;
    b.velocity_x = tmp_x;
    b.velocity_x = tmp_y;

    //slower sumo gets rotation of faster
    if (av > bv) {
        b.angle = a.angle;
    }
    else if (av == bv) {
        sf::Int16 t = a.angle;
        a.angle = b.angle;
        b.angle = t;
    }
    else {
        a.angle = b.angle;
    }
}

bool Player_State::checkForCollision(Player_State &other) {
    if (!isInterescting(*this, other)) return false;
    collide(*this, other);

    return true;

}
