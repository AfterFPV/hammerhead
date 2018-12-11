#pragma once

#include "GameObject.h"
#include "Orbit.h"

class SpaceObject : public GameObject {
private:
	Orbit* orbit;
public:
	SpaceObject(Coord pos, Coord center, Vector2 direction, Vector2 size) :
		GameObject(pos, center, direction, size) {
	}

	SpaceObject(Coord pos, Coord center, Vector2 direction, Vector2 size, Orbit* orbit) :
		GameObject(pos, center, direction, size), orbit(orbit) {
	}
	
	Orbit* get_orbit();
	void set_orbit(Orbit* orbit);	
	void draw() override;
	void draw_orbit();
};
