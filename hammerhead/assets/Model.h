#pragma once

#include <GL/gl3w.h>
#include <GL/glcorearb.h>
#include <glm/vec3.hpp>
#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <limits>
#include <map>
#include <string>
#include <vector>
#include "DrawObject.h"
#include "tiny_obj_loader.h"
#include "stb_image.h"

#ifdef _WIN32
#ifdef __cplusplus
extern "C" {
#endif
#include <windows.h>

#ifdef max
#undef max
#endif

#ifdef min
#undef min
#endif

#include <mmsystem.h>
#ifdef __cplusplus
}
#endif
#endif

using namespace std;
using namespace tinyobj;




struct vec3 {
	float v[3];
	vec3() {
		v[0] = 0.0f;
		v[1] = 0.0f;
		v[2] = 0.0f;
	}
};

class Model
{
public:
	Model(string name, string resource_name, attrib_t&& attrib, vector<shape_t>&& shapes, vector<material_t>&& materials) : name(name), resource_name(resource_name), attrib(attrib), shapes(shapes), materials(materials) {
		this->normalized_scaling_factor = glm::vec3(1.0f, 1.0f, 1.0f);
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

	const vector<DrawObject>& get_draw_objects() const {
		return this->draw_objects;
	}

	const map<string, GLuint>& get_textures() const {
		return this->textures;
	}

	void calc_normal(float N[3], float v0[3], float v1[3], float v2[3]) {
		float v10[3];
		v10[0] = v1[0] - v0[0];
		v10[1] = v1[1] - v0[1];
		v10[2] = v1[2] - v0[2];

		float v20[3];
		v20[0] = v2[0] - v0[0];
		v20[1] = v2[1] - v0[1];
		v20[2] = v2[2] - v0[2];

		N[0] = v20[1] * v10[2] - v20[2] * v10[1];
		N[1] = v20[2] * v10[0] - v20[0] * v10[2];
		N[2] = v20[0] * v10[1] - v20[1] * v10[0];

		float len2 = N[0] * N[0] + N[1] * N[1] + N[2] * N[2];
		if (len2 > 0.0f) {
			float len = sqrtf(len2);

			N[0] /= len;
			N[1] /= len;
			N[2] /= len;
		}
	}

	void normalize_vector(vec3 &v) {
		float len2 = v.v[0] * v.v[0] + v.v[1] * v.v[1] + v.v[2] * v.v[2];
		if (len2 > 0.0f) {
			float len = sqrtf(len2);

			v.v[0] /= len;
			v.v[1] /= len;
			v.v[2] /= len;
		}
	}

	void compute_smoothing_normals(const tinyobj::attrib_t& attrib, const tinyobj::shape_t& shape,
		std::map<int, vec3>& smoothVertexNormals) {
		smoothVertexNormals.clear();
		std::map<int, vec3>::iterator iter;

		for (size_t f = 0; f < shape.mesh.indices.size() / 3; f++) {
			// Get the three indexes of the face (all faces are triangular)
			tinyobj::index_t idx0 = shape.mesh.indices[3 * f + 0];
			tinyobj::index_t idx1 = shape.mesh.indices[3 * f + 1];
			tinyobj::index_t idx2 = shape.mesh.indices[3 * f + 2];

			// Get the three vertex indexes and coordinates
			int vi[3];      // indexes
			float v[3][3];  // coordinates

			for (int k = 0; k < 3; k++) {
				vi[0] = idx0.vertex_index;
				vi[1] = idx1.vertex_index;
				vi[2] = idx2.vertex_index;
				assert(vi[0] >= 0);
				assert(vi[1] >= 0);
				assert(vi[2] >= 0);

				v[0][k] = attrib.vertices[3 * vi[0] + k];
				v[1][k] = attrib.vertices[3 * vi[1] + k];
				v[2][k] = attrib.vertices[3 * vi[2] + k];
			}

			// Compute the normal of the face
			float normal[3];
			calc_normal(normal, v[0], v[1], v[2]);

			// Add the normal to the three vertexes
			for (size_t i = 0; i < 3; ++i) {
				iter = smoothVertexNormals.find(vi[i]);
				if (iter != smoothVertexNormals.end()) {
					// add
					iter->second.v[0] += normal[0];
					iter->second.v[1] += normal[1];
					iter->second.v[2] += normal[2];
				}
				else {
					smoothVertexNormals[vi[i]].v[0] = normal[0];
					smoothVertexNormals[vi[i]].v[1] = normal[1];
					smoothVertexNormals[vi[i]].v[2] = normal[2];
				}
			}

		}  // f

		// Normalize the normals, that is, make them unit vectors
		for (iter = smoothVertexNormals.begin(); iter != smoothVertexNormals.end();
			iter++) {
			normalize_vector(iter->second);
		}
	} 

	bool has_smoothing_group(const tinyobj::shape_t& shape)
	{
		for (size_t i = 0; i < shape.mesh.smoothing_group_ids.size(); i++) {
			if (shape.mesh.smoothing_group_ids[i] > 0) {
				return true;
			}
		}
		return false;
	}

	bool file_exists(const std::string& abs_filename) {
		bool ret;

		SDL_RWops* fp = SDL_RWFromFile(abs_filename.c_str(), "rb");
		//FILE* fp = fopen(abs_filename.c_str(), "rb");
		if (fp) {
			ret = true;
			SDL_RWclose(fp);
		}
		else {
			ret = false;
		}

		return ret;
	}

	string get_base_dir(const std::string& filepath) {
		if (filepath.find_last_of("/\\") != std::string::npos)
			return filepath.substr(0, filepath.find_last_of("/\\"));
		return "";
	}

	glm::vec3 get_normalized_scaling_factor() {
		return this->normalized_scaling_factor;
	}

	void init_mesh_buffers() {
		std::string base_dir = "models/wf-obj/";
		float bmin[3];
		float bmax[3];

		if (base_dir.empty()) {
			base_dir = ".";
		}

		// Append `default` material
		materials.push_back(tinyobj::material_t());

		for (size_t i = 0; i < materials.size(); i++) {
			printf("material[%d].diffuse_texname = %s\n", int(i),
				materials[i].diffuse_texname.c_str());
		}

		// Load diffuse textures		
		for (size_t m = 0; m < materials.size(); m++) {
			tinyobj::material_t* mp = &materials[m];

			if (mp->diffuse_texname.length() > 0) {
				// Only load the texture if it is not already loaded
				if (textures.find(mp->diffuse_texname) == textures.end()) {
					GLuint texture_id;
					int w, h;
					int comp;

					std::string texture_filename = mp->diffuse_texname;
					if (!file_exists(texture_filename)) {
						// Append base dir.
						texture_filename = base_dir + mp->diffuse_texname;
						if (!file_exists(texture_filename)) {
							std::cerr << "Unable to find file: " << mp->diffuse_texname
								<< std::endl;
							exit(1);
						}
					}

					unsigned char* image =
						stbi_load(texture_filename.c_str(), &w, &h, &comp, STBI_default);
					if (!image) {
						std::cerr << "Unable to load texture: " << texture_filename
							<< std::endl;
						exit(1);
					}
					std::cout << "Loaded texture: " << texture_filename << ", w = " << w
						<< ", h = " << h << ", comp = " << comp << std::endl;

					glGenTextures(1, &texture_id);
					glBindTexture(GL_TEXTURE_2D, texture_id);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
					if (comp == 3) {
						glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB,
							GL_UNSIGNED_BYTE, image);
					}
					else if (comp == 4) {
						glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA,
							GL_UNSIGNED_BYTE, image);
					}
					else {
						assert(0);  // TODO
					}
					glBindTexture(GL_TEXTURE_2D, 0);
					stbi_image_free(image);
					textures.insert(std::make_pair(mp->diffuse_texname, texture_id));
				}
			}
		}
		

