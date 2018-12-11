#pragma once
#include <string>
#include <SDL2/SDL_image.h>
using namespace std;

class DisplayAsset {
private:
	string name;
	string resource_name;
	SDL_Texture* texture;
public:
	DisplayAsset() {

	}

	DisplayAsset(string name, string resource_name) : name(name), resource_name(resource_name) {
	}

	string get_name() {
		return this->name;
	}

	string get_resource_name() {
		return this->resource_name;
	}

	void set_texture(SDL_Texture* texture) {
		this->texture = texture;
	}

	SDL_Texture* get_texture() {
		return this->texture;
	}
};