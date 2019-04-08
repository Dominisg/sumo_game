//
// Created by dominis on 08.04.19.
//
#include <SFML/Graphics.hpp>

#ifndef SUMO_PR_SUMO_H
#define SUMO_PR_SUMO_H

enum DIRECTIONS {
    UP,
    DOWN,
    LEFT,
    RIGHT,
    UP_LEFT,
    UP_RIGHT,
    DOWN_LEFT,
    DOWN_RIGHT
};

class Sumo {

private:
    sf::Texture texture[DOWN_RIGHT - UP + 1];
    sf::Sprite sprite;
    sf::IntRect rectSprite;
    sf::Clock clock;
public:
    Sumo();

    sf::Sprite getSprite();
    void moveHandle();
    void setDirection(DIRECTIONS dir);
};

#endif //SUMO_PR_SUMO_H
