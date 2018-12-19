#pragma once
#include "Coord.h"
#include "Vector2.h"
#include "GameTexture.h"
#include "assets/Model.h"
#include <iterator>
#include <list>
#include <math.h>
#include <string>
#include <GL/gl3w.h>
#include <GL/glcorearb.h>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <SDL2/SDL.h>

using namespace std;

class GameObject {
protected:
	bool is_clickable;
	int z_index;
	Coord pos;
	Coord center;
	float orientation;
	float orientation_degrees;
	Vector2 direction;
	Vector2 size;
	string name;
	glm::vec3 rotation;
	glm::vec3 rotation_position;
	glm::mat4 projection;
	glm::mat4 view;
	GLuint shader_program;
	Model* model;
public:
	GameObject(Coord pos, Coord center, Vector2 direction, Vector2 size) :
		pos(pos), center(center), direction(direction), size(size), z_index(0), is_clickable(false) {
		calculate_orientation();

		rotation = glm::vec3();
		rotation.x = 0.0f;
		rotation.y = 0.0f;
		rotation.z = 0.0f;

		rotation_position = glm::vec3();
		rotation_position.x = 0.0f;
		rotation_position.y = 0.0f;
		rotation_position.z = 0.0f;
	}

	virtual void draw();
	int get_z_index();
	Coord get_position();
	Coord get_center();
	Vector2 get_direction();
	Vector2 get_size();
	string get_name();
	void update_position();
	void calculate_orientation();
	void set_rotation(glm::vec3 rotation);
	void set_matrices(glm::mat4 projection, glm::mat4 view);
	void set_shader_program(GLuint value);
	void set_model(Model* model);
	void set_name(string name);
	void set_direction(Vector2 direction);
	void set_z_index(int value);
	bool hit_test(Coord location);
private:
	const int ASSET_ROTATION_OFFSET = 90;
};