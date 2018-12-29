#include <math.h>
#include <SDL2/SDL.h>
#include "SpaceObject.h"
#include "GlPrimatives.h"


Orbit* SpaceObject::get_orbit() {
	return this->orbit;
}

Coord SpaceObject::offset_position(Coord center, Vector2 size)
{
	//return Coord(center.get_floatX() + (size.x * 0.5f), center.get_floatY() + (size.y * 0.5f));
	return Coord(center.get_floatX(), center.get_floatY());
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

		this->center = Coord(this->orbit->get_abs_x(), this->orbit->get_abs_y());
		this->pos = offset_position(this->center, this->size);
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

		for (size_t i = 0; i < this->orbit->get_draw_objects().size(); i++) {
			DrawObject o = this->orbit->get_draw_objects()[i];
			if (o.vao_id < 1) {
				continue;
			}

			glBindVertexArray(o.vao_id);

			glm::mat4 identity_matrix = glm::mat4(1.0);
			glm::mat4 model_matrix;
			glm::vec3 x_axis(1, 0, 0);
			glm::vec3 y_axis(0, 1, 0);
			glm::vec3 z_axis(0, 0, 1);

			//Primitive objects should already be scaled to view
			float scaling_factor_x = 1.0f;
			float scaling_factor_y = 1.0f;
			float scaling_factor_z = 1.0f;

			glm::vec3 scaling_vector(scaling_factor_x, scaling_factor_y, scaling_factor_z);
			glm::mat4 scaling_matrix = glm::scale(identity_matrix, scaling_vector);
			
			Coord body_pos = this->orbit->get_body()->pos;

			glm::vec3 translation_vector(body_pos.get_floatX(), body_pos.get_floatY(), 0);
			glm::mat4 translation_matrix = glm::translate(identity_matrix, translation_vector);

			glm::vec3 body_rotation = this->orbit->get_body()->get_rotation();
			float x_angle_radians = body_rotation.x;
			float y_angle_radians = body_rotation.y;
			float z_angle_radians = body_rotation.z;
			
			glm::mat4 rotation_matrix = glm::rotate(identity_matrix, x_angle_radians, x_axis);
			rotation_matrix = glm::rotate(rotation_matrix, y_angle_radians, y_axis);
			rotation_matrix = glm::rotate(rotation_matrix, z_angle_radians, z_axis);
			

			model_matrix = translation_matrix * rotation_matrix * scaling_matrix;

			glm::mat4 mvp = this->projection * this->view * model_matrix;
			glUniformMatrix4fv(glGetUniformLocation(this->shader_program, "mvpmatrix"), 1, GL_FALSE, glm::value_ptr(mvp));

			glEnableVertexAttribArray(0);
			glBindBuffer(GL_ARRAY_BUFFER, o.position_vbo);

			glEnableVertexAttribArray(1);
			glBindBuffer(GL_ARRAY_BUFFER, o.color_vbo);

			glDrawArrays(GL_TRIANGLES, 0, o.num_triangles);
		}
	}
}