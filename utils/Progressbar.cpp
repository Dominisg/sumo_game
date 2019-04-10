//
// Created by dominis on 10.04.19.
//

#include <SFML/Graphics.hpp>
#include "Progressbar.h"

Progressbar::Progressbar(sf::RenderWindow *window,sf::Vector2f position_middle,float len, float max_val){
    this->window = window;
    this->position = position_middle;
    this->length = len;
    this->max_val = max_val;
    draw();

}

void Progressbar::draw() {
    sf::Vertex line[] =
        {
                sf::Vertex(sf::Vector2f(position.x-length, position.y+3)),
                sf::Vertex(sf::Vector2f(position.x+length, position.y+3))
        };
    window->draw(line, 2, sf::Lines);
    line[0].position.y -=6;
    line[1].position.y -=6;
    window->draw(line, 2, sf::Lines);

    if(curr_value!= 0){
        sf::RectangleShape progress(sf::Vector2f((curr_value/max_val)*length*2, 6));
        progress.setPosition(position.x-length,position.y-3);
        window->draw(progress);
    }
    window->display();
}

void Progressbar::updateProgress(float val){
    this->curr_value = val;
    draw();
}