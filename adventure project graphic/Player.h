#pragma once
#ifndef Player_h
#define Player_h
#include "NPC.h"

class Player : public NPC {
	int HP;
	bool Alive;		//czy �yje
public:
	Player(float x, float y, std::string _texture);

	void setDamage(int damage);		//jak gracz przyjmie damage to u�yj tego by odj�� mu pewn� ilo�� hp; jak ilo�� hp b�dzie < 0 to zmieni status Alive na false
	void setHealt(int _HP);			// umo�liwia uleczenie gracza o pewn� ilo�� hp
	sf::Vector2f updateCamera(sf::RenderWindow *window, sf::Vector2f _Position, Map *m, sf::View *view);	//aktualizuje pozycj� kamery

	int getHealt();

	bool isAlive();		// zwraca warto�� Alive
};

#endif // !Player_h
