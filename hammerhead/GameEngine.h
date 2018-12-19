#pragma once

#include <string>
#include <SDL2/SDL.h>
#include <GL/gl3w.h>
#include <GL/glcorearb.h>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


#include "Asset3d.h"
#include "Asset3dCollection.h"
#include "CelestialBodyFactory.h"
#include "Ship.h"
#include "ShipFactory.h"
#include "Space.h"

using namespace std;

constexpr int SPACE_SIZE = 32;

class GameEngine {
public:
	GameEngine();
	GameEngine(SDL_Window* window, int window_width, int window_height);
	void init();
	void draw();
	void tick();
	void stop();
	void set_vertex_shader_source(string value);
	void set_fragment_shader_source(string value);
private:
	Asset3dCollection assets;
	Space space;
	int window_width;
	int window_height;
	string vertex_shader_source;
	string fragment_shader_source;
	GLuint vertex_shader;
	GLuint fragment_shader;
	GLuint shader_program;
	GLuint vao;
	GLuint vbo[2];
	SDL_Window* window;
	GLchar* vertex_source;
	GLchar* fragment_source;
	void init_assets();
	void init_space();
	void init_map();
	void init_shaders();
	void bind_models();
	char* file_read(const char* filename);
};
