#pragma once

#include "GameObject.h"

class SpaceObject : public GameObject {
public:
	SpaceObject(Coord pos, Coord center, Vector2 direction, Vector2 size) :
		GameObject(pos, center, direction, size) {
	}
};
