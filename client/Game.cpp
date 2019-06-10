//
// Created by dominis on 08.04.19.
//
#include "Game.h"
#include "../utils/Progressbar.h"

Game::Game(SocketHandler *socket_handler) {
    this->socket_handler = socket_handler;
}

void Game::init(sf::RenderWindow &main_window){
    static int initialized=0;
    if(!initialized) {
        initialized = 1;
        Progressbar p_bar(main_window, sf::Vector2f(SCREENSIZE::X / 2, SCREENSIZE::Y / 2), 200., 72.);
        auto *tmptexture = new sf::Texture[72];

        for (int i = 0; i < 72; i++) {
            std::string filename = "spritesheets/sumo_angle" + std::to_string(5 * i) + ".png";
            tmptexture[i].loadFromFile(filename);
            p_bar.updateProgress(main_window, i);
        }
        Sumo::setTextures(tmptexture);

        ring = new Ring("spritesheets/ring.png");

        players = new Sumo *[MAX_CLIENTS];
        for (int i = 0; i < MAX_CLIENTS; i++)
            players[i] = new Sumo(DEFAULT_POSITIONS[i].x, DEFAULT_POSITIONS[i].y,
                                  DEFAULT_POSITIONS[i].angle, DEFAULT_POSITIONS[i].color, this);
    }
}

Game::~Game() {
	for (int i = 0; i < MAX_CLIENTS; i++)
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
//                    if (event.key.code == sf::Keyboard::Escape) {
//                        this->restartGame();
//                    }
                    break;

                default:
                    break;
            }
        }
        if(socket_handler->receive(players) == (int)Server_Message::Out)
            break;

        players[socket_handler->getPlayerId()]->sendInput();

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
            if (!players[i]->isDisabled())
                main_window.draw(*players[i]);
        }
        main_window.display();
    }
}

void Game::restartGame() {
    int p_cnt = Sumo::getPlayersCounter();
    for (int i = 0; i < p_cnt; i++)
        delete players[i];
    delete [] players;

    players = new Sumo*[MAX_CLIENTS];
    for (int i = 0; i < MAX_CLIENTS; i++ )
        players[i] = new Sumo(DEFAULT_POSITIONS[i].x, DEFAULT_POSITIONS[i].y,DEFAULT_POSITIONS[i].angle, sf::Color::Blue,this);

}

bool Game::join() {
    return socket_handler->join();
}

SocketHandler* Game::getSocketHandler(){
    return this->socket_handler;
}