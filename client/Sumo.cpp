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

Sumo::Sumo(float x, float y,int angle, sf::Color color) {
	rectSprite = sf::IntRect(0, 0, 300, 300);
	sprite.setTextureRect(rectSprite);
	sprite.setScale(sf::Vector2f(0.5, 0.5));
    this->angle = angle;
    setDirection(angle/5);

	float px = (float)SCREENSIZE::X / 2 - sprite.getScale().x * 300 * 0.5 + x;
	float py = (float)SCREENSIZE::Y / 2 - sprite.getScale().y * 300 * 0.5 + y;

	contour.setRadius({ 32,28 });
	contour.setOrigin({ contour.getRadius().x, contour.getRadius().y });
	contour.setPosition(px, py+25);
	contour.setFillColor(sf::Color::Transparent);
	contour.setOutlineThickness(2);
	contour.setOutlineColor(color);
    contour.setRotation(angle);
	
	sprite.setOrigin({ (float)rectSprite.width/2, (float)rectSprite.height/2 });
	sprite.setPosition(px, py);
	control_setup = static_cast<CONTROLS>(players_counter++);
	disabled = false;
}

Sumo::~Sumo(){
    players_counter--;
}

sf::Sprite& Sumo::getSprite() {
	return sprite;
}

EllipseShape& Sumo::getContour() {
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
		contour.setRotation(angle);
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

		if(!disabled) {

            if (sf::Keyboard::isKeyPressed(keyboard_control[control_setup][0])) {
                if (actual_velocity < max_velocity)
                    actual_velocity += d_velocity;
                didMove = true;
            }

            if (sf::Keyboard::isKeyPressed(keyboard_control[control_setup][1])) {
                if (actual_velocity > -max_velocity)
                    actual_velocity -= d_velocity;
                didMove = true;
            }

            if (sf::Keyboard::isKeyPressed(keyboard_control[control_setup][2])) {
                angle -= angle_rotation;
                angle += 360;
                angle %= 360;
                didMove = true;
                //contour.rotate(-angle_rotation);
            }

            if (sf::Keyboard::isKeyPressed(keyboard_control[control_setup][3])) {
                angle += angle_rotation;
                angle += 360;
                angle %= 360;
                didMove = true;
                //contour.rotate(angle_rotation);
            }
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
float Sumo::getActualVelocity() {
	return actual_velocity;
}
void Sumo::setActualVelocity(float v) {
	actual_velocity = v;
}
void Sumo::setVelocity(sf::Vector2f v) {
	velocity = v;
}
sf::Vector2f Sumo::getVelocity() {
	return velocity;
}
int Sumo::getAngle() {
	return angle;
}
void Sumo::setAngle(int a) {
	angle = a;
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

bool isInterescting(Sumo &a, Sumo &b) {
	//ellipse is estimated with cirlce

	if(a.elapsedTimeCollision().asSeconds()<0.3 && b.elapsedTimeCollision().asSeconds()<0.3)
	    return false;
	float x = a.getContour().getPosition().x - b.getContour().getPosition().x;
	float y = a.getContour().getPosition().y - b.getContour().getPosition().y;
	float dist = x * x + y * y;
	float radius = a.getContour().getRadius().y;
	return dist <= (radius + radius )*(radius + radius);

}
sf::Vector2f collide(Sumo &a, Sumo &b) {


	float av = abs(a.getActualVelocity());
	float bv = abs(b.getActualVelocity());

    a.wasCollision();
    b.wasCollision();

	float tmp = a.getActualVelocity();
	a.setActualVelocity(b.getActualVelocity());
	b.setActualVelocity(tmp);

	sf::Vector2f tmp2 = a.getVelocity();
	a.setVelocity(b.getVelocity());
	b.setVelocity(tmp2);

	//slower sumo gets rotation of faster
	if (av > bv) {
		b.setAngle(a.getAngle());
	}
	else if (av == bv) {
		int t = a.getAngle();
		a.setAngle(b.getAngle());
		b.setAngle(t);
	}
	else {
		a.setAngle(b.getAngle());
	}

	return { 0.f, 0.f };

}

bool Sumo::checkForCollision(Sumo &other) {
	if (!isInterescting(*this, other)) return false;

	//problem jest w collide!!!
	//postaci się blokują
	collide(*this, other);

	return true;
	
}

void Sumo::disable() {
    disabled = true;
}

bool Sumo::isDisabled() {
    return disabled;
}

sf::Time Sumo::elapsedTimeCollision() {
    return collision_cooldown.getElapsedTime();
}
void Sumo::wasCollision() {
    collision_cooldown.restart();
}