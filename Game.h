//
// Created by dominis on 08.04.19.
//

#ifndef SUMO_PR_GAME_H
#define SUMO_PR_GAME_H
#include <SFML/Graphics.hpp>
#include "Sumo.h"
#include "Ring.h"

#define LOCAL_PLAYERS_MAX 2

enum SCREENSIZE{
    X = 1280,
    Y = 800
};

class Game{
private:
    sf::RenderWindow *main_window;
    Sumo **players;
    Ring *ring;
    void restartGame();
public:
    Game();
    ~Game();
    void mainLoop();
};


#endif //SUMO_PR_GAME_H
