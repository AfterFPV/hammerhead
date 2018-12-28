#pragma once

#include <vector>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace std;

class Camera {
public:
	Camera() {
		this->field_of_view = 45.0f;
		this->aspect_width = 1280.0f;
		this->aspect_height = 720.0f;
		this->clip_near = 0.1f;
		this->clip_far = 1000.0f;
		this->zoom = 1.0f;
		this->zoom_factor = 1.0f;
		this->position = glm::vec3(0, 900, 0); //Camera position in World Space
		this->look_at = glm::vec3(0, 0, 0); //and looks at the origin
		this->coord_system_orientation = glm::vec3(0, 1, 1);  // Head is up (set to 0,-1,0 to look upside-down)

		this->zoom_levels.push_back(0.27f);
		this->zoom_levels.push_back(0.3333f);
		this->zoom_levels.push_back(0.5f);
		this->zoom_levels.push_back(0.6667f);
		this->zoom_levels.push_back(0.75f);
		this->zoom_levels.push_back(0.9f);
		this->zoom_levels.push_back(1.0f);
		this->zoom_levels.push_back(1.25f);
		this->zoom_levels.push_back(1.5f);
		this->zoom_levels.push_back(1.75f);
		this->zoom_levels.push_back(2.0f);
		this->zoom_levels.push_back(3.0f);
		this->zoom_levels.push_back(4.0f);
		this->zoom_levels.push_back(5.0f);
		this->zoom_levels.push_back(6.0f);
		this->zoom_levels.push_back(8.0f);
		this->zoom_levels.push_back(12.0f);
		this->zoom_levels.push_back(16.0f);
		this->zoom_levels.push_back(24.0f);
		this->zoom_levels.push_back(32.0f);

		this->zoom_index = 6;
	}

	void set_aspect_width(float value) {
		this->aspect_width = value;
	}

	void set_aspect_height(float value) {
		this->aspect_height = value;
	}

	void set_clip_near(float value) {
		this->clip_near = value;
	}

	void set_clip_far(float value) {
		this->clip_far = value;
	}

	void set_zoom(float value) {
		this->zoom = value;
		this->zoom_factor = 1.0f / this->zoom;
	}

	void set_position(glm::vec3 value) {
		this->position = value;
	}

	void zoom_in() {
		if (this->zoom_index < this->zoom_levels.size() - 2) {
			this->zoom_index++;
			float zoom_index_value = this->zoom_levels[this->zoom_index];
			this->set_zoom(zoom_index_value);
		}
	}

	void zoom_out() {
		if (this->zoom_index > 0) {
			this->zoom_index--;
			float zoom_index_value = this->zoom_levels[this->zoom_index];
			this->set_zoom(zoom_index_value);
		}
	}

	void set_look_at(glm::vec3 value) {
		this->look_at = value;
	}

	void set_coord_system_orientation(glm::vec3 value) {
		this->coord_system_orientation = value;
	}

	glm::vec3 get_position() {
		return this->position;
	}

	glm::vec3 get_look_at() {
		return this->look_at;
	}

	glm::vec3 get_coord_system_orientation() {
		return this->coord_system_orientation;
	}

	float get_clip_near() {
		return this->clip_near;
	}

	float get_clip_far() {
		return this->clip_far;
	}

	float get_zoom() {
		return this->zoom;
	}

	void calculate_matrices() {
		// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
		float fov_radians = glm::radians(this->field_of_view);
		float aspect_ratio = 1280.0f / 720.0f;
		this->projection = glm::perspective(fov_radians * this->zoom_factor, aspect_ratio, this->clip_near, this->clip_far);

		//glm::vec3 zoom_position

		// Camera matrix
		this->view = glm::lookAt(
			this->position, 
			this->look_at,
			this->coord_system_orientation
		);
	}

	glm::mat4 get_view() {
		return this->view;
	}

	glm::mat4 get_projection() {
		return this->projection;
	}
private:
	int zoom_index;
	float aspect_width;
	float aspect_height;
	float aspect_ratio;
	float clip_near;
	float clip_far;
	float field_of_view;
	float zoom;
	float zoom_factor;
	glm::mat4 projection;
	glm::mat4 view;
	glm::vec3 position;
	glm::vec3 look_at;
	glm::vec3 coord_system_orientation;
	vector<float> zoom_levels;
};