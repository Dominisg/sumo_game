//
// Created by dominis on 08.04.19.
//
#include "Game.h"
#include "Sumo.h"
#include <SFML/Window/Keyboard.hpp>

Sumo::Sumo() {
    rectSprite = sf::IntRect(300,0,300,300);
    texture[LEFT].loadFromFile("spritesheets/sumo_angle90.png");
    texture[UP].loadFromFile("spritesheets/sumo_angle180.png");
    texture[RIGHT].loadFromFile("spritesheets/sumo_angle270.png");
    texture[DOWN].loadFromFile("spritesheets/sumo_angle0.png");
    texture[DOWN_RIGHT].loadFromFile("spritesheets/sumo_angle315.png");
    texture[DOWN_LEFT].loadFromFile("spritesheets/sumo_angle45.png");
    texture[UP_RIGHT].loadFromFile("spritesheets/sumo_angle235.png");
    texture[UP_LEFT].loadFromFile("spritesheets/sumo_angle135.png");

    sprite = sf::Sprite(texture[UP]);
    sprite.setScale(sf::Vector2f(0.5,0.5));
    sprite.setPosition((float)SCREENSIZE::X/2 - sprite.getScale().x * 300 * 0.5,(float)SCREENSIZE::Y/2 - sprite.getScale().y* 300*0.5);
}

sf::Sprite Sumo::getSprite() {
    return sprite;
}

void Sumo::moveHandle(){

    sf::Vector2f start, pos = sprite.getPosition();
    start = pos;
    float speed = 1.;
    if (clock.getElapsedTime().asSeconds() > 0.022f){

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
        {
            setDirection(UP);
            pos.y -= speed;
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
        {
            setDirection(DOWN);
            pos.y += speed;
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        {
            setDirection(LEFT);
            pos.x -= speed;
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        {
            setDirection(RIGHT);
            pos.x += speed;
        }

        if(start != pos) {
            if(start.x != pos.x && start.y != pos.y){
                if(start.x > pos.x && start.y>pos.y)
                    setDirection(UP_LEFT);
                else if (start.x < pos.x && start.y>pos.y)
                    setDirection(UP_RIGHT);
                else if (start.x > pos.x && start.y<pos.y)
                    setDirection(DOWN_LEFT);
                else
                    setDirection(DOWN_RIGHT);
            }

            //frames changing
            if (rectSprite.left == 1500) {
                rectSprite.left = 0;
                if (rectSprite.top != 1500)
                    rectSprite.top += 300;
                else
                    rectSprite.top = 0;
            } else
                rectSprite.left += 300;

            sprite.setTextureRect(rectSprite);
        }


        sprite.setPosition(pos);
        clock.restart();
    }
}

void Sumo::setDirection(DIRECTIONS dir) {
 this->sprite.setTexture(texture[dir]);
}
