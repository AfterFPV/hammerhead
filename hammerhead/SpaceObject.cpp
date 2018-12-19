#include <math.h>
#include "SpaceObject.h"
#include "SdlPrimatives.h"


Orbit* SpaceObject::get_orbit() {
	return this->orbit;
}

void  SpaceObject::set_orbit(Orbit* orbit) {
	this->orbit = orbit;

	calculate_orbit_radius();
	calculate_orbit_phase();
}

void SpaceObject::calculate_orbit_phase() {
	if (is_in_orbit()) {
		float phase = 0;

		float delta_x = this->center.get_floatX() - this->orbit->get_body()->center.get_floatX();
		float delta_y = this->center.get_floatY() - this->orbit->get_body()->center.get_floatY();

		phase = atan(delta_y / delta_x);
		this->orbit->set_phase(phase);
	}
}


bool SpaceObject::is_in_orbit() {
	if (this->orbit == nullptr) {
		return false;
	}
	else {
		return (this->orbit->get_body() != nullptr);
	}
}

void SpaceObject::update_orbit() {
	if (is_in_orbit()) {
		this->orbit->update_phase();

		float center_offset_x = this->center.get_floatX() - this->pos.get_floatX();
		float center_offset_y = this->center.get_floatY() - this->pos.get_floatY();

		this->center = Coord(this->orbit->get_abs_center_x(), this->orbit->get_abs_center_y());
		this->pos = Coord(this->center.get_floatX() - center_offset_x, this->center.get_floatY() - center_offset_y);
	}
}


void SpaceObject::calculate_orbit_radius() {
	if (is_in_orbit()) {
		float delta_x = abs(this->center.get_floatX() - this->orbit->get_body()->center.get_floatX());
		float delta_y = abs(this->center.get_floatY() - this->orbit->get_body()->center.get_floatY());

		float orbit_radius = sqrt((delta_x * delta_x) + (delta_y * delta_y));
		this->orbit->set_radius(orbit_radius);
	}
}

void SpaceObject::draw() {
	//draw_orbit();
	GameObject::draw();
}

void SpaceObject::draw_orbit() {
	if (is_in_orbit()) {
		/*
		int orbit_x = this->orbit->get_body()->get_center().get_intX();
		int orbit_y = this->orbit->get_body()->get_center().get_intY();
		int orbit_radius = this->orbit->get_radius();

		SdlPrimatives::draw_circle(this->renderer, orbit_x, orbit_y, orbit_radius, this->orbit_color);	
		*/
	}
}