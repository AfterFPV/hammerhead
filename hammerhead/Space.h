#pragma once

#include "AstralObject.h"
#include <list>
#include <iterator>
#include <iostream>
#include <SDL2/SDL.h>

constexpr int MIN_SPACE_SIZE = 32;
constexpr int MAX_SPACE_SIZE = 1024;

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
	list<AstralObject> objs;
	void tick();
	void add_object(AstralObject obj);
	void draw_map();
	void print_list();
	void set_renderer(SDL_Renderer* value);
	void set_window_size(int width, int height);
	Space();
	Space(int column_count, int row_count);
private:
	void init_map();
	void blank_map();
	void update_map();
	void update_orbit(AstralObject &obj);
	void add_center(AstralObject);
};

