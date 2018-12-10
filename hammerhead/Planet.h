#pragma once
#include "AstralObject.h"

class Planet : public AstralObject {
public:
	Planet(Coord pos, Coord center, float radius, Vector2 direction, float size, int dmg, int range) :
		AstralObject(pos, center, radius, direction, size, dmg, range) {
	}
};