//
// Created by dominis on 08.04.19.
//
#include "Game.h"

sf::Keyboard::Key  keyboard_control[LOCAL_PLAYERS_MAX][4] = {
        {sf::Keyboard::Down,sf::Keyboard::Up,sf::Keyboard::Left,sf::Keyboard::Right},
        {sf::Keyboard::S,sf::Keyboard::W,sf::Keyboard::A,sf::Keyboard::D}
    };

int Sumo::players_counter = 0;
sf::Texture* Sumo::texture = nullptr;

Sumo::Sumo(float x, float y,sf::Int16 angle, sf::Color color, Game* game) {
	rectSprite = sf::IntRect(0, 0, 300, 300);
	sprite.setTextureRect(rectSprite);
	sprite.setScale(sf::Vector2f(0.5, 0.5));
    this->angle = angle;
    setDirection(angle/5);

	contour.setRadius({ 32,28 });
	contour.setOrigin({ contour.getRadius().x, contour.getRadius().y });
	contour.setPosition(x, y+25);
	contour.setFillColor(sf::Color::Transparent);
	contour.setOutlineThickness(2);
	contour.setOutlineColor(color);
    contour.setRotation(angle);

	
	sprite.setOrigin({ (float)rectSprite.width/2, (float)rectSprite.height/2 });
	sprite.setPosition(x, y);
	control_setup = static_cast<CONTROLS>(players_counter++);
	disabled = false;
	still_in_game = true;
    didMove = false;
	this->game = game;
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
}


void Sumo::sendInput(){

    if (clock.getElapsedTime().asSeconds() > 0.011f) {
        Player_Input input = {};
        if (!disabled) {

            if (sf::Keyboard::isKeyPressed(keyboard_control[0][0])) {
                input.down = true;
            }

            if (sf::Keyboard::isKeyPressed(keyboard_control[0][1])) {

                input.up = true;
            }

            if (sf::Keyboard::isKeyPressed(keyboard_control[0][2])) {
                input.left = true;
            }

            if (sf::Keyboard::isKeyPressed(keyboard_control[0][3])) {
                input.right = true;
            }
        }


        this->game->getSocketHandler()->sendInput(input);
    }

}


void Sumo::update() {
	if (clock.getElapsedTime().asSeconds() > 0.022f) {
		contour.setRotation(angle);

		setDirection(angle / 5);

		if (this->didMove) {
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
void Sumo::setAngle(sf::Int16 a) {
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

void Sumo::disable(bool t) {
    disabled = t;
}

bool Sumo::isDisabled() {
    return disabled;
}

void Sumo::setSumoDidMove(bool val) {
    didMove = val;
}

void Sumo::inGame(bool t) {
    still_in_game = t;
}

bool Sumo::isInGame() {
   return still_in_game;
}