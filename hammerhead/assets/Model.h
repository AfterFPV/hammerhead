#pragma once

#include <GL/glcorearb.h>
#include <glm/vec3.hpp>
#include <string>
#include <vector>
#include "tiny_obj_loader.h"

using namespace std;
using namespace tinyobj;

class Model
{
public:
	Model(string name, string resource_name, attrib_t&& attrib, vector<shape_t>&& shapes, vector<material_t>&& materials) : name(name), resource_name(resource_name), attrib(attrib), shapes(shapes), materials(materials) {
		
	}

	string get_name() {
		return this->name;
	}

	string get_resource_name() {
		return this->resource_name;
	}

	const attrib_t& get_attrib() const {
		return this->attrib;
	}

	const vector<shape_t>& get_shapes() const {
		return this->shapes;
	}

	const vector<material_t>& get_materials() const {
		return this->materials;
	}
private:
	string resource_name;
	attrib_t attrib;
	vector<shape_t> shapes;
	vector<material_t> materials;
	string name;
};

