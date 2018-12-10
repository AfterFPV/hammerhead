#pragma once
#include "Coord.h"
#include <SDL2/SDL.h>

class GameTexture {
private:
	SDL_Texture* texture;
	Coord relative_pos;
	float width;
	float height;
public:
	GameTexture(SDL_Texture* texture, Coord relative_pos, float width, float height) : texture(texture), relative_pos(relative_pos), width(width), height(height) {
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
};

