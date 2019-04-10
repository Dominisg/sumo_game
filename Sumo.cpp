//
// Created by dominis on 08.04.19.
//
#include "Game.h"
#include <SFML/Window/Keyboard.hpp>
#include <iostream>
#include <string>
#define _USE_MATH_DEFINES
#include <math.h>

Sumo::Sumo(float px, float py) {
	rectSprite = sf::IntRect(0, 0, 300, 300);

	for (int i = 0; i < 72; i++) {
		std::string filename = "spritesheets/sumo_angle" + std::to_string(5 * i) + ".png";
		texture[i].loadFromFile(filename);
	}
	
	setDirection(0);
	sprite.setScale(sf::Vector2f(0.5, 0.5));
	px = (float)SCREENSIZE::X / 2 - sprite.getScale().x * 300 * 0.5;
	py = (float)SCREENSIZE::Y / 2 - sprite.getScale().y * 300 * 0.5;
	sprite.setOrigin({ (float)rectSprite.width/2, (float)rectSprite.height/2 });
	sprite.setPosition(px, py);
}

sf::Sprite Sumo::getSprite() {
	return sprite;
}

void Sumo::draw(sf::RenderTarget& target, sf::RenderStates state) const {
	target.draw(this->sprite, state);
}

void Sumo::update() {
	//sf::Vector2f start, pos = sprite.getPosition();
	//start = pos;
	
	if (clock.getElapsedTime().asSeconds() > 0.022f) {
		std::cout << this->velocity.x << " " << this->velocity.y << std::endl;
		this->sprite.move(this->velocity);
		
		//(1) z powodu niedok³adnoœci kodowania liczb zmiennoprzecinkowych, wynik trzeba zaokr¹gliæ
		if (actual_velocity > -0.2 && actual_velocity < 0.2)
			actual_velocity = 0;
		
		if (actual_velocity > 0) {
			actual_velocity -= friction;
		}
		else if (actual_velocity < 0) {
			actual_velocity += friction;
		}
		

		bool didMove = actual_velocity !=0 ? true : false;
		
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		{
			if(actual_velocity < max_velocity)
				actual_velocity += d_velocity;
			didMove = true;
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		{
			if (actual_velocity > -max_velocity)
				actual_velocity -= d_velocity;
			didMove = true;
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		{
			angle-=angle_rotation;
			angle += 360;
			angle %= 360;
			didMove = true;
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		{
			angle+=angle_rotation;
			angle += 360;
			angle %= 360;
			didMove = true;
		}

		velocity.y = actual_velocity * cos(((float)angle / 360.f) * 2 * M_PI);
		velocity.x = - actual_velocity * sin(((float)angle / 360.f) * 2 * M_PI);
		
		setDirection(angle / 5);

		if (didMove) {
			//frames changing
			if (rectSprite.left == 1500) {
				rectSprite.left = 0;
				if (rectSprite.top != 1500)
					rectSprite.top += 300;
				else
					rectSprite.top = 0;
			}
			else
				rectSprite.left += 300;

			
		}
		sprite.setTextureRect(rectSprite);
		clock.restart();
		
	}
	
}

void Sumo::setDirection(int dir) {
	this->sprite.setTexture(texture[dir]);
}
