#include "Menu.h"

int main()
{
	sf::ContextSettings settings;
	settings.antialiasingLevel = 4;
	sf::RenderWindow window(sf::VideoMode(1280, 800), "Sumo Slam",sf::Style::Default,settings);

	Menu menu = Menu();
	menu.run(window);
    return 0;
}