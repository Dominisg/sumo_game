//
// Created by dominis on 10.04.19.
//

#ifndef SUMO_PR_PROGRESSBAR_H
#define SUMO_PR_PROGRESSBAR_H

#include <SFML/System.hpp>
#include <SFML/Window.hpp>

class Progressbar {
    sf::RenderWindow *window;
    sf::Vector2f position;
    float length;
    float max_val;
    float curr_value=0;
    void draw();
public:
    Progressbar(sf::RenderWindow *window,sf::Vector2f position_middle,float len, float max_val);
    void updateProgress(float val);
};


#endif //SUMO_PR_PROGRESSBAR_H