		bmin[0] = bmin[1] = bmin[2] = std::numeric_limits<float>::max();
		bmax[0] = bmax[1] = bmax[2] = -std::numeric_limits<float>::max();

		
		for (size_t s = 0; s < shapes.size(); s++) {
			DrawObject o;
			int current_material_id = 0;
			int previous_material_id = -1;
			int material_triangle_start = 0;
			int material_triangle_end = 0;
			//std::vector<float> buffer;  // pos(3float), normal(3float), color(3float)
			std::vector<float> v_positions;
			std::vector<float> v_normals;
			std::vector<float> v_colors;
			std::vector<float> v_texture_coords;

			// Check for smoothing group and compute smoothing normals
			std::map<int, vec3> smoothVertexNormals;
			if (has_smoothing_group(shapes[s]) > 0) {
				std::cout << "Compute smoothingNormal for shape [" << s << "]" << std::endl;
				compute_smoothing_normals(attrib, shapes[s], smoothVertexNormals);
			}

			for (size_t f = 0; f < shapes[s].mesh.indices.size() / 3; f++) {
				tinyobj::index_t idx0 = shapes[s].mesh.indices[3 * f + 0];
				tinyobj::index_t idx1 = shapes[s].mesh.indices[3 * f + 1];
				tinyobj::index_t idx2 = shapes[s].mesh.indices[3 * f + 2];

				current_material_id = shapes[s].mesh.material_ids[f];

				if ((current_material_id < 0) ||
					(current_material_id >= static_cast<int>(materials.size()))) {
					// Invaid material ID. Use default material.
					current_material_id =
						materials.size() -
						1;  // Default material is added to the last item in `materials`.
				}
				// if (current_material_id >= materials.size()) {
				//    std::cerr << "Invalid material index: " << current_material_id <<
				//    std::endl;
				//}
				//
				float diffuse[3];
				for (size_t i = 0; i < 3; i++) {
					diffuse[i] = materials[current_material_id].diffuse[i];
				}
				float tc[3][2];
				if (attrib.texcoords.size() > 0) {
					if ((idx0.texcoord_index < 0) || (idx1.texcoord_index < 0) ||
						(idx2.texcoord_index < 0)) {
						// face does not contain valid uv index.
						tc[0][0] = 0.0f;
						tc[0][1] = 0.0f;
						tc[1][0] = 0.0f;
						tc[1][1] = 0.0f;
						tc[2][0] = 0.0f;
						tc[2][1] = 0.0f;
					}
					else {
						assert(attrib.texcoords.size() >
							size_t(2 * idx0.texcoord_index + 1));
						assert(attrib.texcoords.size() >
							size_t(2 * idx1.texcoord_index + 1));
						assert(attrib.texcoords.size() >
							size_t(2 * idx2.texcoord_index + 1));

						// Flip Y coord.
						tc[0][0] = attrib.texcoords[2 * idx0.texcoord_index];
						tc[0][1] = 1.0f - attrib.texcoords[2 * idx0.texcoord_index + 1];
						tc[1][0] = attrib.texcoords[2 * idx1.texcoord_index];
						tc[1][1] = 1.0f - attrib.texcoords[2 * idx1.texcoord_index + 1];
						tc[2][0] = attrib.texcoords[2 * idx2.texcoord_index];
						tc[2][1] = 1.0f - attrib.texcoords[2 * idx2.texcoord_index + 1];
					}
				}
				else {
					tc[0][0] = 0.0f;
					tc[0][1] = 0.0f;
					tc[1][0] = 0.0f;
					tc[1][1] = 0.0f;
					tc[2][0] = 0.0f;
					tc[2][1] = 0.0f;
				}

				float v[3][3];
				for (int k = 0; k < 3; k++) {
					int f0 = idx0.vertex_index;
					int f1 = idx1.vertex_index;
					int f2 = idx2.vertex_index;
					assert(f0 >= 0);
					assert(f1 >= 0);
					assert(f2 >= 0);

					v[0][k] = attrib.vertices[3 * f0 + k];
					v[1][k] = attrib.vertices[3 * f1 + k];
					v[2][k] = attrib.vertices[3 * f2 + k];
					bmin[k] = std::min(v[0][k], bmin[k]);
					bmin[k] = std::min(v[1][k], bmin[k]);
					bmin[k] = std::min(v[2][k], bmin[k]);
					bmax[k] = std::max(v[0][k], bmax[k]);
					bmax[k] = std::max(v[1][k], bmax[k]);
					bmax[k] = std::max(v[2][k], bmax[k]);
				}

				float n[3][3];
				{
					bool invalid_normal_index = false;
					if (attrib.normals.size() > 0) {
						int nf0 = idx0.normal_index;
						int nf1 = idx1.normal_index;
						int nf2 = idx2.normal_index;

						if ((nf0 < 0) || (nf1 < 0) || (nf2 < 0)) {
							// normal index is missing from this face.
							invalid_normal_index = true;
						}
						else {
							for (int k = 0; k < 3; k++) {
								assert(size_t(3 * nf0 + k) < attrib.normals.size());
								assert(size_t(3 * nf1 + k) < attrib.normals.size());
								assert(size_t(3 * nf2 + k) < attrib.normals.size());
								n[0][k] = attrib.normals[3 * nf0 + k];
								n[1][k] = attrib.normals[3 * nf1 + k];
								n[2][k] = attrib.normals[3 * nf2 + k];
							}
						}
					}
					else {
						invalid_normal_index = true;
					}

					if (invalid_normal_index && !smoothVertexNormals.empty()) {
						// Use smoothing normals
						int f0 = idx0.vertex_index;
						int f1 = idx1.vertex_index;
						int f2 = idx2.vertex_index;

						if (f0 >= 0 && f1 >= 0 && f2 >= 0) {
							n[0][0] = smoothVertexNormals[f0].v[0];
							n[0][1] = smoothVertexNormals[f0].v[1];
							n[0][2] = smoothVertexNormals[f0].v[2];

							n[1][0] = smoothVertexNormals[f1].v[0];
							n[1][1] = smoothVertexNormals[f1].v[1];
							n[1][2] = smoothVertexNormals[f1].v[2];

							n[2][0] = smoothVertexNormals[f2].v[0];
							n[2][1] = smoothVertexNormals[f2].v[1];
							n[2][2] = smoothVertexNormals[f2].v[2];

							invalid_normal_index = false;
						}
					}

					if (invalid_normal_index) {
						// compute geometric normal
						calc_normal(n[0], v[0], v[1], v[2]);
						n[1][0] = n[0][0];
						n[1][1] = n[0][1];
						n[1][2] = n[0][2];
						n[2][0] = n[0][0];
						n[2][1] = n[0][1];
						n[2][2] = n[0][2];
					}
				}


				if (previous_material_id != current_material_id) {
					material_triangle_end = (v_positions.size() / 3) - 1;
					
					//If previous range was non-empty
					if (previous_material_id != -1) {
						string diffuse_texture_name = materials[previous_material_id].diffuse_texname.c_str();
						GLuint texture_id = 0;

						if (textures.find(diffuse_texture_name) != textures.end()) {
							texture_id = textures[diffuse_texture_name];
						}

						MaterialRange range;

						range.triangle_start = material_triangle_start;
						range.triangle_end = material_triangle_end;
						range.material_id = previous_material_id;
						range.texture_id = texture_id;

						o.material_ranges.push_back(range);
					}

					material_triangle_start = material_triangle_end + 1;
					previous_material_id = current_material_id;
				}

				for (int k = 0; k < 3; k++) {
					v_positions.push_back(v[k][0]);
					v_positions.push_back(v[k][1]);
					v_positions.push_back(v[k][2]);

					v_normals.push_back(n[k][0]);
					v_normals.push_back(n[k][1]);
					v_normals.push_back(n[k][2]);
					// Combine normal and diffuse to get color.
					float normal_factor = 0.2;
					float diffuse_factor = 1 - normal_factor;
					float c[3] = { n[k][0] * normal_factor + diffuse[0] * diffuse_factor,
									n[k][1] * normal_factor + diffuse[1] * diffuse_factor,
									n[k][2] * normal_factor + diffuse[2] * diffuse_factor };
					float len2 = c[0] * c[0] + c[1] * c[1] + c[2] * c[2];
					if (len2 > 0.0f) {
						float len = sqrtf(len2);

						c[0] /= len;
						c[1] /= len;
						c[2] /= len;
					}

					v_colors.push_back(c[0] * 0.5 + 0.5);
					v_colors.push_back(c[1] * 0.5 + 0.5);
					v_colors.push_back(c[2] * 0.5 + 0.5);

					v_texture_coords.push_back(tc[k][0]);
					v_texture_coords.push_back(tc[k][1]);
				}
			}

			//Add last material_id range (or only one if uniform)			
			material_triangle_end = (v_positions.size() / 3) - 1;
			string diffuse_texture_name = materials[current_material_id].diffuse_texname.c_str();
			GLuint texture_id = 0;

			if (textures.find(diffuse_texture_name) != textures.end()) {
				texture_id = textures[diffuse_texture_name];
			}

			MaterialRange range;
			range.triangle_start = material_triangle_start;
			range.triangle_end = material_triangle_end;
			range.material_id = previous_material_id;
			range.texture_id = texture_id;

			o.material_ranges.push_back(range);

			o.vao_id = 0;
			o.num_triangles = 0;

			//Setup vao and child buffers
			if (v_positions.size() > 0) {
				
				//Allocate and assign a vertex array object to our handle
				glGenVertexArrays(1, &o.vao_id);

				//Bind our vertex array object as the current used object
				glBindVertexArray(o.vao_id);

				//Allocate and assign to vertex buffer object to our handle
				GLuint vbo[4];
				glGenBuffers(4, vbo);

				//Setup position attributes
				glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
				glBufferData(GL_ARRAY_BUFFER, v_positions.size() * sizeof(float), &v_positions.at(0), GL_STATIC_DRAW);
				glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
				glEnableVertexAttribArray(0);

				//Setup color attributes
				glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
				glBufferData(GL_ARRAY_BUFFER, v_colors.size() * sizeof(float), &v_colors.at(0), GL_STATIC_DRAW);
				glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
				glEnableVertexAttribArray(1);

				//Setup normal attributes
				glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
				glBufferData(GL_ARRAY_BUFFER, v_normals.size() * sizeof(float), &v_normals.at(0), GL_STATIC_DRAW);
				glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
				glEnableVertexAttribArray(2);

				//Setup texture coordinates
				glBindBuffer(GL_ARRAY_BUFFER, vbo[3]);
				glBufferData(GL_ARRAY_BUFFER, v_texture_coords.size() * sizeof(float), &v_texture_coords.at(0), GL_STATIC_DRAW);
				glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 0, 0);
				glEnableVertexAttribArray(3);

				//Record buffer ids
				o.num_triangles = v_positions.size() / 3;
				o.position_vbo = vbo[0];
				o.color_vbo = vbo[1];
				o.normal_vbo = vbo[2];
				o.texture_coord_vbo = vbo[3];

				this->draw_objects.push_back(o);
			}
		}	

		min_vertex_values = glm::vec3(bmin[0], bmin[1], bmin[2]);
		max_vertex_values = glm::vec3(bmax[0], bmax[1], bmax[2]);

		//Set value to uniformly scale off relative x size;
		float delta_x = max_vertex_values.x - min_vertex_values.x;
		if (delta_x > 0) {
			float factor_x = 1.0f / delta_x;
			normalized_scaling_factor = glm::vec3(factor_x, factor_x, factor_x);
		}
	}
private:

	string name;
	string resource_name;
	glm::vec3 min_vertex_values;
	glm::vec3 max_vertex_values;
	glm::vec3 normalized_scaling_factor;
	attrib_t attrib;
	vector<shape_t> shapes;
	vector<material_t> materials;
	vector<DrawObject> draw_objects;
	map<string, GLuint> textures;
};

