#pragma once

#include "GameObject.h"
#include "Orbit.h"
#include "Color.h"

class Orbit;

class SpaceObject : public GameObject {
private:
	Orbit* orbit;
	Color orbit_color;
	void calculate_orbit_phase();
	void calculate_orbit_radius();
public:
	SpaceObject(Coord center, Vector2 direction, Vector2 size) :
		GameObject(offset_position(center, size), center, direction, size), orbit_color(Color(236, 205, 53)) {
	}

	SpaceObject(Coord center, Vector2 direction, Vector2 size, Orbit* orbit) :
		GameObject(offset_position(center, size), center, direction, size), orbit(orbit), orbit_color(Color(236, 205, 53)) {
		this->orbit_color = Color(236, 205, 53);
	}
	
	Orbit* get_orbit();
	Coord offset_position(Coord center, Vector2 size);
	void set_orbit(Orbit* orbit);	
	void draw() override;
	void draw_orbit();
	bool is_in_orbit();
	void update_orbit();
	void register_orbit_draw_objects();
};
