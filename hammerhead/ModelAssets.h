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
	}

	ModelAsset& get_asset(string name) {
		return assets[name];
	}
};

