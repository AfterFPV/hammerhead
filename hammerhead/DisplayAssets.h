#pragma once
#include <map>
#include <string>
#include <SDL2/SDL.h>
#include "DisplayAsset.h"

using namespace std;

class DisplayAssets {
private:
	map<string, DisplayAsset> assets;
public:
	DisplayAssets() {
	}

	void load() {
		load_png_asset("planet");
	}

	void load_png_asset(string name) {
		string png_file_name = name + ".png";
		SDL_Surface* surface = IMG_Load("planet.png");
		
		if (surface == NULL)
		{
			string error_text = SDL_GetError();
			printf("Unable to load image %s! SDL Error: %s\n", png_file_name, error_text);
		}

		DisplayAsset asset(name, png_file_name, surface);
		assets[name] = asset;
	}

	void setup_textures(SDL_Renderer* renderer) {
		map<string, DisplayAsset>::iterator it;

		for (it = assets.begin(); it != assets.end(); it++)
		{
			SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, it->second.get_surface());
			it->second.set_texture(texture);
		}
	}

	DisplayAsset& get_asset(string name) {
		return assets[name];
	}
};

