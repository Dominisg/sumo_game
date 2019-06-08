#pragma once
#include "SFML/Graphics.hpp"
#include "Game.h"
#include <iostream>
#include <string>

#define MAX_ITEMS 3
class Menu
{
public:
	Menu();
	~Menu();

	void run(sf::RenderWindow &window);
	
private:
	int selectedIdx;
	enum GameState { MENU, JOIN, HOST, GAME_OVER, END };
	GameState state;
	sf::Font font;
	sf::Text menu_opts[MAX_ITEMS];


	void menu(sf::RenderWindow &window);
	void join(sf::RenderWindow &window);
	void host(sf::RenderWindow &window);
	void lobby(sf::RenderWindow &window,Game& game);

	std::string getAddress(sf::RenderWindow &window);
	std::string getIP(std::string);
	int getPort(std::string);
};

