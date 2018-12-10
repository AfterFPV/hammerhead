#pragma once
#include <string>
#include <SDL2/SDL_image.h>
using namespace std;

class DisplayAsset {
private:
	string name;
	string resource_name;
	SDL_Surface* surface;
	SDL_Texture* texture;
public:
	DisplayAsset() {

	}

	DisplayAsset(string name, string resource_name, SDL_Surface* surface) : name(name), resource_name(resource_name), surface(surface) {
	}

	string get_name() {
		return this->name;
	}

	void set_texture(SDL_Texture* texture) {
		this->texture = texture;
	}

	SDL_Texture* get_texture() {
		return this->texture;
	}

	SDL_Surface* get_surface() {
		return this->surface;
	}
};