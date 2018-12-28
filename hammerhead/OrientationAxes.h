#pragma once

#include "Camera.h"
#include <glm/mat4x4.hpp>
#include <GL/gl3w.h>
#include <GL/glcorearb.h>

enum AxisLocation { TOP_RIGHT, TOP_LEFT, BOTTOM_RIGHT, BOTTOM_LEFT };

class OrientationAxes {
public:
	OrientationAxes() {
		this->axis_location = AxisLocation::BOTTOM_LEFT;
		this->position_padding = 16.0f;
	}

	void set_position_padding(float value) {
		this->position_padding = value;
	}

	void set_camera(Camera value) {
		this->camera = value;
	}

	void update() {
		float axis_size = 48.0f;
		float x_position = 0.0f;
		float y_position = 0.0f;
		float z_position = 0.0f;
		float x_offset = 0.0f;
		float y_offset = 0.0f;

		switch (this->axis_location) {
		case BOTTOM_LEFT:
			x_offset = this->position_padding;
			y_offset = -1 * (axis_size + this->position_padding);
			break;
		case BOTTOM_RIGHT:
			x_offset = -1 * (axis_size + this->position_padding);
			y_offset = -1 * (axis_size + this->position_padding);
			break;
		case TOP_LEFT:
			x_offset = this->position_padding;
			y_offset = this->position_padding;
			break;
		case TOP_RIGHT:
			x_offset = -1 * (axis_size + this->position_padding);
			y_offset = this->position_padding;
			break;
		}


		z_position = this->camera.get_position().z;

		this->position = glm::vec3(x_position, y_position, z_position);
	}

	void draw() {
		//Draw x,y,z axes 

	}

private:
	float position_padding;
	Camera camera;
	glm::vec3 position;
	AxisLocation axis_location;
};