#pragma once

#pragma once
#include <string>
#include "assets/Model.h"
using namespace std;

class Asset3d {
private:
	string name;
	string resource_name;
	Model* model;
public:
	Asset3d() {

	}

	Asset3d(string name, string resource_name) : name(name), resource_name(resource_name) {
	}

	string get_name() {
		return this->name;
	}

	string get_resource_name() {
		return this->resource_name;
	}

	void set_model(Model* value) {
		this->model = value;
	}

	Model* get_model() {
		return this->model;
	}
};