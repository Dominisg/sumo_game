//
// Created by domins on 24.04.19.
//

#ifndef SUMO_PR_RING_H
#define SUMO_PR_RING_H


#include <SFML/Graphics.hpp>
#include "EllipseShape.h"
#include "Sumo.h"

class Ring {
    sf::Texture texture;
    sf::Sprite sprite;
    EllipseShape contour;

public:
    Ring(const char* texturepath);
    sf::Sprite getSprite();
    EllipseShape getContour();
    bool isInside(Sumo * s);
};


#endif //SUMO_PR_RING_H
