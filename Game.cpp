//
// Created by dominis on 08.04.19.
//

#include "Game.h"
#include "utils/Progressbar.h"

Game::Game() {
    main_window = new sf::RenderWindow(sf::VideoMode(SCREENSIZE::X, SCREENSIZE::Y), "Sumo Slam");
    Progressbar p_bar(main_window,sf::Vector2f(SCREENSIZE::X/2,SCREENSIZE::Y/2),200.,72.);
    auto *tmptexture = new sf::Texture[72];
    for (int i = 0; i < 72; i++) {
        std::string filename = "spritesheets/sumo_angle" + std::to_string(5 * i) + ".png";
        tmptexture[i].loadFromFile(filename);
        p_bar.updateProgress(i);
    }
    Sumo::setTextures(tmptexture);
	players = new Sumo*[Sumo::getPlayersCounter() + 1];
	players[0] = new Sumo(0.0, 0.0);
    //players[1] = new Sumo(0.0, 0.0);
}

Game::~Game() {
	for (int i = 0; i < Sumo::getPlayersCounter(); i++)
		delete players[i];

    delete [] players;
    delete [] Sumo::getTextures();
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
		//players moving
		for (int i = 0; i < Sumo::getPlayersCounter(); i++) {
			players[i]->update();
		}
        main_window->clear();

		//drawing players
		for (int i = 0; i < Sumo::getPlayersCounter(); i++) {
			main_window->draw(players[i]->getContour());
			main_window->draw(players[i]->getSprite());
			
		}
        main_window->display();
    }
}