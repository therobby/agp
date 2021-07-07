#pragma once
#ifndef NPC_h
#define NPC_h
#include "includes.h"
#include "Map.h"
//Ta klasa bêdzie jeszcze przez mnie ulepszana 
class NPC {
	sf::Vector2f Position;	//aktualna pozycja npc
	sf::Vector2f lastPosition;	//poprzednia pozycja npc
	sf::Texture texture[15];	//textura npc
	sf::Sprite sprite;		//sprite npc
	float Jump;		//wysokosc skoku
	float height;	//aktualna wysokosc npc wzgledem skoku
	bool canJump;	//czy moze teraz skoczyc jeszcze raz
	int moveState;	//0-stoi w miejscu, 1-2-3-4- idzie w lewo, 5-6-7-8 -idzie w prawo
	int airState;	//0-na ziemi, 1- leci w górê, 2- spada
	int deley;		//opóŸnienie tekstury chodzenia

public:
	NPC(float x, float y, std::string _texture);	

	sf::Sprite getSprite();	//zwraca sprite
	void setPosition(sf::Vector2f position);
	bool move(float x, float y, Map &m);	//motoda która odpowiada za poruszanie siê (sprawdza m. in. kolizje )
	void Draw(sf::RenderWindow *window);	//metoda do rysowania npc
	void gravity(Map &m, float velocity);	//grawitacja npc korzystaj¹ca z move i odpowiedzialna za skakanie/opadanie
	void jump(float HowHigh, bool force);	//metoda która zmienia wartoœci Jump/height/canjummp w zale¿noœci czy mo¿e czy nie skakaæ
	void animationComponent();

	sf::Vector2f getPosition();	//zwaraca aktualn¹ pozycjê
	int getMoveState();
	void setMoveState(int state);
	void setDeley(int _deley);
};

#endif // !NPC_h
