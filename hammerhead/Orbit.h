#pragma once
#include "SpaceObject.h"

class SpaceObject;

class Orbit {
private:
	float speed;
	SpaceObject* body;
public:
	Orbit() : speed(0) {

	}

	Orbit(SpaceObject* body, float speed) : body(body), speed(speed) {
	}

	bool is_in_orbit() {
		return (this->body != nullptr);
	}

	float get_speed() {
		return this->speed;
	}

	SpaceObject* get_body() {
		return this->body;
	}
};

