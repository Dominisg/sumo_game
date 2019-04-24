//
// Created by dominis on 08.04.19.
//

#include "Game.h"
#include "utils/Progressbar.h"

Game::Game() {
    sf::ContextSettings settings;
    settings.antialiasingLevel = 4;
    main_window = new sf::RenderWindow(sf::VideoMode(SCREENSIZE::X, SCREENSIZE::Y), "Sumo Slam",sf::Style::Default, settings);
    Progressbar p_bar(main_window,sf::Vector2f(SCREENSIZE::X/2,SCREENSIZE::Y/2),200.,72.);
    auto *tmptexture = new sf::Texture[72];

    for (int i = 0; i < 72; i++) {
        std::string filename = "spritesheets/sumo_angle" + std::to_string(5 * i) + ".png";
        tmptexture[i].loadFromFile(filename);
        p_bar.updateProgress(i);
    }
    Sumo::setTextures(tmptexture);

    ring= new Ring("spritesheets/ring.png");

	players = new Sumo*[Sumo::getPlayersCounter() + 1];
	players[0] = new Sumo(200.f, 0.0, sf::Color::Blue);
    players[1] = new Sumo(-200.f, 0.0, sf::Color::Red);
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
		//check only once for two players
		players[0]->checkForCollision(*players[1]);
		//players[1]->checkForCollision(*players[0]);
		for (int i = 0; i < Sumo::getPlayersCounter(); i++) {
			players[i]->update();
		}
        main_window->clear();

		main_window->draw(ring->getSprite());
        main_window->draw(ring->getContour());

        //drawing elipses
        for (int i = 0; i < Sumo::getPlayersCounter(); i++) {
            main_window->draw(players[i]->getContour());
        }

		//drawing players
		for (int i = 0; i < Sumo::getPlayersCounter(); i++) {
			main_window->draw(*players[i]);
		}

        //drawing players

        printf(ring->isInside(players[0])?"true\n":"false\n");
        main_window->display();
    }
}