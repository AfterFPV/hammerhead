#pragma once

#pragma once
#include "CelestialBody.h"

class Sun : public CelestialBody {
public:
	Sun(Coord pos, Vector2 direction, float radius) :
		CelestialBody(pos, direction, radius) {
	}
};