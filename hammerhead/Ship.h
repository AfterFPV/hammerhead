#pragma once
#include "SpaceObject.h"

class Ship : public SpaceObject {
private:
	float damage;
	float range;
protected:
public:
	Ship(Coord pos, Coord center, Vector2 direction, Vector2 size, float damage, float range) :
		SpaceObject(pos, center, direction, size), damage(damage), range(range) {
	}

	float get_damage();
	float get_range();
};