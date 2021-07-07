#include "Player.h"

Player::Player(float x, float y, std::string _texture) : NPC(x, y, _texture) {
	Alive = true;
	HP = 3;
}

void Player::setDamage(int damage) {
	HP -= damage;
	if (HP <= 0)
		Alive = false;
}

void Player::setHealt(int _HP) {
	HP += _HP;
}

int Player::getHealt() {
	return HP;
}

bool Player::isAlive() {
	return Alive;
}

sf::Vector2f Player::updateCamera(sf::RenderWindow *window, sf::Vector2f _Position, Map *m, sf::View *view) {
	float maxX = m->getMapLastPosition().x;
	float maxY = m->getMapLastPosition().y;
	sf::Vector2f v;

	if (_Position.x - (window->getSize().x / 4) > 0 && _Position.x + (window->getSize().x / 4) < maxX) {
		v.x = _Position.x;
	}
	else if (_Position.x - (window->getSize().x / 4) <= 0) {
		if (view->getCenter().x - (window->getSize().x / 4) <= 0)
			v.x = (window->getSize().x / 4);
		else
			v.x = view->getCenter().x;
	}
	else if (_Position.x + (window->getSize().x / 4) >= maxX) {
		if (view->getCenter().x + (window->getSize().x / 4) >= maxX)
			v.x = maxX - (window->getSize().x / 4);
		else
			v.x = view->getCenter().x;
	}
	else {
		v.x = _Position.x;
	}

	if (_Position.y - (window->getSize().y / 4) > 0 && _Position.y + (window->getSize().y / 4) < maxY) {
		v.y = _Position.y;
	}
	else if (_Position.y - (window->getSize().y / 4) <= 0) {
		if (view->getCenter().y - (window->getSize().y / 4) <= 0)
			v.y = (window->getSize().y / 4);
		else
			v.y = view->getCenter().y;
	}
	else if (_Position.y + (window->getSize().y / 4) >= maxY) {
		if (view->getCenter().y + (window->getSize().y / 4) <= 0)
			v.y = maxY - (window->getSize().y / 4);
		else
			v.y = view->getCenter().y;
	}
	else {
		v.y = _Position.y;
	}

	return v;
}