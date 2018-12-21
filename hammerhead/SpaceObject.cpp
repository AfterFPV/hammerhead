#include <math.h>
#include "SpaceObject.h"
#include "SdlPrimatives.h"
#include "GlPrimatives.h"


Orbit* SpaceObject::get_orbit() {
	return this->orbit;
}

void  SpaceObject::set_orbit(Orbit* orbit) {
	this->orbit = orbit;

	calculate_orbit_radius();
	calculate_orbit_phase();
	register_orbit_draw_objects();
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

void SpaceObject::register_orbit_draw_objects() {
	this->orbit->register_draw_objects();
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
	draw_orbit();
	GameObject::draw();
}

void SpaceObject::draw_orbit() {
	if (is_in_orbit()) {		
		int orbit_x = this->orbit->get_body()->get_center().get_intX();
		int orbit_y = this->orbit->get_body()->get_center().get_intY();
		int orbit_radius = this->orbit->get_radius();

		for (size_t i = 0; i < this->orbit->get_draw_objects().size(); i++) {
			DrawObject o = this->orbit->get_draw_objects()[i];
			if (o.vao_id < 1) {
				continue;
			}

			//glm::vec3 scaling_factor = this->model->get_normalized_scaling_factor();

			glBindVertexArray(o.vao_id);
			//glBindBuffer(GL_ARRAY_BUFFER, o.vao_id);

			glm::mat4 identity_matrix = glm::mat4(1.0);
			glm::mat4 model_matrix;
			glm::vec3 x_axis(1, 0, 0);
			glm::vec3 y_axis(0, 1, 0);
			glm::vec3 z_axis(0, 0, 1);

			//float scaling_factor_x = this->orbit->get_radius();
			//float scaling_factor_y = this->orbit->get_radius();
			//float scaling_factor_z = scaling_factor_x;

			//Primitive objects should already be scaled to view
			float scaling_factor_x = 1.0f;
			float scaling_factor_y = 1.0f;
			float scaling_factor_z = 1.0f;

			glm::vec3 scaling_vector(scaling_factor_x, scaling_factor_y, scaling_factor_z);
			glm::mat4 scaling_matrix = glm::scale(identity_matrix, scaling_vector);

			float z_pos_3d = this->pos.get_floatY();
			glm::vec3 translation_vector(this->pos.get_floatX(), -250, z_pos_3d);
			glm::mat4 translation_matrix = glm::translate(identity_matrix, translation_vector);

			//float x_angle_radians = this->rotation_position.x;
			//float y_angle_radians = this->rotation_position.y;
			//float z_angle_radians = this->rotation_position.z;
			float x_angle_radians = 0.0f;
			float y_angle_radians = 0.0f;
			float z_angle_radians = 0.0f;


			glm::mat4 rotation_matrix = glm::rotate(identity_matrix, x_angle_radians, x_axis);
			rotation_matrix = glm::rotate(rotation_matrix, y_angle_radians, y_axis);
			rotation_matrix = glm::rotate(rotation_matrix, z_angle_radians, z_axis);

			model_matrix = translation_matrix * rotation_matrix * scaling_matrix;

			glm::mat4 mvp = this->projection * this->view * model_matrix;
			glUniformMatrix4fv(glGetUniformLocation(this->shader_program, "mvpmatrix"), 1, GL_FALSE, glm::value_ptr(mvp));

			glDrawArrays(GL_TRIANGLES, 0, o.num_triangles);
		}
	}
}