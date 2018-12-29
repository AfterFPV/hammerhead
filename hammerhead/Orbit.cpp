#include "Orbit.h"


void Orbit::calculate_position() {
	this->x = this->radius * cos(this->phase);
	this->y = this->radius * sin(this->phase);

	this->abs_x = this->x + this->body->get_center().get_floatX();
	this->abs_y = this->y + this->body->get_center().get_floatY();
}


void Orbit::set_phase(float value) {
	this->phase = value;

	calculate_position();
	calculate_phase_step();
}

void Orbit::update_phase() {
	this->phase += this->phase_step;

	calculate_position();
}

void Orbit::register_draw_objects() {
	Color orbit_color(255, 200, 0);

	int line_segments = 256;
	float line_width = 4.0f;
	float circle_radius = this->radius; //circle uses radius of 1. Will scale to set radius at draw time
	DrawObject o = GlPrimatives::register_circle(0.0f, 0.0f, circle_radius, line_width, orbit_color, line_segments);

	this->draw_objects.push_back(o);
}