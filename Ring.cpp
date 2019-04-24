//
// Created by domins on 24.04.19.
//

#include "Ring.h"
#include "Game.h"
#include <cmath>


sf::Sprite Ring::getSprite() {
    return sprite;
}

EllipseShape Ring::getContour() {
    return contour;
}

Ring::Ring(const char* texturepath) {

    texture.loadFromFile("spritesheets/ring.png");
    sprite.setTexture(texture);
    float px = (float)SCREENSIZE::X / 2 - sprite.getScale().x * 300 * 0.5 ;
    float py = (float)SCREENSIZE::Y / 2 - sprite.getScale().y * 300 * 0.5 ;

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

    for (int i =0; i<scon.getPointCount();i++){
        pos = scon.getPoint(i) + scon.getPosition();
        if(pow((pos.x-680),2)/(315*315) + pow((pos.y-245),2)/(165*165) < 1)
            return true;
    }
    return false;
}