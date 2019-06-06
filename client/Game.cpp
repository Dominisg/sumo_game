//
// Created by dominis on 08.04.19.
//

#include "Game.h"
#include "../utils/Progressbar.h"

Game::Game(sf::RenderWindow &main_window) {
   
    Progressbar p_bar(main_window,sf::Vector2f(SCREENSIZE::X/2,SCREENSIZE::Y/2),200.,72.);
    auto *tmptexture = new sf::Texture[72];

    for (int i = 0; i < 72; i++) {
        std::string filename = "spritesheets/sumo_angle" + std::to_string(5 * i) + ".png";
        tmptexture[i].loadFromFile(filename);
        p_bar.updateProgress(main_window, i);
    }
    Sumo::setTextures(tmptexture);

    ring= new Ring("spritesheets/ring.png");

	players = new Sumo*[LOCAL_PLAYERS_MAX];
	players[0] = new Sumo(180.f, -180.0, 230, sf::Color::Blue);
    players[1] = new Sumo(40, -100, 60, sf::Color::Red);

 //   socket_handler = new SocketHandler(sf::IpAddress::getLocalAddress(),5600);
}

Game::~Game() {
    int p_cnt = Sumo::getPlayersCounter();
	for (int i = 0; i < p_cnt; i++)
		delete players[i];

    delete [] players;
    delete [] Sumo::getTextures();
    //delete main_window;
}

void Game::mainLoop(sf::RenderWindow &main_window) {
    while (main_window.isOpen())
    {
        sf::Event event;

        while (main_window.pollEvent(event)) {
            switch (event.type) {

                // main_window closed
                case sf::Event::Closed:
                    main_window.close();
                    break;
                    // we don't process other types of events
                case sf::Event::KeyPressed:
                    if (event.key.code == sf::Keyboard::Escape) {
                        this->restartGame();
                    }
                    break;

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
        main_window.clear();

		main_window.draw(ring->getSprite());

        //drawing elipses
        for (int i = 0; i < Sumo::getPlayersCounter(); i++) {
            if(!players[i]->isDisabled())
                main_window.draw(players[i]->getContour());
        }

		//drawing players
		for (int i = 0; i < Sumo::getPlayersCounter(); i++) {
			main_window.draw(*players[i]);
		}

        //disabling players outside the ring
        for (int i = 0; i < Sumo::getPlayersCounter(); i++) {
            if (!ring->isInside(players[i])) {
                players[i]->disable();
            }
        }
        main_window.display();
    }
}

void Game::restartGame() {
    int p_cnt = Sumo::getPlayersCounter();
    for (int i = 0; i < p_cnt; i++)
        delete players[i];
    delete [] players;

    players = new Sumo*[LOCAL_PLAYERS_MAX];
    players[0] = new Sumo(180.f, -180.0, 230, sf::Color::Blue);
    players[1] = new Sumo(40, -100, 60, sf::Color::Red);
}

bool Game::join() {
    return socket_handler->join();
}

void Game::setSocketHandler(SocketHandler *socket_handler) {

    this->socket_handler = socket_handler;
}