#pragma once

#include <iostream>
#include <map>
#include <string>
#include "Asset3d.h"
#include "assets/Model.h"
#include "assets/tiny_obj_loader.h"

using namespace std;

class Asset3dCollection {
private:
	map<string, Asset3d> assets;
public:
	Asset3dCollection() {
	}

	void load() {
		load_obj_asset("sphere");
	}

	void load_obj_asset(string name) {
		string stl_model_folder = "models/obj";
		string resource_file_name = stl_model_folder + "/" + name + ".obj";
		Asset3d asset(name, resource_file_name);
		assets[name] = asset;
	}

	void setup_models() {
		map<string, Asset3d>::iterator it;

		for (it = assets.begin(); it != assets.end(); it++)
		{
			Model* m = load_model(it->second);

			it->second.set_model(m);
		}
	}

	Model* load_model(Asset3d& asset) {	
		std::string inputfile = asset.get_resource_name();
		tinyobj::attrib_t attrib;
		std::vector<tinyobj::shape_t> shapes;
		std::vector<tinyobj::material_t> materials;

		std::string warn;
		std::string err;
		bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, inputfile.c_str());

		if (!err.empty()) { // `err` may contain warning message.
			std::cerr << err << std::endl;
		}

		if (!ret) {
			exit(1);
		}

		// Loop over shapes
		for (size_t s = 0; s < shapes.size(); s++) {
			// Loop over faces(polygon)
			size_t index_offset = 0;
			for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
				int fv = shapes[s].mesh.num_face_vertices[f];

				// Loop over vertices in the face.
				for (size_t v = 0; v < fv; v++) {
					// access to vertex
					tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
					tinyobj::real_t vx = attrib.vertices[3 * idx.vertex_index + 0];
					tinyobj::real_t vy = attrib.vertices[3 * idx.vertex_index + 1];
					tinyobj::real_t vz = attrib.vertices[3 * idx.vertex_index + 2];
					tinyobj::real_t nx = attrib.normals[3 * idx.normal_index + 0];
					tinyobj::real_t ny = attrib.normals[3 * idx.normal_index + 1];
					tinyobj::real_t nz = attrib.normals[3 * idx.normal_index + 2];
					tinyobj::real_t tx = attrib.texcoords[2 * idx.texcoord_index + 0];
					tinyobj::real_t ty = attrib.texcoords[2 * idx.texcoord_index + 1];
					// Optional: vertex colors
					// tinyobj::real_t red = attrib.colors[3*idx.vertex_index+0];
					// tinyobj::real_t green = attrib.colors[3*idx.vertex_index+1];
					// tinyobj::real_t blue = attrib.colors[3*idx.vertex_index+2];
				}
				index_offset += fv;

				// per-face material
				shapes[s].mesh.material_ids[f];
			}
		}

		Model* m = new Model(asset.get_name(), asset.get_resource_name(), std::move(attrib), std::move(shapes), std::move(materials));

		return m;
	}

	Asset3d& get_asset(string name) {
		return assets[name];
	}
};
