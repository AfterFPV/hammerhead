#include "CelestialBody.h"

float CelestialBody::get_mass() {
	return mass;
}

float CelestialBody::get_radius() {
	return radius;
}

float CelestialBody::get_atmosphere_radius() {
	return this->atmosphere_radius;
}

void CelestialBody::set_atmosphere_radius(float value) {
	this->atmosphere_radius = value;
}

void CelestialBody::set_mass(float value) {
	this->mass = value;
}
