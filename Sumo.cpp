//
// Created by dominis on 08.04.19.
//
#include "Game.h"
#include <SFML/Window/Keyboard.hpp>
#include <iostream>
#include <string>
#define _USE_MATH_DEFINES
#include <math.h>

sf::Keyboard::Key  keyboard_control[LOCAL_PLAYERS_MAX][4] = {
        {sf::Keyboard::Down,sf::Keyboard::Up,sf::Keyboard::Left,sf::Keyboard::Right},
        {sf::Keyboard::S,sf::Keyboard::W,sf::Keyboard::A,sf::Keyboard::D}
    };

int Sumo::players_counter = 0;
sf::Texture* Sumo::texture = nullptr;

Sumo::Sumo(float px, float py) {
	rectSprite = sf::IntRect(0, 0, 300, 300);
	sprite.setTextureRect(rectSprite);
	setDirection(0);
	sprite.setScale(sf::Vector2f(0.5, 0.5));


	px = (float)SCREENSIZE::X / 2 - sprite.getScale().x * 300 * 0.5;
	py = (float)SCREENSIZE::Y / 2 - sprite.getScale().y * 300 * 0.5;

	contour.setRadius({ 32,28 });
	contour.setOrigin({ contour.getRadius().x, contour.getRadius().y });
	contour.setPosition(px, py+25);
	contour.setFillColor(sf::Color::Transparent);
	contour.setOutlineThickness(2);
	contour.setOutlineColor(sf::Color::Yellow);

	
	sprite.setOrigin({ (float)rectSprite.width/2, (float)rectSprite.height/2 });
	sprite.setPosition(px, py);
	control_setup = static_cast<CONTROLS>(players_counter++);
}

sf::Sprite Sumo::getSprite() {
	return sprite;
}

EllipseShape Sumo::getContour() {
	return contour;
}

void Sumo::draw(sf::RenderTarget& target, sf::RenderStates state) const {
	target.draw(this->sprite, state);
	//target.draw(this->contour, state);
}

void Sumo::update() {
	
	if (clock.getElapsedTime().asSeconds() > 0.022f) {
		this->sprite.move(this->velocity);
		this->contour.move(this->velocity);

		//(1) z powodu niedok�adno�ci kodowania liczb zmiennoprzecinkowych, wynik trzeba zaokr�gli�
		if (actual_velocity > -0.2 && actual_velocity < 0.2)
			actual_velocity = 0;
		
		if (actual_velocity > 0) {
			actual_velocity -= friction;
		}
		else if (actual_velocity < 0) {
			actual_velocity += friction;
		}	

		bool didMove = actual_velocity !=0;
		
		if (sf::Keyboard::isKeyPressed(keyboard_control[control_setup][0]))
		{
			if(actual_velocity < max_velocity)
				actual_velocity += d_velocity;
			didMove = true;
		}

		if (sf::Keyboard::isKeyPressed(keyboard_control[control_setup][1]))
		{
			if (actual_velocity > -max_velocity)
				actual_velocity -= d_velocity;
			didMove = true;
		}

		if (sf::Keyboard::isKeyPressed(keyboard_control[control_setup][2]))
		{
			angle-=angle_rotation;
			angle += 360;
			angle %= 360;
			didMove = true;
			contour.rotate(-angle_rotation);
		}

		if (sf::Keyboard::isKeyPressed(keyboard_control[control_setup][3]))
		{
			angle+=angle_rotation;
			angle += 360;
			angle %= 360;
			didMove = true;
			contour.rotate(angle_rotation);
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

int Sumo::getPlayersCounter() {
    return players_counter;
}

sf::Texture * Sumo::getTextures() {
    return texture;
}

void Sumo::setTextures(sf::Texture *t) {
    texture = t;
}
