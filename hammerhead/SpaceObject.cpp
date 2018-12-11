#include "SpaceObject.h"
#include "SdlPrimatives.h"


Orbit SpaceObject::get_orbit() {
	return this->orbit;
}

void  SpaceObject::set_orbit(Orbit orbit) {
	this->orbit = orbit;
}

void SpaceObject::draw() {
	draw_orbit();
	GameObject::draw();
}

void SpaceObject::draw_orbit() {
	if (this->orbit.is_in_orbit()) {
		//int orbit_x = this->orbit.get_body()->get_center().get_intX();
		//int orbit_y = this->orbit.get_body()->get_center().get_intY();
		//int orbit_radius = 50;

		//int orbit_x = 176;
		//int orbit_y = 176;
		//int orbit_radius = 100;

		int orbit_x = 176;
		int orbit_y = 176;
		int orbit_radius = 100;

		SdlPrimatives::draw_circle(this->renderer, orbit_x, orbit_y, orbit_radius);
	}
}