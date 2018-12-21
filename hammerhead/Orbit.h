#pragma once
#include "SpaceObject.h"
#include "Color.h"
#include "GlPrimatives.h"

class SpaceObject;

class Orbit {
private:
	float phase;
	float phase_step;
	float speed;
	float radius;
	float abs_center_x;
	float abs_center_y;
	float x;
	float y;
	int steps_per_rotation = 1000;
	SpaceObject* body;
	vector<DrawObject> draw_objects;
public:
	Orbit() : speed(0), radius(0), body(nullptr) {

	}

	Orbit(SpaceObject* body, float speed) : body(body), speed(speed), radius(0) {
	}

	float get_speed() {
		return this->speed;
	}

	float get_radius() {
		return this->radius;
	}

	void set_radius(float value) {
		this->radius = value;
	}

	SpaceObject* get_body() {
		return this->body;
	}

	float get_phase() {
		return this->phase;
	}

	void calculate_phase_step() {
		if (speed == 0) {
			phase_step = 0;
		}
		else {
			phase_step = speed / steps_per_rotation;
		}
	}


	float get_x() {
		return this->x;
	}

	float get_y() {
		return this->y;
	}

	float get_abs_center_x() {
		return this->abs_center_x;
	}

	float get_abs_center_y() {
		return this->abs_center_y;
	}

	const vector<DrawObject>& get_draw_objects() const {
		return this->draw_objects;
	}

	void register_draw_objects();
	void update_phase();
	void calculate_position();
	void set_phase(float value);
};




