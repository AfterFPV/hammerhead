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