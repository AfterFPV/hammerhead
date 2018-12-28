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
#include "Camera.h"
#include "CelestialBodyFactory.h"
#include "OrientationAxes.h"
#include "Ship.h"
#include "ShipFactory.h"
#include "Space.h"

using namespace std;

constexpr int SPACE_SIZE = 32;

class GameEngine {
public:
	GameEngine();
	GameEngine(SDL_Window* window, int window_width, int window_height);
	void update();
	void init();
	void draw();
	void dispose();
	void input_camera_move_start();
	void input_camera_move_end();
	void input_camera_pan_start();
	void input_camera_pan_end();
	void input_camera_zoom_in();
	void input_camera_zoom_out();
	void input_move(float x, float y, float delta_x, float delta_y);
	void set_invert_pan(bool value);
	void set_show_axes(bool value);
	void set_pan_speed(float value);
	void set_vertex_shader_source(string value);
	void set_fragment_shader_source(string value);
private:
	Asset3dCollection assets;
	Space space;
	Camera camera;
	OrientationAxes orientation_axes;
	int window_width;
	int window_height;
	bool invert_pan;
	bool show_axes;
	bool is_camera_moving;
	bool is_camera_panning;
	bool is_camera_zooming;
	float pan_speed;
	float pan_factor;
	string vertex_shader_source;
	string fragment_shader_source;
	GLuint vertex_shader;
	GLuint fragment_shader;
	GLuint shader_program;
	SDL_Window* window;
	GLchar* vertex_source;
	GLchar* fragment_source;
	void bind_models();
	void calculate_pan_factor();
	void draw_axes();
	void init_assets();
	void init_camera();
	void init_space();
	void init_map();
	void init_shaders();
	void init_settings();
	void update_camera();
	char* file_read(const char* filename);
};
