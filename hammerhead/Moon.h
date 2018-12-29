#pragma once
#include "CelestialBody.h"

class Moon : public CelestialBody {
public:
	Moon(Coord pos, Vector2 direction, float radius) :
		CelestialBody(pos, direction, radius) {
	}
};