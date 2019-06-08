//
// Created by dominis on 05.06.19.
//

#ifndef SUMO_PR_SERVER_H
#define SUMO_PR_SERVER_H
#include <SFML/Network.hpp>
#include <SFML/Graphics.hpp>
#include "../common/messages.h"



typedef struct Player_State Player_State;

 struct Player_State{
     sf::Sprite sprite;
     float velocity_x,velocity_y;
     float actual_velocity;
     sf::Int16 angle;
     bool didMove;
     sf::Clock collision_cooldown;
     bool ready;
     bool checkForCollision(Player_State &other);
};


struct IP_Endpoint
{
    sf::IpAddress address;
    unsigned short port;
    bool in_use;
};

class Server {
    sf::UdpSocket socket;
    sf::SocketSelector selector;
    IP_Endpoint client_endpoints[MAX_CLIENTS];
    sf::Clock time_since_heard_from_clients[MAX_CLIENTS];
    Player_State client_objects[MAX_CLIENTS];
    Player_Input client_inputs[MAX_CLIENTS]={};
    bool started = false;
    sf::Clock clock;
public:
    Server();
    void perform();
    void sendBack();
    void updateState();
};

#endif //SUMO_PR_SERVER_H
