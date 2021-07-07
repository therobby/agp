#include "EventChecker.h"
//tego jeszce nie opisuje bo jest work in progress
EventChecker::EventChecker() {
	time = respawn = Texturenumber = tempTexturenumber = x = y = 0;
	m = NULL;
}

void EventChecker::add(Map *_m, int _Texturenumber, int _tempTexturenumber, int _time) {
	m = _m;
	Texturenumber = _Texturenumber;
	tempTexturenumber = _tempTexturenumber;
	time = _time;
	m->ChangeSprite(x, y, tempTexturenumber);
}

void EventChecker::add(Map *_m, int _Texturenumber, int _tempTexturenumber, int _time, int _respawn) {
	m = _m;
	Texturenumber = _Texturenumber;
	tempTexturenumber = _tempTexturenumber;
	time = _time;
	respawn = _respawn;
	m->ChangeSprite(x, y, tempTexturenumber);
}

bool EventChecker::working() {
	if (time > 0) {
		time--;
		return true;
	}
	else if (respawn != 0) {
		respawn--;
	}
	else {
		m->ChangeSprite(x, y, Texturenumber);
		time = respawn = Texturenumber = tempTexturenumber = x = y = 0;
		m = NULL;
		return false;
	}
	return false;
}