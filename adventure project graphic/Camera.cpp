#include "Camera.h"

camera::camera(Map m) {
	maxX = m.getMapLastPosition().x;
	maxY = m.getMapLastPosition().y;
}

void camera::update(sf::RenderWindow *window, sf::Vector2f _Position) {
	sf::View view;
	std::cout << "Camera update\n";
	if (!(_Position.x - (window->getSize().x / 2) < 0 || _Position.y - (window->getSize().y) < 0 || _Position.x + (window->getSize().x / 2) > maxX || _Position.y + (window->getSize().y / 2) > maxY)){
		view.setCenter(_Position);
		Position = _Position;
	}
	else if (_Position.x - (window->getSize().x / 2) < 0 && _Position.y - (window->getSize().y) < 0) {
		sf::Vector2f v((window->getSize().x / 2), (window->getSize().y));
		view.setCenter(v);
		Position = v;
	}
	else if(_Position.x - (window->getSize().x / 2) < 0) {
		sf::Vector2f v((window->getSize().x / 2), _Position.y);
		view.setCenter(v);
		Position = v;
	}
	else if (_Position.y - (window->getSize().y / 2) < 0) {
		sf::Vector2f v((window->getSize().y / 2), _Position.y);
		view.setCenter(v);
		Position = v;
	}
	else if (_Position.x + (window->getSize().x / 2) > maxX && _Position.y + (window->getSize().y / 2) > maxY) {
		sf::Vector2f v(maxX - (window->getSize().x / 2), maxY - (window->getSize().y));
		view.setCenter(v);
		Position = v;
	}
	else if (_Position.x + (window->getSize().x / 2) > maxX) {
		sf::Vector2f v(maxX - (window->getSize().x / 2), _Position.y);
		view.setCenter(v);
		Position = v;
	}
	else if (_Position.y + (window->getSize().y / 2) > maxY) {
		sf::Vector2f v(_Position.x, maxY - (window->getSize().y / 2));
		view.setCenter(v);
		Position = v;
	}
	else {
		std::cout << "Camera error!\n";
		exit(-3);
	}
	view.setSize(window->getSize().x, window->getSize().y);	//ustawiam rozdzielczoœæ kamery
	view.zoom(0.5);

	window->setView(view);
}

sf::Vector2f camera::getPosition() {
	return Position;
}