#include "GameObject.h"

Coord GameObject::get_position() {
	return pos;
}

void GameObject::update_position() {
	pos = Coord(pos.get_floatX() + direction.x, pos.get_floatY() + direction.y);
	center = Coord(center.get_floatX() + direction.x, center.get_floatY() + direction.y);
}

Vector2 GameObject::get_direction() {
	return direction;
}

Coord GameObject::get_center() {
	return center;
}

Vector2 GameObject::get_size() {
	return size;
}

void GameObject::set_direction(Vector2 direction) {
	this->direction = direction;

	calculate_orientation();
}

void GameObject::set_matrices(glm::mat4 projection, glm::mat4 view) {
	this->projection = projection;
	this->view = view;
}

void GameObject::set_shader_program(GLuint value) {
	this->shader_program = value;
}

void GameObject::draw() {
	for (size_t i = 0; i < this->model->get_draw_objects().size(); i++) {
		DrawObject o = this->model->get_draw_objects()[i];
		if (o.vao_id < 1) {
			continue;
		}

		glBindBuffer(GL_ARRAY_BUFFER, o.vao_id);

		glm::mat4 identity_matrix = glm::mat4(1.0);
		glm::mat4 model_matrix;
		glm::vec3 x_axis(1, 0, 0);
		glm::vec3 y_axis(0, 1, 0);
		glm::vec3 z_axis(0, 0, 1);

		float scaling_factor_x = 0.001 * this->size.x;
		float scaling_factor_y = 0.001 * this->size.y;
		float scaling_factor_z = scaling_factor_x;

		glm::vec3 scaling_vector(scaling_factor_x, scaling_factor_y, scaling_factor_z);
		glm::mat4 scaling_matrix = glm::scale(identity_matrix, scaling_vector);

		glm::vec3 translation_vector(this->pos.get_floatX() * 0.01, this->pos.get_floatY() * 0.01, 0);
		glm::mat4 translation_matrix = glm::translate(identity_matrix, translation_vector);

		float x_angle_radians = this->rotation.x;
		float y_angle_radians = this->rotation.y;
		float z_angle_radians = this->rotation.z;

		glm::mat4 rotation_matrix = glm::rotate(identity_matrix, x_angle_radians, x_axis);
		rotation_matrix = glm::rotate(rotation_matrix, y_angle_radians, y_axis);
		rotation_matrix = glm::rotate(rotation_matrix, z_angle_radians, z_axis);

		model_matrix = translation_matrix * rotation_matrix * scaling_matrix;

		glm::mat4 mvp = this->projection * this->view * model_matrix;
		glUniformMatrix4fv(glGetUniformLocation(this->shader_program, "mvpmatrix"), 1, GL_FALSE, glm::value_ptr(mvp));


		glDrawArrays(GL_TRIANGLES, 0, o.num_triangles);
	}
}

/*
void GameObject::draw() {
	glm::mat4 identity_matrix = glm::mat4(1.0);
	glm::mat4 model_matrix;
	glm::vec3 x_axis(1, 0, 0);
	glm::vec3 y_axis(0, 1, 0);
	glm::vec3 z_axis(0, 0, 1);

	float scaling_factor_x = 0.01 * this->size.x;
	float scaling_factor_y = 0.01 * this->size.y;
	float scaling_factor_z = scaling_factor_x;

	glm::vec3 scaling_vector(scaling_factor_x, scaling_factor_y, scaling_factor_z);
	glm::mat4 scaling_matrix = glm::scale(identity_matrix, scaling_vector);

	glm::vec3 translation_vector(this->pos.get_floatX() * 0.01, this->pos.get_floatY() * 0.01, 0);
	glm::mat4 translation_matrix = glm::translate(identity_matrix, translation_vector);

	float x_angle_radians = this->rotation.x;
	float y_angle_radians = this->rotation.y;
	float z_angle_radians = this->rotation.z;

	glm::mat4 rotation_matrix = glm::rotate(identity_matrix, x_angle_radians, x_axis);
	rotation_matrix = glm::rotate(rotation_matrix, y_angle_radians, y_axis);
	rotation_matrix = glm::rotate(rotation_matrix, z_angle_radians, z_axis);

	model_matrix = translation_matrix * rotation_matrix * scaling_matrix;

	glm::mat4 mvp = this->projection * this->view * model_matrix;
	glUniformMatrix4fv(glGetUniformLocation(this->shader_program, "mvpmatrix"), 1, GL_FALSE, glm::value_ptr(mvp));


	glDrawArrays(GL_TRIANGLES, 0, 12 * 3);
}
*/

/*
void GameObject::draw() {
	glm::mat4 identity_matrix = glm::mat4(1.0);
	glm::mat4 model_matrix;
	glm::vec3 x_axis(1, 0, 0);
	glm::vec3 y_axis(0, 1, 0);
	glm::vec3 z_axis(0, 0, 1);

	float x_angle_radians = this->rotation.x;
	float y_angle_radians = this->rotation.y;
	float z_angle_radians = this->rotation.z;

	model_matrix = glm::rotate(identity_matrix, x_angle_radians, x_axis);
	model_matrix = glm::rotate(model_matrix, y_angle_radians, y_axis);
	model_matrix = glm::rotate(model_matrix, z_angle_radians, z_axis);

	glm::vec3 translation_vector(this->pos.get_floatX() * 0.01, this->pos.get_floatY() * 0.01, 0);

	float scaling_factor_x = 0.01 * this->size.x;
	float scaling_factor_y = 0.01 * this->size.y;
	float scaling_factor_z = scaling_factor_x;

	glm::vec3 scaling_vector(scaling_factor_x, scaling_factor_y, scaling_factor_z);
	//model_matrix = glm::scale(model_matrix, scaling_vector);
	model_matrix = glm::translate(model_matrix, translation_vector);

	glm::mat4 mvp = this->projection * this->view * model_matrix;
	glUniformMatrix4fv(glGetUniformLocation(this->shader_program, "mvpmatrix"), 1, GL_FALSE, glm::value_ptr(mvp));


	glDrawArrays(GL_TRIANGLES, 0, 12 * 3);
}*/

/*
void GameObject::draw() {
	list<GameTexture>::iterator it;

	for (it = textures.begin(); it != textures.end(); it++) {
		SDL_Rect display_rect = { this->pos.get_intX() + it->get_relative_pos().get_intX(), this->pos.get_intY() + it->get_relative_pos().get_intY(), it->get_width(), it->get_height() };
		SDL_SetTextureColorMod(it->get_texture(), it->get_color().r, it->get_color().g, it->get_color().b);
		
		float angle = this->orientation_degrees + ASSET_ROTATION_OFFSET;
		SDL_RenderCopyEx(renderer, it->get_texture(), NULL, &display_rect, angle, NULL, SDL_FLIP_NONE);
	}
}*/

/*
void GameObject::set_renderer(SDL_Renderer* value) {
	this->renderer = value;
}*/

void GameObject::set_model(Model* model) {
	this->model = model;
}

void GameObject::set_name(string name) {
	this->name = name;
}

string GameObject::get_name() {
	return this->name;
}

void GameObject::calculate_orientation() {
	this->orientation = atan2(0.5, 0.5);
	this->orientation_degrees = this->orientation * 180 / M_PI;
}

bool GameObject::hit_test(Coord location) {
	if (this->is_clickable) {

	}

	return false;
}

void GameObject::set_z_index(int value) {
	this->z_index = value;
}

int GameObject::get_z_index() {
	return this->z_index;
}
