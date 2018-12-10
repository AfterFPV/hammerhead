#pragma once
#include "SpaceObject.h"

class CelestialBody : public SpaceObject {
private:
	float radius;
protected:
public:
	CelestialBody(Coord pos, Coord center, Vector2 direction, float radius) :
		SpaceObject(pos, center, direction, Vector2(radius * 2, radius * 2)) {
	}

	float get_radius();
};