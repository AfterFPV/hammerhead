#pragma once
#include "AstrObject.h"
#include <list>
#include <iterator>
#include <iostream>
#include <SDL2/SDL.h>

#define MIN_SPACE_SIZE 32
#define MAX_SPACE_SIZE 1024

using namespace std;

class Space {
private:
	char** two_d;
	int window_width;
	int window_height;
	int tile_width;
	int tile_height;
	int row_count;
	int column_count;
	SDL_Renderer* renderer;
public:
	list<AstrObject> objs;
	void tick();
	void add_object(AstrObject obj);
	void draw_map();
	void print_list();
	void set_renderer(SDL_Renderer* value);
	void set_window_size(int width, int height);
	Space() {
		Space(MIN_SPACE_SIZE, MIN_SPACE_SIZE);
	}

	Space(int column_count, int row_count) {
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
private:
	void init_map();
	void blank_map();
	void update_map();
	void update_orbit(AstrObject &obj);
	void add_center(AstrObject);
};

