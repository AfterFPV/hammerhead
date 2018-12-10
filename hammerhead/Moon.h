#pragma once
#include "CelestialBody.h"

class Moon : public CelestialBody {
public:
	Moon(Coord pos, Coord center, Vector2 direction, float radius, float dmg, float range) :
		CelestialBody(pos, center, direction, radius, dmg, range) {
	}
};