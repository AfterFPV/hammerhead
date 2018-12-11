#pragma once

#pragma once
#include "CelestialBody.h"

class Sun : public CelestialBody {
public:
	Sun(Coord pos, Coord center, Vector2 direction, float radius) :
		CelestialBody(pos, center, direction, radius) {
	}
};