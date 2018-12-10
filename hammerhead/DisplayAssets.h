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
		string asset_folder = "";


	}
	DisplayAsset& get_asset(string name) {
		return assets[name];
	}
};

