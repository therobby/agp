#pragma once
#ifndef Play_h
#define Play_h
#include "includes.h"

class Map {
	int *M;	//zbiór wartoœci textur na mapie
	int x, y;	//rozmiar mapy
	int *Collide;	//zbiór bloków które kooliduj¹
	int CollideSize;
	sf::Sprite *map;	//tablica mapy
	sf::Texture *textures;	//tablica zawieraj¹ca textury
	int texturessize;
	int texturesHeight;
	int texturesWidth;
	std::string MapName;

	void getXY(int &x, int &y, std::ifstream *file);	//metoda zliczaj¹ca rozmiary mapy na podstawie macierzy mapy w pliku txt
	void getX(int &x, std::ifstream *file);
	void getY(int &y, std::ifstream *file);
public:
	Map(std::string _MapName, int _texturesHeight, int _texturesWidth);
	~Map();
	void load(std::string _MapName, int _texturesHeight, int _texturesWidth, bool firstLoad);
	void Draw(sf::RenderWindow *window);	
	bool collision(sf::Sprite npc);	//sprawdz kolizjê z granicami mapy (obiektami ustalonymi w metodzie)
	bool collision(sf::Sprite npc, int object);	//sprawdz kolizjê z danym obiektem na mapie
	void ChangeSprite(int x, int y, int texturenumber);	//zmieñ texture sprita na mapie

	sf::Vector2f getMapLastPosition();	//nie mam pomys³u na nazwê, ale zwraca wspó³rzêdne dolnego prawego rogu
	std::string getMapName();
	sf::Vector2f getRespawn();
};

#endif // !Play_h
