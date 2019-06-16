#include "Menu.h"
#include "../server/Server.h"


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

	menu_opts[1].setFont(font);
	menu_opts[1].setCharacterSize(48);
	menu_opts[1].setFillColor(sf::Color::White);
	menu_opts[1].setString("Host game");
	menu_opts[1].setPosition(sf::Vector2f((w - menu_opts[1].getLocalBounds().width) / 2, h / (MAX_ITEMS + 1)*2));

	menu_opts[2].setFont(font);
	menu_opts[2].setCharacterSize(48);
	menu_opts[2].setFillColor(sf::Color::White);
	menu_opts[2].setString("Exit");
	menu_opts[2].setPosition(sf::Vector2f((w - menu_opts[2].getLocalBounds().width) / 2, h / (MAX_ITEMS + 1)*3));

	while (state == MENU)
	{
		sf::Vector2f mouse(sf::Mouse::getPosition(window));
		sf::Event event;

		while (window.pollEvent(event))
		{
			//Wci�ni�cie ESC lub przycisk X
			if (event.type == sf::Event::Closed || (event.type == sf::Event::KeyPressed &&
				event.key.code == sf::Keyboard::Escape))
				state = END;

			//klikni�cie MENU                                        
			else if (menu_opts[0].getGlobalBounds().contains(mouse) &&
				event.type == sf::Event::MouseButtonReleased && event.key.code == sf::Mouse::Left)
			{
				state = JOIN;
			}

			//klikni�cie EXIT
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
	std::string ip = getAddress(window);
	int port = getPort(ip),result;
	if (state == MENU) return;
	ip = getIP(ip);
	std::cout << ip << " " << port << std::endl;
    Game game(new SocketHandler(sf::IpAddress(ip),port));
    if(game.join()) {
        std::cout<<"Wchodze do lobby!"<<std::endl;
        lobby(window, game);
    }else{
        std::cout<<"Serwer nas nie wpuscil!";
    }

	state = MENU;
}

void Menu::host(sf::RenderWindow &window) {
	std::string p = getPort(window);
	if (state == MENU) return;

	//int port = getPort(ip);
	int port = atoi(p.c_str());
    Server server(port);
    sf::Thread thread(&Server::whilePerform,&server);
    thread.launch();
    std::cout<<port<<std::endl;
    sf::sleep(sf::seconds(0.1));
    Game game(new SocketHandler(sf::IpAddress(sf::IpAddress::LocalHost),port));
    if(game.join()) {
        hostLobby(window, game);
    }else{
        thread.terminate();
    }

	state = MENU;
}

void Menu::hostLobby(sf::RenderWindow &window, Game& game) {
	float w = window.getSize().x;
	float h = window.getSize().y;

	sf::Text title("Sumo Slam", font, 80);
	title.setStyle(sf::Text::Bold);
	title.setPosition((w - title.getGlobalBounds().width) / 2, 20);

	sf::Text lobby("Host lobby", font, 40);
	lobby.setStyle(sf::Text::Bold);
	lobby.setPosition((w - lobby.getGlobalBounds().width) / 2, 240);


	sf::Text start("Start!", font, 40);
	start.setStyle(sf::Text::Bold);
	start.setPosition((w - start.getGlobalBounds().width) / 2, 600);

	
	while (window.isOpen()) {
		sf::Event event;
		sf::Vector2f mouse(sf::Mouse::getPosition(window));

		while (window.pollEvent(event)) {
			switch (event.type) {
			case sf::Event::Closed:
                game.getSocketHandler()->sendLeave();
				window.close();
				break;
			case sf::Event::MouseMoved:
				if (start.getGlobalBounds().contains(mouse))
				{
					start.setFillColor(sf::Color::Cyan);
				}
				else start.setFillColor(sf::Color::White);
				break;
			case sf::Event::MouseButtonReleased:
				if (start.getGlobalBounds().contains(mouse))
				{
					if (event.type == sf::Event::MouseButtonReleased && event.key.code == sf::Mouse::Left) {
                        game.getSocketHandler()->sendStart();
					}
				}
				break;
			}
        }

        if(game.getSocketHandler()->listenLobby() == (int)Server_Message::Init){
            game.init(window);
            game.getSocketHandler()->sendReady();
            game.mainLoop(window);
        }
		window.clear();
		window.draw(title);
		window.draw(lobby);
		window.draw(start);
		window.display();
	}
}

void Menu::lobby(sf::RenderWindow &window, Game& game) {
	float w = window.getSize().x;
	float h = window.getSize().y;

	sf::Text title("Sumo Slam", font, 80);
	title.setStyle(sf::Text::Bold);
	title.setPosition((w - title.getGlobalBounds().width) / 2, 20);

	sf::Text lobby("Lobby", font, 40);
	lobby.setStyle(sf::Text::Bold);
	lobby.setPosition((w - lobby.getGlobalBounds().width) / 2, 240);


	sf::Text start("Waiting for host to start the game!", font, 40);
	start.setStyle(sf::Text::Bold);
	start.setPosition((w - start.getGlobalBounds().width) / 2, 600);


	while (window.isOpen()) {
		sf::Event event;
		sf::Vector2f mouse(sf::Mouse::getPosition(window));

		while (window.pollEvent(event)) {
			switch (event.type) {
			case sf::Event::Closed:
                game.getSocketHandler()->sendLeave();
				window.close();
				break;
			case sf::Event::MouseMoved:

				break;
			case sf::Event::MouseButtonReleased:

				break;

			}
		}
		if (game.getSocketHandler()->listenLobby() == (int)Server_Message::Init) {
			game.init(window);
			game.getSocketHandler()->sendReady();
			game.mainLoop(window);
		}
		window.clear();
		window.draw(title);
		window.draw(lobby);
		window.draw(start);
		window.display();
	}
}

std::string Menu::getAddress(sf::RenderWindow &window) {
	char port[] = "--------------------";
	int index = 0;
	float w = window.getSize().x;
	float h = window.getSize().y;

	sf::Text title("Sumo Slam", font, 80);
	title.setStyle(sf::Text::Bold);
	title.setPosition((w - title.getGlobalBounds().width) / 2, 20);

	sf::Text message("Server Address:", font, 40);
	message.setStyle(sf::Text::Bold);
	message.setPosition((w - message.getGlobalBounds().width) / 2, 240);


	sf::Text join("Join!", font, 40);
	join.setStyle(sf::Text::Bold);
	join.setPosition((w - join.getGlobalBounds().width) / 2, 600);

	sf::Text portTxt[20];
	for (int i = 0; i < 20; i++) {
		portTxt[i].setFont(font);
		portTxt[i].setString(port[i]);
		portTxt[i].setStyle(sf::Text::Bold);
		portTxt[i].setPosition((w - portTxt[i].getGlobalBounds().width + 20 * (i-10)) / 2, 300);
	}

	bool portSet = false;

	while (window.isOpen()) {
		sf::Event event;
		sf::Vector2f mouse(sf::Mouse::getPosition(window));

		while (window.pollEvent(event)) {
			switch (event.type) {
			case sf::Event::Closed:
				window.close();
				break;
			case sf::Event::TextEntered:
				if (index < 20 && (event.text.unicode >= '0' && event.text.unicode <= '9' || event.text.unicode == '.' || event.text.unicode == ':')) {
					port[index++] = char(event.text.unicode);
					if (index == 20) index--;
				}
				else if (index >= 0 && event.text.unicode == 8) { //backspace
					port[index--] = '-';
					if (index < 0) index = 0;

				}
				break;
			case sf::Event::MouseMoved:
				if (join.getGlobalBounds().contains(mouse))
				{
					join.setFillColor(sf::Color::Cyan);
				}
				else join.setFillColor(sf::Color::White);
				break;
			case sf::Event::MouseButtonReleased:
				if (join.getGlobalBounds().contains(mouse))
				{
					if (event.type == sf::Event::MouseButtonReleased && event.key.code == sf::Mouse::Left) {
						portSet = true;
					}
				}
				break;
			case sf::Event::KeyPressed:
				if(event.key.code == sf::Keyboard::Escape)
					state = MENU;
				break;
			}
		}

		for (int i = 0; i < 20; i++) {
			portTxt[i].setString(port[i]);
			portTxt[i].setStyle(sf::Text::Bold);
			portTxt[i].setFillColor(sf::Color::White);
		}
		portTxt[index].setFillColor(sf::Color::Cyan);
	
		window.clear();
		window.draw(title);
		window.draw(message);
		for (int i = 0; i < 20; i++) {
			window.draw(portTxt[i]);
		}
		window.draw(join);
		window.display();

		if (portSet == true) break;
		if (state == MENU) break;
	}
	port[index] = '\0';
	return port;
}

std::string Menu::getPort(sf::RenderWindow &window) {
	char port[] = "----";
	int index = 0;
	float w = window.getSize().x;
	float h = window.getSize().y;

	sf::Text title("Sumo Slam", font, 80);
	title.setStyle(sf::Text::Bold);
	title.setPosition((w - title.getGlobalBounds().width) / 2, 20);

	sf::Text message("Server Port:", font, 40);
	message.setStyle(sf::Text::Bold);
	message.setPosition((w - message.getGlobalBounds().width) / 2, 240);


	sf::Text join("Create!", font, 40);
	join.setStyle(sf::Text::Bold);
	join.setPosition((w - join.getGlobalBounds().width) / 2, 600);

	sf::Text portTxt[4];
	for (int i = 0; i < 4; i++) {
		portTxt[i].setFont(font);
		portTxt[i].setString(port[i]);
		portTxt[i].setStyle(sf::Text::Bold);
		portTxt[i].setPosition((w - portTxt[i].getGlobalBounds().width + 20 * (i - 2)) / 2, 300);
	}

	bool portSet = false;

	while (window.isOpen()) {
		sf::Event event;
		sf::Vector2f mouse(sf::Mouse::getPosition(window));

		while (window.pollEvent(event)) {
			switch (event.type) {
			case sf::Event::Closed:
				window.close();
				break;
			case sf::Event::TextEntered:
				if (index < 4 && (event.text.unicode >= '0' && event.text.unicode <= '9')) {
					port[index++] = char(event.text.unicode);
					if (index == 4) index--;
				}
				else if (index >= 0 && event.text.unicode == 8) { //backspace
					port[index--] = '-';
					if (index < 0) index = 0;

				}
				break;
			case sf::Event::MouseMoved:
				if (join.getGlobalBounds().contains(mouse))
				{
					join.setFillColor(sf::Color::Cyan);
				}
				else join.setFillColor(sf::Color::White);
				break;
			case sf::Event::MouseButtonReleased:
				if (join.getGlobalBounds().contains(mouse))
				{
					if (event.type == sf::Event::MouseButtonReleased && event.key.code == sf::Mouse::Left) {
						portSet = true;
					}
				}
				break;
			case sf::Event::KeyPressed:
				if (event.key.code == sf::Keyboard::Escape)
					state = MENU;
				break;
			}
		}

		for (int i = 0; i < 4; i++) {
			portTxt[i].setString(port[i]);
			portTxt[i].setStyle(sf::Text::Bold);
			portTxt[i].setFillColor(sf::Color::White);
		}
		portTxt[index].setFillColor(sf::Color::Cyan);

		window.clear();
		window.draw(title);
		window.draw(message);
		for (int i = 0; i < 4; i++) {
			window.draw(portTxt[i]);
		}
		window.draw(join);
		window.display();

		if (portSet == true) break;
		if (state == MENU) break;
	}
	int last_used=-1;
	while(port[++last_used] != '-' && port[last_used] != '\0');

	port[last_used] = '\0';
	return port;
}

std::string Menu::getIP(std::string s) {
	int index = 0;
	while (s[index] != ':')
		index++;
	return s.substr(0,index);
}

int Menu::getPort(std::string s) {
	std::string p = s.substr(s.find(':') + 1);
	return atoi(p.c_str());
}
