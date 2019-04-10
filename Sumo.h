//
// Created by dominis on 08.04.19.
//
#include <SFML/Graphics.hpp>

#ifndef SUMO_PR_SUMO_H
#define SUMO_PR_SUMO_H


enum CONTROLS{
    arrows,
    wsad,
};

class Sumo : public sf::Drawable {
	sf::Sprite sprite;
	static sf::Texture *texture;
	sf::IntRect rectSprite;
	sf::Clock clock;

	float max_velocity = 6.f;
	float actual_velocity = 0.f;
	float d_velocity = 0.25f;	//musi by� wi�ksze ni� ograniczenie (1) w update()
	int angle_rotation = 5;
	int angle = 0;
	float friction = 0.04f;
	sf::Vector2f velocity{ 0.f, 0.f };
	CONTROLS control_setup;
	static int players_counter;

	void draw(sf::RenderTarget& target, sf::RenderStates state) const override;

public:
	Sumo(float px, float py);
	Sumo() = delete;
	~Sumo() = default;

	sf::Sprite getSprite();

	void update();
	void setDirection(int angle);
	static int getPlayersCounter();
	static sf::Texture* getTextures();
	static void setTextures(sf::Texture* t);
};

#endif //SUMO_PR_SUMO_H
