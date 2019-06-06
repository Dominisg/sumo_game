#include "Menu.h"


//sf::RenderWindow window(sf::VideoMode(1280, 800), "Sumo Slam");
//RenderWindow window(VideoMode(1280, 800), "Sumo Slam");

Menu::Menu()
{
	state = END;
	if (!font.loadFromFile("AmaticSC-Regular.ttf")) {
		//error
	}
	state = MENU;
}

Menu::~Menu()
{
}

void Menu::run(sf::RenderWindow &window) {
	while (state != END) {
		switch (state) {
		case MENU:
			menu(window);
			break;
		case JOIN:
			join(window);
			break;
		case HOST:
			host(window);
			break;
		}
	}
}

void Menu::menu(sf::RenderWindow &window) {
	float w = window.getSize().x;
	float h = window.getSize().y;

	sf::Text title("Sumo Slam", font, 80);
	title.setStyle(sf::Text::Bold);
	title.setPosition((w - title.getGlobalBounds().width) / 2, 20);


	menu_opts[0].setFont(font);
	menu_opts[0].setCharacterSize(48);
	menu_opts[0].setFillColor(sf::Color::White);
	menu_opts[0].setString("Join game");
	menu_opts[0].setPosition(sf::Vector2f((w - menu_opts[0].getLocalBounds().width) / 2, h / (MAX_ITEMS + 1)));

	std::cout << "0" << std::endl;
	menu_opts[1].setFont(font);
	menu_opts[1].setCharacterSize(48);
	menu_opts[1].setFillColor(sf::Color::White);
	menu_opts[1].setString("Host game");
	menu_opts[1].setPosition(sf::Vector2f((w - menu_opts[1].getLocalBounds().width) / 2, h / (MAX_ITEMS + 1)*2));
	std::cout << "1" << std::endl;

	menu_opts[2].setFont(font);
	menu_opts[2].setCharacterSize(48);
	menu_opts[2].setFillColor(sf::Color::White);
	menu_opts[2].setString("Exit");
	menu_opts[2].setPosition(sf::Vector2f((w - menu_opts[2].getLocalBounds().width) / 2, h / (MAX_ITEMS + 1)*3));
	std::cout << "2" << std::endl;

	while (state == MENU)
	{
		sf::Vector2f mouse(sf::Mouse::getPosition(window));
		sf::Event event;

		while (window.pollEvent(event))
		{
			//Wciœniêcie ESC lub przycisk X
			if (event.type == sf::Event::Closed || event.type == sf::Event::KeyPressed &&
				event.key.code == sf::Keyboard::Escape)
				state = END;

			//klikniêcie MENU                                        
			else if (menu_opts[0].getGlobalBounds().contains(mouse) &&
				event.type == sf::Event::MouseButtonReleased && event.key.code == sf::Mouse::Left)
			{
				state = JOIN;
			}

			//klikniêcie EXIT
			else if (menu_opts[1].getGlobalBounds().contains(mouse) &&
				event.type == sf::Event::MouseButtonReleased && event.key.code == sf::Mouse::Left)
			{
				state = HOST;
			}

			else if (menu_opts[2].getGlobalBounds().contains(mouse) &&
				event.type == sf::Event::MouseButtonReleased && event.key.code == sf::Mouse::Left)
			{
				state = END;
			}
		}
		for (int i = 0; i < MAX_ITEMS; i++)
			if (menu_opts[i].getGlobalBounds().contains(mouse))
				menu_opts[i].setFillColor(sf::Color::Cyan);
			else menu_opts[i].setFillColor(sf::Color::White);

		window.clear();

		window.draw(title);
		for (int i = 0; i < MAX_ITEMS; i++)
			window.draw(menu_opts[i]);

		window.display();
	}

}

void Menu::join(sf::RenderWindow &window) {
	//window.clear();
	//window.display();
	std::cout << "XD" << std::endl;
	Game game(window);
	game.mainLoop(window);
}

void Menu::host(sf::RenderWindow &window) {
	std::cout << "Hostowanko" << std::endl;
	state = MENU;
}