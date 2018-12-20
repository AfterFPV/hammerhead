#pragma once
#include "CelestialBody.h"

class Planet : public CelestialBody {
public:
	Planet(Coord pos, Coord center, Vector2 direction, float radius) :
		CelestialBody(pos, center, direction, radius) {
	}
};