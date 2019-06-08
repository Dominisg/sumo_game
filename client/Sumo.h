//
// Created by dominis on 08.04.19.
//
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include "../utils/EllipseShape.h"

#ifndef SUMO_PR_SUMO_H
#define SUMO_PR_SUMO_H

class Game;

enum CONTROLS{
    arrows,
    wsad,
};

class Sumo : public sf::Drawable {
	sf::Sprite sprite;
	static sf::Texture *texture;
	sf::IntRect rectSprite;
	EllipseShape contour;
	sf::Clock clock;

	sf::Int16 angle = 0;
	CONTROLS control_setup;
	static int players_counter;
	bool disabled;
	Game* game;
    bool didMove;

	void draw(sf::RenderTarget& target, sf::RenderStates state) const override;

public:
	Sumo(float x, float y,sf::Int16 angle, sf::Color color,Game* game);
	Sumo() = delete;
	~Sumo();

	sf::Sprite& getSprite();
	EllipseShape& getContour();

	void update();
	void setDirection(int angle);
	void setAngle(sf::Int16);
	static int getPlayersCounter();
	static sf::Texture* getTextures();
	static void setTextures(sf::Texture* t);
	void disable(bool);
	bool isDisabled();
    void setSumoDidMove(bool);
    void sendInput();
};

#endif//SUMO_PR_SUMO_H
