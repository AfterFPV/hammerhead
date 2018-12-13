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
		load_png_asset("box");
		load_png_asset("planet");
		load_png_asset("planet-large");
		load_png_asset("ship");
	}

	void load_png_asset(string name) {
		string image_folder = "images";
		string png_file_name = image_folder + "/" + name + ".png";
		DisplayAsset asset(name, png_file_name);
		assets[name] = asset;
	}

	void setup_textures(SDL_Renderer* renderer) {
		map<string, DisplayAsset>::iterator it;

		for (it = assets.begin(); it != assets.end(); it++)
		{
			string png_file_name = it->second.get_resource_name();
			SDL_Surface* surface = IMG_Load(png_file_name.c_str());
			if (surface == NULL)
			{
				string error_text = SDL_GetError();
				printf("Unable to load image %s! SDL Error: %s\n", png_file_name, error_text);
			}

			SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
			SDL_FreeSurface(surface);
			//SDL_Texture* texture = IMG_LoadTexture(renderer, it->second.get_resource_name().c_str());
			
			it->second.set_texture(texture);
		}
	}

	DisplayAsset& get_asset(string name) {
		return assets[name];
	}
};

