//
// Created by dominis on 08.04.19.
//

#ifndef SUMO_PR_GAME_H
#define SUMO_PR_GAME_H
#include <SFML/Graphics.hpp>
#include "Sumo.h"

enum SCREENSIZE{
    X = 1280,
    Y = 800
};

class Game{
private:
    sf::RenderWindow* main_window;
    Sumo** players;
	int playersInGame;
public:
    Game();
    ~Game();
    void mainLoop();
};


#endif //SUMO_PR_GAME_H
