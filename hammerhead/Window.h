#pragma once

#include "GameObject.h"

using namespace std;

class Window : public GameObject {
private:
	string title;
protected:
public:
	Window(Coord pos, Vector2 size, string title) :
		GameObject(pos, center, direction, size) {
	}
};
