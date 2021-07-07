#pragma once
#ifndef camera_h
#define camera_h
#include "includes.h"
#include "Map.h"

class camera {
	int maxX;
	int maxY;
	sf::Vector2f Position;

public:
	camera(Map m);

	void update(sf::RenderWindow *window, sf::Vector2f _Position);

	sf::Vector2f getPosition();
};

#endif // !camera_h
