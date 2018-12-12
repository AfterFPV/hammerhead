#pragma once
#include "Coord.h"
#include "Color.h"
#include <SDL2/SDL.h>

class GameTexture {
private:
	SDL_Texture* texture;
	Coord relative_pos;
	Color color;
	float width;
	float height;
public:
	GameTexture(SDL_Texture* texture, Coord relative_pos, float width, float height) : texture(texture), relative_pos(relative_pos), width(width), height(height), color(Color(255,255,255)) {
	}
	GameTexture(SDL_Texture* texture, Coord relative_pos, float width, float height, Color color) : texture(texture), relative_pos(relative_pos), width(width), height(height), color(color) {
	}
	
	SDL_Texture* get_texture(){
		return this->texture;
	}

	Coord get_relative_pos() {
		return this->relative_pos;
	}

	float get_width() {
		return this->width;
	}

	float get_height() {
		return this->height;
	}

	Color get_color() {
		return this->color;
	}

	void set_color(Color value) {
		this->color = value;
	}
};

