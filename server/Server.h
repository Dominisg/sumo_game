//
// Created by dominis on 05.06.19.
//

#ifndef SUMO_PR_SERVER_H
#define SUMO_PR_SERVER_H
#include <SFML/Network.hpp>
#include "../common/messages.h"

typedef struct{
    float velocity_x,velocity_y;
    sf::Int16 angle;
}Player_State;

struct IP_Endpoint
{
    sf::IpAddress address;
    unsigned short port;
    bool in_use;
};

struct Player_Input
{
    bool up, down, left, right;
};

class Server {
    sf::UdpSocket socket;
    sf::SocketSelector selector;
    IP_Endpoint client_endpoints[MAX_CLIENTS];
    sf::Clock time_since_heard_from_clients[MAX_CLIENTS];
    Player_State client_objects[MAX_CLIENTS];
    Player_Input client_inputs[MAX_CLIENTS];
public:
    Server();
    void perform();
    void sendBack();
};



#endif //SUMO_PR_SERVER_H
