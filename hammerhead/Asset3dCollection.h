#pragma once

#include <map>
#include <string>
#include "Asset3d.h"

using namespace std;

class Asset3dCollection {
private:
	map<string, Asset3d> assets;
public:
	Asset3dCollection() {
	}

	void load() {
		load_stl_asset("sphere");
	}

	void load_stl_asset(string name) {
		string stl_model_folder = "models/stl";
		string model_file_name = stl_model_folder + "/" + name + ".stl";
		Asset3d asset(name, model_file_name);
		assets[name] = asset;
	}

	void setup_models() {

	}

	Asset3d& get_asset(string name) {
		return assets[name];
	}
};
