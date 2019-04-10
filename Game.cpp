//
// Created by dominis on 08.04.19.
//

#include "Game.h"

Game::Game() {
    main_window = new sf::RenderWindow(sf::VideoMode(SCREENSIZE::X, SCREENSIZE::Y), "Sumo Slam");
	playersInGame = 1;
	players = new Sumo*[playersInGame];
	players[0] = new Sumo(0.0, 0.0);

}

Game::~Game() {
	for (int i = 0; i < playersInGame; i++)
		delete players[i];

    delete [] players;
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
        players[0]->update();
        main_window->clear();
        main_window->draw(players[0]->getSprite());
        main_window->display();
    }
}