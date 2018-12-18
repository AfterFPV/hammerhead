#pragma once

#include "Space.h"

Space::Space() :
	Space(MIN_SPACE_SIZE, MIN_SPACE_SIZE) {
}

Space::Space(int column_count, int row_count) {
	this->tick_count = 0;
	this->column_count = MIN_SPACE_SIZE;
	this->row_count = MIN_SPACE_SIZE;

	if ((column_count > MIN_SPACE_SIZE) && (column_count < MAX_SPACE_SIZE)) {
		this->column_count = column_count;
	}

	if ((row_count > MIN_SPACE_SIZE) && (row_count < MAX_SPACE_SIZE)) {
		this->row_count = row_count;
	}

	init_map();
	blank_map();
}


void Space::init_map() {
	two_d = new char*[this->column_count];

	for (int i = 0; i < this->column_count; i++) {
		two_d[i] = new char[this->row_count];
	}
}

void Space::tick() {
	update_map();

	this->tick_count++;
}

int Space::get_tick_count() {
	return this->tick_count;
}

void Space::add_object(unique_ptr<SpaceObject> obj) {
	obj->set_renderer(this->renderer);
	objs.push_back(move(obj));
}

void Space::set_renderer(SDL_Renderer* value) {
	this->renderer = value;
}

void Space::set_window_size(int width, int height) {
	this->window_width = width;
	this->window_height = height;

	this->tile_width = width / this->column_count;
	this->tile_height = height / this->row_count;
}

void Space::draw_grid() {
	int posX = 0;
	int posY = 0;

	for (int i = 0; i < this->column_count; i++) {
		posY = i * this->tile_height;

		for (int j = 0; j < this->row_count; j++) {
			posX = j * this->tile_width;

			SDL_Rect tile;
			tile.h = tile_height;
			tile.w = tile_width;
			tile.x = posX;
			tile.y = posY;

			SDL_RenderDrawRect(renderer, &tile);
		}
	}
}

void Space::set_matrices(glm::mat4 projection, glm::mat4 view) {
	this->projection = projection;
	this->view = view;
}

void Space::set_shader_program(GLuint value) {
	this->shader_program = value;
}

void Space::draw_map() {
	//Clear draw area
	//SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	//SDL_RenderClear(renderer);

	//SDL_SetRenderDrawColor(renderer, 44, 44, 44, 255);

	//draw_grid();	

	//Draw objects in space
	list<unique_ptr<SpaceObject>>::iterator it;
	for (it = objs.begin(); it != objs.end(); it++) {
		it->get()->set_matrices(this->projection, this->view);
		it->get()->set_shader_program(this->shader_program);
		it->get()->draw();
	}

	// Render the changes above
	//SDL_RenderPresent(renderer);
}

void Space::print_list() {
	list<unique_ptr<SpaceObject>>::iterator it;
	int x, y;

	for (it = objs.begin(); it != objs.end(); it++) {

		SpaceObject* obj = it->get();
		x = (int)(obj->get_position().get_floatX() + obj->get_direction().x);
		y = (int)(obj->get_position().get_floatY() + obj->get_direction().y);

		cout << "x = " << x << ", y = " << y << endl;
	}
}

void Space::blank_map() {
	for (int i = 0; i < this->column_count; i++) {
		for (int j = 0; j < this->row_count; j++) {
			two_d[i][j] = '.';
		}
	}
}

void Space::update_map() {
	list<unique_ptr<SpaceObject>>::iterator it;

	for (it = objs.begin(); it != objs.end(); it++) {
		it->get()->update_orbit();
	}

	for (it = objs.begin(); it != objs.end(); it++) {
		it->get()->update_position();
	}
}

void Space::draw_selected() {
	if (this->selected_object != nullptr) {

	}
}

void Space::update_orbit(CelestialBody &obj) {

	float radius;
	const float PI = 3.14159;
	Coord center = obj.get_center();
	int x, y;
	float cent_x, cent_y;
	float tmp_x, tmp_y;

	cent_x = center.get_floatX();
	cent_y = center.get_floatY();
	radius = obj.get_radius();

	for (float angle = 0; angle <= 2 * PI; angle += 0.01) {

		tmp_x = cent_x + (radius * cos(angle));
		tmp_y = cent_y + (radius * sin(angle));

		x = (int)tmp_x;
		y = (int)tmp_y;

		two_d[x][y] = '-';
	}

}
