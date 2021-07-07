#pragma once
#ifndef EventChecker_h
#define EventChecker_h

#include "Map.h"
//czytaj info w .cpp
class EventChecker {
	int time;
	int respawn;
	int Texturenumber;
	int tempTexturenumber;
	int x;
	int y;
	Map *m;

public:
	EventChecker();

	void add(Map *m, int _Texturenumber, int _tempTexturenumber, int _time);
	void add(Map *m, int _Texturenumber, int _tempTexturenumber, int _time, int _respawn);
	bool working();

};

#endif // !EventChecker_h
