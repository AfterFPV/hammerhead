#pragma once
#include "GameObject.h"

class AstralObject : public GameObject {
private:
	int attack() {

	}
protected:
public:
	AstralObject(Coord pos, Coord center, float radius, Vector2 direction, float size, int dmg, int range) :
		GameObject(pos, center, radius, direction, size, dmg, range) {
	}
};