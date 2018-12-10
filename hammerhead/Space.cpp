#pragma once

#include "Space.h"

Space::Space() :
	Space(MIN_SPACE_SIZE, MIN_SPACE_SIZE) {
}

Space::Space(int column_count, int row_count) {
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
}

void Space::add_object(SpaceObject obj) {
	obj.set_renderer(this->renderer);
	objs.push_back(obj);
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

void Space::draw_map() {
	/*
	int posX = 0;
	int posY = 0;

	//Draw map grid
	for (int i = 0; i < this->column_count; i++) {
		posY = i * this->tile_height;

		for (int j = 0; j < this->row_count; j++) {
			posX = j * this->tile_width;

			char tile_type = two_d[i][j];

			switch (tile_type) {
			case 'x':
				SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
				break;
			case '-':
				SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
				break;
			case '.':
				SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
				break;
			}
			SDL_Rect tile;
			tile.h = tile_height;
			tile.w = tile_width;
			tile.x = posX;
			tile.y = posY;

			SDL_RenderFillRect(renderer, &tile);
		}
	}*/
	//Clear draw area
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);

	//Draw objects in space
	list<SpaceObject>::iterator it;
	for (it = objs.begin(); it != objs.end(); it++) {
		it->draw();
	}

	// Render the changes above
	SDL_RenderPresent(renderer);
}

void Space::print_list() {

	list<SpaceObject>::iterator it;
	int x, y;

	for (it = objs.begin(); it != objs.end(); it++) {

		//pos = &(it->get_position());
		x = (int)(it->get_position().get_floatX() + it->get_direction().x);
		y = (int)(it->get_position().get_floatY() + it->get_direction().y);

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
	/*
	blank_map();

	int x, y, width, height;
	*/
	list<SpaceObject>::iterator it;

	for (it = objs.begin(); it != objs.end(); it++) {
		it->update_position();

		/*
		x = (int)(it->get_position().get_floatX() + it->get_direction().x);
		y = (int)(it->get_position().get_floatY() + it->get_direction().y);

		width = it->get_size().x;
		height = it->get_size().y;

		two_d[x][y] = 'x';

		for (int i = x; i < x + width; i++) {
			for (int j = y; j < y + height; j++) {
				two_d[i][j] = 'x';
			}
		}

		//CelestialBody* body = dynamic_cast<CelestialBody*>(&(*it));
		//CelestialBody& body = dynamic_cast<CelestialBody&>((*it));

		//if (body != NULL) {
		//	update_orbit((*body));
		//}
		*/
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

void Space::add_center(SpaceObject obj) {


}