#pragma once
#include "SpaceObject.h"

class CelestialBody : public SpaceObject {
private:
	float radius;
	float atmosphere_radius;
	float mass;
protected:
public:
	CelestialBody(Coord pos, Coord center, Vector2 direction, float radius) :
		SpaceObject(pos, center, direction, Vector2(radius * 2, radius * 2)), radius(radius) {
	}

	void set_atmosphere_radius(float value);
	void set_mass(float value);
	float get_mass();
	float get_radius();
	float get_atmosphere_radius();
};