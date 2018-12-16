#pragma once
#include <map>
#include <string>
#include <SDL2/SDL.h>
#include "ModelAsset.h"

using namespace std;

class ModelAssets {
private:
	map<string, ModelAsset> assets;
public:
	ModelAssets() {
	}

	void load() {
		load_stl_asset("sphere");
	}

	void load_stl_asset(string name) {
		string stl_model_folder = "models/stl";
		string model_file_name = stl_model_folder + "/" + name + ".stl";
		ModelAsset asset(name, model_file_name);
		assets[name] = asset;
	}

	void setup_textures(SDL_Renderer* renderer) {
		map<string, ModelAsset>::iterator it;

		/*
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
		}*/
	}

	ModelAsset& get_asset(string name) {
		return assets[name];
	}
};

