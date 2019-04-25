//
// Created by domins on 24.04.19.
//

#include "Ring.h"
#include "Game.h"
#include <cmath>
#include <iostream>


sf::Sprite Ring::getSprite() {
    return sprite;
}

EllipseShape Ring::getContour() {
    return contour;
}

Ring::Ring(const char* texturepath) {

    texture.loadFromFile("spritesheets/ring.png");
    sprite.setTexture(texture);

    contour.setRadius({ 315,165 });
    contour.setOrigin({ contour.getRadius().x, contour.getRadius().y });
    contour.setPosition(680, 245);
    contour.setFillColor(sf::Color::Transparent);
    contour.setOutlineThickness(2);
    contour.setOutlineColor(sf::Color::White);
}

bool Ring::isInside(Sumo *s) {
    EllipseShape scon = s->getContour();
    sf::Vector2f pos;
    sf::Transform tf = scon.getTransform();


    for (int i =0; i<scon.getPointCount();i++){
        pos = scon.getPoint(i);
        pos = tf.transformPoint(pos);

        if(pow((pos.x-680),2)/pow(315,2) + pow((pos.y-245),2)/pow(165,2) < 1)
            return true;
    }
    return false;
}