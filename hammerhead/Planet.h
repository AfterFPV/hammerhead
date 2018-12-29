#pragma once
#include "CelestialBody.h"

class Planet : public CelestialBody {
public:
	Planet(Coord pos, Vector2 direction, float radius) :
		CelestialBody(pos, direction, radius) {
	}
};