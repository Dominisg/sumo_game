//
// Created by dominis on 08.04.19.
//

#include "Game.h"

Game::Game() {
    main_window = new sf::RenderWindow(sf::VideoMode(SCREENSIZE::X, SCREENSIZE::Y), "Sumo Slam");
    player = new Sumo[1]();

}

Game::~Game() {
    delete [] player;
    delete main_window;
}

void Game::mainLoop() {
    while (main_window->isOpen())
    {
        sf::Event event;
        while (main_window->pollEvent(event))
        {
            switch (event.type)
            {
                // main_window closed
                case sf::Event::Closed:
                    main_window->close();
                    break;
                    // we don't process other types of events
                default:
                    break;
            }
        }
        player[0].moveHandle();
        main_window->clear();
        main_window->draw(player[0].getSprite());
        main_window->display();
    }
}