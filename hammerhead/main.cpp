#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <list>
#include <iterator>
#include <iostream>
#include <chrono>
#include <utility>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "CelestialBody.h"
#include "Coord.h"
#include "GameEngine.h"
#include "ModelAssets.h"
#include "Moon.h"
#include "Space.h"
#include "Planet.h"
#include "CelestialBodyFactory.h"
#include "Ship.h"
#include "ShipFactory.h"
#include "Vector2.h"

//#include "GL/glcorearb.h"
#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"

#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


// About OpenGL function loaders: modern OpenGL doesn't have a standard header file and requires individual function pointers to be loaded manually. 
// Helper libraries are often used for this purpose! Here we are supporting a few common ones: gl3w, glew, glad.
// You may use another loader/header of your choice (glext, glLoadGen, etc.), or chose to manually implement your own.
#if defined(IMGUI_IMPL_OPENGL_LOADER_GL3W)
#include <GL/gl3w.h>    // Initialize with gl3wInit()
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLEW)
#include <GL/glew.h>    // Initialize with glewInit()
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLAD)
#include <glad/glad.h>  // Initialize with gladLoadGL()
#else
#include IMGUI_IMPL_OPENGL_LOADER_CUSTOM
#endif

using namespace std;

SDL_Event inputEvent;
SDL_Surface* window_surface;
SDL_Renderer* renderer;
//DisplayAssets assets;
//ModelAssets models;
GameEngine engine;
float x_rotation;
float y_rotation;
float z_rotation;

/*
void initialize_map() {
	Coord sun_pos(320, 320);
	float sun_radius = 50.0;
	unique_ptr<SpaceObject> sun = CelestialBodyFactory::create_sun("Sun", sun_pos, sun_radius, assets.get_asset("planet-large"), Color(251, 184, 41));

	Coord earth_pos(140, 140);
	float earth_radius = 18.0;
	unique_ptr<SpaceObject> earth = CelestialBodyFactory::create_planet("Earth", earth_pos, earth_radius, assets.get_asset("planet-large"), Color(42, 143, 189));
	Orbit* sun_orbit = new Orbit(sun.get(), 2);
	earth->set_orbit(sun_orbit);

	Coord moon_pos(240, 240);
	float moon_radius = 4.5;
	unique_ptr<SpaceObject> moon = CelestialBodyFactory::create_moon("Moon", moon_pos, moon_radius, assets.get_asset("planet"), Color(216,216,216));
	Orbit* earth_orbit = new Orbit(earth.get(), 5);
	moon->set_orbit(earth_orbit);

	Coord ship_pos(300, 300);
	Vector2 ship_direction(1, 1);
	Vector2 ship_size(8, 8);
	unique_ptr<Ship> ship = ShipFactory::create("intrepid", ship_pos, ship_size, assets.get_asset("ship"));
	ship->set_direction(ship_direction);

	space.add_object(move(sun));
	space.add_object(move(earth));
	space.add_object(move(moon));
	space.add_object(move(ship));
}*/

void sdldie(const char* msg) {
	printf("%s: %s\n", msg, SDL_GetError());
	SDL_Quit();
	exit(1);
}

void checkSDLError(int line = -1) {
#ifndef NDEBUG
	const char* error = SDL_GetError();
	if (*error != '\0') {
		printf("SDL Error: %s\n", error);
		if (line != -1) {
			printf(" + line: %i\n", line);			
		}
		SDL_ClearError();
	}
#endif
}

char* file_read(const char* filename) {
	SDL_RWops *rw = SDL_RWFromFile(filename, "rb");
	if (rw == NULL) return NULL;

	Sint64 res_size = SDL_RWsize(rw);
	char* res = (char*)malloc(res_size + 1);

	Sint64 nb_read_total = 0, nb_read = 1;
	char* buf = res;
	while (nb_read_total < res_size && nb_read != 0) {
		nb_read = SDL_RWread(rw, buf, 1, (res_size - nb_read_total));
		nb_read_total += nb_read;
		buf += nb_read;
	}
	SDL_RWclose(rw);
	if (nb_read_total != res_size) {
		free(res);
		return NULL;
	}

	res[nb_read_total] = '\0';
	return res;
}

void draw_scene(SDL_Window* sdl_window) {
	int i;
	GLuint vao, vbo[2];

	int is_compiled_vs, is_compiled_fs;
	int is_linked;
	int max_length;
	char* vertex_info_log;
	char* fragment_info_log;
	char* shader_program_info_log;


	const GLfloat diamond[4][2] = {
		{ 0.0, 1.0 },
		{ 1.0, 0.0 },
		{ 0.0, -1.0},
		{-1.0, 0.0}
	};

	const GLfloat colors[4][3] = {
		{ 1.0, 0.0, 0.0 },
		{ 0.0, 1.0, 0.0 },
		{ 0.0, 0.0, 1.0 },
		{ 1.0, 1.0, 1.0 }
	};

	static const GLfloat g_vertex_buffer_data[] = {
	-1.0f,-1.0f,-1.0f, // triangle 1 : begin
	-1.0f,-1.0f, 1.0f,
	-1.0f, 1.0f, 1.0f, // triangle 1 : end
	1.0f, 1.0f,-1.0f, // triangle 2 : begin
	-1.0f,-1.0f,-1.0f,
	-1.0f, 1.0f,-1.0f, // triangle 2 : end
	1.0f,-1.0f, 1.0f,
	-1.0f,-1.0f,-1.0f,
	1.0f,-1.0f,-1.0f,
	1.0f, 1.0f,-1.0f,
	1.0f,-1.0f,-1.0f,
	-1.0f,-1.0f,-1.0f,
	-1.0f,-1.0f,-1.0f,
	-1.0f, 1.0f, 1.0f,
	-1.0f, 1.0f,-1.0f,
	1.0f,-1.0f, 1.0f,
	-1.0f,-1.0f, 1.0f,
	-1.0f,-1.0f,-1.0f,
	-1.0f, 1.0f, 1.0f,
	-1.0f,-1.0f, 1.0f,
	1.0f,-1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f,-1.0f,-1.0f,
	1.0f, 1.0f,-1.0f,
	1.0f,-1.0f,-1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f,-1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f,-1.0f,
	-1.0f, 1.0f,-1.0f,
	1.0f, 1.0f, 1.0f,
	-1.0f, 1.0f,-1.0f,
	-1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	-1.0f, 1.0f, 1.0f,
	1.0f,-1.0f, 1.0f
	};
	// One color for each vertex. They were generated randomly.
	static const GLfloat g_color_buffer_data[] = {
		0.583f,  0.771f,  0.014f,
		0.609f,  0.115f,  0.436f,
		0.327f,  0.483f,  0.844f,
		0.822f,  0.569f,  0.201f,
		0.435f,  0.602f,  0.223f,
		0.310f,  0.747f,  0.185f,
		0.597f,  0.770f,  0.761f,
		0.559f,  0.436f,  0.730f,
		0.359f,  0.583f,  0.152f,
		0.483f,  0.596f,  0.789f,
		0.559f,  0.861f,  0.639f,
		0.195f,  0.548f,  0.859f,
		0.014f,  0.184f,  0.576f,
		0.771f,  0.328f,  0.970f,
		0.406f,  0.615f,  0.116f,
		0.676f,  0.977f,  0.133f,
		0.971f,  0.572f,  0.833f,
		0.140f,  0.616f,  0.489f,
		0.997f,  0.513f,  0.064f,
		0.945f,  0.719f,  0.592f,
		0.543f,  0.021f,  0.978f,
		0.279f,  0.317f,  0.505f,
		0.167f,  0.620f,  0.077f,
		0.347f,  0.857f,  0.137f,
		0.055f,  0.953f,  0.042f,
		0.714f,  0.505f,  0.345f,
		0.783f,  0.290f,  0.734f,
		0.722f,  0.645f,  0.174f,
		0.302f,  0.455f,  0.848f,
		0.225f,  0.587f,  0.040f,
		0.517f,  0.713f,  0.338f,
		0.053f,  0.959f,  0.120f,
		0.393f,  0.621f,  0.362f,
		0.673f,  0.211f,  0.457f,
		0.820f,  0.883f,  0.371f,
		0.982f,  0.099f,  0.879f
	};


	//This pointers will receive the contents of our shader source code files
	GLchar* vertex_source;
	GLchar* fragment_source;

	//These are handles used to reference the shaders
	GLuint vertex_shader, fragment_shader;

	//This is a handle to the shader program
	GLuint shader_program;

	//Allocate and assign a vertex array object to our handle
	glGenVertexArrays(1, &vao);

	//Bind our vertext array object as the current used object
	glBindVertexArray(vao);

	//Allocate and assign to vertex buffer object to our handle
	glGenBuffers(2, vbo);

	//Bind our first VBO as being the active buffer and storing vertex attributes (coordinates)
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);

	//Copy the vertex data from diamond to our buffer
	// 8 * sizeof(GLfloat) is the size of the diamond array since it contains 8 GLfloat values
	//glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(GLfloat), diamond, GL_STATIC_DRAW);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

	//Specify that our coordinate data is going into attribute index 0, and contains three floats per vertex
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	//Enable attribute index 0 as being used
	glEnableVertexAttribArray(0);

	//Bind our second VBO as being the active buffer and storing vertex attribure (colors)
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);

	//Copy the color data from colors to our buffer
	//12 * sizeof(GLfloat) is the size of the colors array since it contains 12 GLfloat values
	//glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(GLfloat), colors, GL_STATIC_DRAW);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), g_color_buffer_data, GL_STATIC_DRAW);

	//Specify that our color data is going into attribute index 1, and contains three floats per vertex
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

	//Enable attribute index 1 as being used
	glEnableVertexAttribArray(1);

	//Read our shaders into the appropriate buffers
	vertex_source = file_read("shaders/tutorial2.vert");
	fragment_source = file_read("shaders/tutorial2.frag");

	//Create an empty vertex shader handle
	vertex_shader = glCreateShader(GL_VERTEX_SHADER);

	//Send the shader source code to GL
	//Note that the source code is NULL character terminated
	//GL will automatically detect that, therefore length info can be 0 in this case (the last parameter)
	glShaderSource(vertex_shader, 1, (const GLchar**)&vertex_source, 0);

	//compile the vertex shader
	glCompileShader(vertex_shader);

	glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &is_compiled_vs);
	if (is_compiled_vs == GL_FALSE) {
		glGetShaderiv(vertex_shader, GL_INFO_LOG_LENGTH, &max_length);

		//The max length includes the NULL terminator character
		vertex_info_log = (char*)malloc(max_length);
		glGetShaderInfoLog(vertex_shader, max_length, &max_length, vertex_info_log);

		//Handle the error in an appropriate way such as displaying a message or writing to a log file
		//In this simple program we'll just leave
		free(vertex_info_log);
		return;
	}

	//Create an empty fragment shader handle
	fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);

	//Send the fragment shader code to GL
	//Note that the source code is NULL terminated
	//GL will automatically detect that, therefore the length info can be 0 in this case (the last parameter)
	glShaderSource(fragment_shader, 1, (const GLchar**)&fragment_source, 0);

	//Compile the fragment shader
	glCompileShader(fragment_shader);

	glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &is_compiled_fs);

	if (is_compiled_fs == GL_FALSE) {
		glGetShaderiv(fragment_shader, GL_INFO_LOG_LENGTH, &max_length);

		//the max length includes the NULL character
		fragment_info_log = (char*)malloc(max_length);

		glGetShaderInfoLog(fragment_shader, max_length, &max_length, fragment_info_log);

		//Handle the error in an appropriate way such as displaying a message or writing to a log file
		//In this simple program we'll just leave
		free(fragment_info_log);
		return;
	}

	//If we reached this point it means that the vertex and fragment shaders compiled and are syntax error free
	//We must link them together to make a GL shader program
	//GL shader programs are monolithic. It is a single piece made of one vertex shader and one fragment shader
	//Assign our program handle a name
	shader_program = glCreateProgram();

	//attach our shaders to the program
	glAttachShader(shader_program, vertex_shader);
	glAttachShader(shader_program, fragment_shader);

	//Bind attribute index 0 (coordinates) to in_Position and attribute index 1 (color) to in_Color
	glBindAttribLocation(shader_program, 0, "in_Position");
	glBindAttribLocation(shader_program, 1, "in_Color");

	//Link our program
	//At this stage, the vertex and fragment programs are inspected, optimized, and binary code is generated for the shader
	//The binary code is loaded to the GPU if there is no error
	glLinkProgram(shader_program);

	//Again we must check and make sure that it linked. If it fails, it would mean either there is a mismatch between the vertex
	//and fragment shaders. It might be that you have surpassed your GPU's abilities. Perhaps too many ALU operations or
	//too many texel fetch instructions or too many interpolators or dynamic loops
	glGetProgramiv(shader_program, GL_LINK_STATUS, (int*)&is_linked);
	if (is_linked == GL_FALSE) {
		//Notice that glGetProgramiv is used to get the length for a shader program, not glGetShaderiv
		glGetProgramiv(shader_program, GL_INFO_LOG_LENGTH, &max_length);

		//The max length includes the null character
		shader_program_info_log = (char*)malloc(max_length);

		//Notice that you use glGetProgramInfoLog and not glGetShaderInfoLog
		glGetProgramInfoLog(shader_program, max_length, &max_length, shader_program_info_log);

		//Handle the error in an appropriate way such as displaying a message or writing to a log file
		//In this simple program we'll just leave
		free(shader_program_info_log);
		return;
	}

	//Load the shader into the rendering pipeline
	glUseProgram(shader_program);
	glm::mat4 model_matrix;
	//glm::mat4 identity_matrix;

	//Create our projections matrix with a 45 degree field of view
	//a width-to-height ratio of 1.0 and view from .1 to 100 in front of us
	glm::mat4 projection_matrix = glm::perspective(glm::radians(45.0f), 1920.0f / 1200.0f, 0.1f, 100.0f);
	//glm::mat4 projection_matrix = glm::perspective(glm::radians(45.0), 1.0, 0.1, 100.0);


	//Loop our display rotating our model more each time
	glm::vec3 x_axis(1, 0, 0);
	glm::vec3 y_axis(0, 1, 0);
	glm::vec3 z_axis(0, 0, 1);

	glm::mat4 identity_matrix = glm::mat4(1.0);

	// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	glm::mat4 Projection = glm::perspective(glm::radians(45.0f), 1280.0f / 720.0f, 0.1f, 100.0f);

	// Camera matrix
	glm::mat4 View = glm::lookAt(
		glm::vec3(4, 3, 10), // Camera is at (4,3,10), in World Space
		glm::vec3(0, 0, 0), // and looks at the origin
		glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
	);

	// Model matrix : an identity matrix (model will be at the origin)
	glm::mat4 Model = glm::mat4(1.0f);

	for (i = 0; i < 360; i++) {
		//Load the identity matrix into model matrix, rotate the model, and move it back by 5
		//memcpy(model_matrix, identity_matrix, sizeof(GLfloat) * 16);
		glClearColor(0.0, 0.0, 0.0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		float x_angle_radians = glm::radians((GLfloat)i * 1.0);
		float y_angle_radians = glm::radians((GLfloat)i * 1.0);
		float z_angle_radians = glm::radians((GLfloat)i * 0.5);

		model_matrix = glm::rotate(identity_matrix, x_angle_radians, x_axis);
		model_matrix = glm::rotate(model_matrix, y_angle_radians, y_axis);
		model_matrix = glm::rotate(model_matrix, z_angle_radians, z_axis);

		glm::mat4 mvp = Projection * View * model_matrix;
		glUniformMatrix4fv(glGetUniformLocation(shader_program, "mvpmatrix"), 1, GL_FALSE, glm::value_ptr(mvp));


		glDrawArrays(GL_TRIANGLES, 0, 12 * 3);

		//Swap our buffers and make our changes visible
		SDL_GL_SwapWindow(sdl_window);

		//Sleep for 2 seconds
		SDL_Delay(100);
	}

	SDL_Delay(10000);

	//Cleanup all the things we bound and allocated
	glUseProgram(0);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDetachShader(shader_program, vertex_shader);
	glDetachShader(shader_program, fragment_shader);
	glDeleteProgram(shader_program);
	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);
	glDeleteBuffers(2, vbo);
	glDeleteVertexArrays(1, &vao);
	free(vertex_source);
	free(fragment_source);
}

void draw_models(SDL_Window* sdl_window) {
	int i;
	GLuint vao, vbo[2];

	int is_compiled_vs, is_compiled_fs;
	int is_linked;
	int max_length;
	char* vertex_info_log;
	char* fragment_info_log;
	char* shader_program_info_log;


	const GLfloat diamond[4][2] = {
		{ 0.0, 1.0 },
		{ 1.0, 0.0 },
		{ 0.0, -1.0},
		{-1.0, 0.0}
	};

	const GLfloat colors[4][3] = {
		{ 1.0, 0.0, 0.0 },
		{ 0.0, 1.0, 0.0 },
		{ 0.0, 0.0, 1.0 },
		{ 1.0, 1.0, 1.0 }
	};

	static const GLfloat g_vertex_buffer_data[] = {
	-1.0f,-1.0f,-1.0f, // triangle 1 : begin
	-1.0f,-1.0f, 1.0f,
	-1.0f, 1.0f, 1.0f, // triangle 1 : end
	1.0f, 1.0f,-1.0f, // triangle 2 : begin
	-1.0f,-1.0f,-1.0f,
	-1.0f, 1.0f,-1.0f, // triangle 2 : end
	1.0f,-1.0f, 1.0f,
	-1.0f,-1.0f,-1.0f,
	1.0f,-1.0f,-1.0f,
	1.0f, 1.0f,-1.0f,
	1.0f,-1.0f,-1.0f,
	-1.0f,-1.0f,-1.0f,
	-1.0f,-1.0f,-1.0f,
	-1.0f, 1.0f, 1.0f,
	-1.0f, 1.0f,-1.0f,
	1.0f,-1.0f, 1.0f,
	-1.0f,-1.0f, 1.0f,
	-1.0f,-1.0f,-1.0f,
	-1.0f, 1.0f, 1.0f,
	-1.0f,-1.0f, 1.0f,
	1.0f,-1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f,-1.0f,-1.0f,
	1.0f, 1.0f,-1.0f,
	1.0f,-1.0f,-1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f,-1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f,-1.0f,
	-1.0f, 1.0f,-1.0f,
	1.0f, 1.0f, 1.0f,
	-1.0f, 1.0f,-1.0f,
	-1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	-1.0f, 1.0f, 1.0f,
	1.0f,-1.0f, 1.0f
	};
	// One color for each vertex. They were generated randomly.
	static const GLfloat g_color_buffer_data[] = {
		0.583f,  0.771f,  0.014f,
		0.609f,  0.115f,  0.436f,
		0.327f,  0.483f,  0.844f,
		0.822f,  0.569f,  0.201f,
		0.435f,  0.602f,  0.223f,
		0.310f,  0.747f,  0.185f,
		0.597f,  0.770f,  0.761f,
		0.559f,  0.436f,  0.730f,
		0.359f,  0.583f,  0.152f,
		0.483f,  0.596f,  0.789f,
		0.559f,  0.861f,  0.639f,
		0.195f,  0.548f,  0.859f,
		0.014f,  0.184f,  0.576f,
		0.771f,  0.328f,  0.970f,
		0.406f,  0.615f,  0.116f,
		0.676f,  0.977f,  0.133f,
		0.971f,  0.572f,  0.833f,
		0.140f,  0.616f,  0.489f,
		0.997f,  0.513f,  0.064f,
		0.945f,  0.719f,  0.592f,
		0.543f,  0.021f,  0.978f,
		0.279f,  0.317f,  0.505f,
		0.167f,  0.620f,  0.077f,
		0.347f,  0.857f,  0.137f,
		0.055f,  0.953f,  0.042f,
		0.714f,  0.505f,  0.345f,
		0.783f,  0.290f,  0.734f,
		0.722f,  0.645f,  0.174f,
		0.302f,  0.455f,  0.848f,
		0.225f,  0.587f,  0.040f,
		0.517f,  0.713f,  0.338f,
		0.053f,  0.959f,  0.120f,
		0.393f,  0.621f,  0.362f,
		0.673f,  0.211f,  0.457f,
		0.820f,  0.883f,  0.371f,
		0.982f,  0.099f,  0.879f
	};


	//This pointers will receive the contents of our shader source code files
	GLchar* vertex_source;
	GLchar* fragment_source;

	//These are handles used to reference the shaders
	GLuint vertex_shader, fragment_shader;

	//This is a handle to the shader program
	GLuint shader_program;

	//Allocate and assign a vertex array object to our handle
	glGenVertexArrays(1, &vao);

	//Bind our vertext array object as the current used object
	glBindVertexArray(vao);

	//Allocate and assign to vertex buffer object to our handle
	glGenBuffers(2, vbo);

	//Bind our first VBO as being the active buffer and storing vertex attributes (coordinates)
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);

	//Copy the vertex data from diamond to our buffer
	// 8 * sizeof(GLfloat) is the size of the diamond array since it contains 8 GLfloat values
	//glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(GLfloat), diamond, GL_STATIC_DRAW);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

	//Specify that our coordinate data is going into attribute index 0, and contains three floats per vertex
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	//Enable attribute index 0 as being used
	glEnableVertexAttribArray(0);

	//Bind our second VBO as being the active buffer and storing vertex attribure (colors)
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);

	//Copy the color data from colors to our buffer
	//12 * sizeof(GLfloat) is the size of the colors array since it contains 12 GLfloat values
	//glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(GLfloat), colors, GL_STATIC_DRAW);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), g_color_buffer_data, GL_STATIC_DRAW);

	//Specify that our color data is going into attribute index 1, and contains three floats per vertex
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

	//Enable attribute index 1 as being used
	glEnableVertexAttribArray(1);

	//Read our shaders into the appropriate buffers
	vertex_source = file_read("shaders/tutorial2.vert");
	fragment_source = file_read("shaders/tutorial2.frag");

	//Create an empty vertex shader handle
	vertex_shader = glCreateShader(GL_VERTEX_SHADER);

	//Send the shader source code to GL
	//Note that the source code is NULL character terminated
	//GL will automatically detect that, therefore length info can be 0 in this case (the last parameter)
	glShaderSource(vertex_shader, 1, (const GLchar**)&vertex_source, 0);

	//compile the vertex shader
	glCompileShader(vertex_shader);

	glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &is_compiled_vs);
	if (is_compiled_vs == GL_FALSE) {
		glGetShaderiv(vertex_shader, GL_INFO_LOG_LENGTH, &max_length);

		//The max length includes the NULL terminator character
		vertex_info_log = (char*)malloc(max_length);
		glGetShaderInfoLog(vertex_shader, max_length, &max_length, vertex_info_log);

		//Handle the error in an appropriate way such as displaying a message or writing to a log file
		//In this simple program we'll just leave
		free(vertex_info_log);
		return;
	}

	//Create an empty fragment shader handle
	fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);

	//Send the fragment shader code to GL
	//Note that the source code is NULL terminated
	//GL will automatically detect that, therefore the length info can be 0 in this case (the last parameter)
	glShaderSource(fragment_shader, 1, (const GLchar**)&fragment_source, 0);

	//Compile the fragment shader
	glCompileShader(fragment_shader);

	glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &is_compiled_fs);

	if (is_compiled_fs == GL_FALSE) {
		glGetShaderiv(fragment_shader, GL_INFO_LOG_LENGTH, &max_length);

		//the max length includes the NULL character
		fragment_info_log = (char*)malloc(max_length);

		glGetShaderInfoLog(fragment_shader, max_length, &max_length, fragment_info_log);

		//Handle the error in an appropriate way such as displaying a message or writing to a log file
		//In this simple program we'll just leave
		free(fragment_info_log);
		return;
	}

	//If we reached this point it means that the vertex and fragment shaders compiled and are syntax error free
	//We must link them together to make a GL shader program
	//GL shader programs are monolithic. It is a single piece made of one vertex shader and one fragment shader
	//Assign our program handle a name
	shader_program = glCreateProgram();

	//attach our shaders to the program
	glAttachShader(shader_program, vertex_shader);
	glAttachShader(shader_program, fragment_shader);

	//Bind attribute index 0 (coordinates) to in_Position and attribute index 1 (color) to in_Color
	glBindAttribLocation(shader_program, 0, "in_Position");
	glBindAttribLocation(shader_program, 1, "in_Color");

	//Link our program
	//At this stage, the vertex and fragment programs are inspected, optimized, and binary code is generated for the shader
	//The binary code is loaded to the GPU if there is no error
	glLinkProgram(shader_program);

	//Again we must check and make sure that it linked. If it fails, it would mean either there is a mismatch between the vertex
	//and fragment shaders. It might be that you have surpassed your GPU's abilities. Perhaps too many ALU operations or
	//too many texel fetch instructions or too many interpolators or dynamic loops
	glGetProgramiv(shader_program, GL_LINK_STATUS, (int*)&is_linked);
	if (is_linked == GL_FALSE) {
		//Notice that glGetProgramiv is used to get the length for a shader program, not glGetShaderiv
		glGetProgramiv(shader_program, GL_INFO_LOG_LENGTH, &max_length);

		//The max length includes the null character
		shader_program_info_log = (char*)malloc(max_length);

		//Notice that you use glGetProgramInfoLog and not glGetShaderInfoLog
		glGetProgramInfoLog(shader_program, max_length, &max_length, shader_program_info_log);

		//Handle the error in an appropriate way such as displaying a message or writing to a log file
		//In this simple program we'll just leave
		free(shader_program_info_log);
		return;
	}

	//Load the shader into the rendering pipeline
	glUseProgram(shader_program);
	glm::mat4 model_matrix;
	//glm::mat4 identity_matrix;

	//Create our projections matrix with a 45 degree field of view
	//a width-to-height ratio of 1.0 and view from .1 to 100 in front of us
	glm::mat4 projection_matrix = glm::perspective(glm::radians(45.0f), 1920.0f / 1200.0f, 0.1f, 100.0f);
	//glm::mat4 projection_matrix = glm::perspective(glm::radians(45.0), 1.0, 0.1, 100.0);


	//Loop our display rotating our model more each time
	glm::vec3 x_axis(1, 0, 0);
	glm::vec3 y_axis(0, 1, 0);
	glm::vec3 z_axis(0, 0, 1);

	glm::mat4 identity_matrix = glm::mat4(1.0);

	// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	glm::mat4 Projection = glm::perspective(glm::radians(45.0f), 1280.0f / 720.0f, 0.1f, 100.0f);

	// Camera matrix
	glm::mat4 View = glm::lookAt(
		glm::vec3(4, 3, 10), // Camera is at (4,3,10), in World Space
		glm::vec3(0, 0, 0), // and looks at the origin
		glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
	);

	// Model matrix : an identity matrix (model will be at the origin)
	glm::mat4 Model = glm::mat4(1.0f);

	for (i = 0; i < 360; i++) {
		//Load the identity matrix into model matrix, rotate the model, and move it back by 5
		//memcpy(model_matrix, identity_matrix, sizeof(GLfloat) * 16);
		glClearColor(0.0, 0.0, 0.0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		float x_angle_radians = glm::radians((GLfloat)i * 1.0);
		float y_angle_radians = glm::radians((GLfloat)i * 1.0);
		float z_angle_radians = glm::radians((GLfloat)i * 0.5);

		model_matrix = glm::rotate(identity_matrix, x_angle_radians, x_axis);
		model_matrix = glm::rotate(model_matrix, y_angle_radians, y_axis);
		model_matrix = glm::rotate(model_matrix, z_angle_radians, z_axis);

		glm::mat4 mvp = Projection * View * model_matrix;
		glUniformMatrix4fv(glGetUniformLocation(shader_program, "mvpmatrix"), 1, GL_FALSE, glm::value_ptr(mvp));


		glDrawArrays(GL_TRIANGLES, 0, 12 * 3);

		//Swap our buffers and make our changes visible
		SDL_GL_SwapWindow(sdl_window);

		//Sleep for 2 seconds
		SDL_Delay(100);
	}

	SDL_Delay(10000);

	//Cleanup all the things we bound and allocated
	glUseProgram(0);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDetachShader(shader_program, vertex_shader);
	glDetachShader(shader_program, fragment_shader);
	glDeleteProgram(shader_program);
	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);
	glDeleteBuffers(2, vbo);
	glDeleteVertexArrays(1, &vao);
	free(vertex_source);
	free(fragment_source);
}

int open_gl_tutorial() {
	// Setup SDL
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0)
	{
		printf("Error: %s\n", SDL_GetError());
		return -1;
	}

	// Decide GL+GLSL versions
#if __APPLE__
	// GL 3.2 Core + GLSL 150
	const char* glsl_version = "#version 150";
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG); // Always required on Mac
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
#else
	// GL 3.0 + GLSL 130
	const char* glsl_version = "#version 130";
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
#endif

	// Create window with graphics context
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
	SDL_DisplayMode current;
	SDL_GetCurrentDisplayMode(0, &current);
	SDL_Window* window = SDL_CreateWindow("Dear ImGui SDL2+OpenGL3 example", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
	SDL_GLContext gl_context = SDL_GL_CreateContext(window);
	SDL_GL_SetSwapInterval(1); // Enable vsync

	// Initialize OpenGL loader
#if defined(IMGUI_IMPL_OPENGL_LOADER_GL3W)
	bool err = gl3wInit() != 0;
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLEW)
	bool err = glewInit() != GLEW_OK;
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLAD)
	bool err = gladLoadGL() == 0;
#else
	bool err = false; // If you use IMGUI_IMPL_OPENGL_LOADER_CUSTOM, your loader is likely to requires some form of initialization.
#endif
	if (err)
	{
		fprintf(stderr, "Failed to initialize OpenGL loader!\n");
		return 1;
	}

	//Enable z depth testing so objects closest to the viewpoint are in front of objects further away
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glClearColor(1.0, 0.0, 0.0, 1.0);

	glClear(GL_COLOR_BUFFER_BIT);
	SDL_GL_SwapWindow(window);

	draw_scene(window);

	SDL_Delay(30000);

	SDL_GL_DeleteContext(gl_context);
	SDL_DestroyWindow(window);

	return 0;



	return 0;
}

/*
int setup_renderer() {
	int posX = 100;
	int posY = 200;
	int sizeX = 1024;
	int sizeY = 800;
	SDL_Window* window;


	// Initialize SDL
	// ==========================================================
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		// Something failed, print error and exit.
		std::cout << " Failed to initialize SDL : " << SDL_GetError() << std::endl;
		return -1;
	}

	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");

	// Create and init the window
	// ==========================================================
	window = SDL_CreateWindow("hammerhead", posX, posY, sizeX, sizeY, 0);

	if (window == nullptr)
	{
		std::cout << "Failed to create window : " << SDL_GetError();
		return -1;
	}

	window_surface = SDL_GetWindowSurface(window);


	int imgFlags = IMG_INIT_PNG;

	int rees = IMG_Init(4);
	
	if (!(IMG_Init(imgFlags) & imgFlags))
	{
		printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());		
	}

	// Create and init the renderer
	// ==========================================================
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	if (renderer == nullptr)
	{
		std::cout << "Failed to create renderer : " << SDL_GetError();
		return -1;
	}

	// Render something
	// ==========================================================

	// Set size of renderer to the same as window
	SDL_RenderSetLogicalSize(renderer, sizeX, sizeY);

	// Set color of renderer to black
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

	// Clear the window and make it all black
	SDL_RenderClear(renderer);

	// Render the changes above ( which up until now had just happened behind the scenes )
	SDL_RenderPresent(renderer);

	
	space.set_window_size(sizeX, sizeY);
	space.set_renderer(renderer);


	assets.load();
	assets.setup_textures(renderer);

	return 0;
}*/

void update_state() {
	engine.tick();
}

void draw() {
	engine.draw();
}


void handle_input() {
	/*
	if( event.type == SDL_KEYDOWN )
	{
		switch( event.key.keysym.sym )
		{
			case SDLK_UP: yVel -= DOT_HEIGHT / 2; break;
			case SDLK_DOWN: yVel += DOT_HEIGHT / 2; break;
			case SDLK_LEFT: xVel -= DOT_WIDTH / 2; break;
			case SDLK_RIGHT: xVel += DOT_WIDTH / 2; break;
		}
	}
	else if( event.type == SDL_KEYUP )
	{
		switch( event.key.keysym.sym )
		{
			case SDLK_UP: yVel += DOT_HEIGHT / 2; break;
			case SDLK_DOWN: yVel -= DOT_HEIGHT / 2; break;
			case SDLK_LEFT: xVel += DOT_WIDTH / 2; break;
			case SDLK_RIGHT: xVel -= DOT_WIDTH / 2; break;
		}
	}

	x += xVel;

	if( ( x < 0 ) || ( x + DOT_WIDTH > SCREEN_WIDTH ) )
	{
		x -= xVel;
	}

	y += yVel;

	if( ( y < 0 ) || ( y + DOT_HEIGHT > SCREEN_HEIGHT ) )
	{
		y -= yVel;
	}
	*/
}

void run_game_loop() {
	bool quit = false;
	int loopMax = 600;
	int loopCount = 0;
	using clock = std::chrono::high_resolution_clock;
	using milliseconds = std::chrono::milliseconds;
	using std::chrono::duration_cast;

	auto start = clock::now(), end = clock::now();
	uint64_t diff = duration_cast<milliseconds>(end - start).count();

	auto accum_start = clock::now();
	while ((quit == false) && (loopCount < loopMax)) {
		start = clock::now();
		diff = duration_cast<milliseconds>(end - start).count();

		//Events
		while (SDL_PollEvent(&inputEvent))
		{
			handle_input();

			if (inputEvent.type == SDL_QUIT)
			{
				quit = true;
			}
		}

		if (duration_cast<chrono::nanoseconds>(clock::now() - accum_start).count() >= 16666666) {
			// do render updates every 60th of a second
			update_state();
			draw();

			accum_start = clock::now();
			loopCount++;
		}

		end = clock::now();
	}
}

int setup_render_loop() {
	int window_width = 1280;
	int window_height = 720;

	// Setup SDL
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0)
	{
		printf("Error: %s\n", SDL_GetError());
		return -1;
	}

	// Decide GL+GLSL versions
#if __APPLE__
	// GL 3.2 Core + GLSL 150
	const char* glsl_version = "#version 150";
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG); // Always required on Mac
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
#else
	// GL 3.0 + GLSL 130
	const char* glsl_version = "#version 130";
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
#endif

	// Create window with graphics context
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
	SDL_DisplayMode current;
	SDL_GetCurrentDisplayMode(0, &current);
	SDL_Window* window = SDL_CreateWindow("Dear ImGui SDL2+OpenGL3 example", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, window_width, window_height, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
	SDL_GLContext gl_context = SDL_GL_CreateContext(window);
	SDL_GL_SetSwapInterval(1); // Enable vsync

	// Initialize OpenGL loader
#if defined(IMGUI_IMPL_OPENGL_LOADER_GL3W)
	bool err = gl3wInit() != 0;
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLEW)
	bool err = glewInit() != GLEW_OK;
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLAD)
	bool err = gladLoadGL() == 0;
#else
	bool err = false; // If you use IMGUI_IMPL_OPENGL_LOADER_CUSTOM, your loader is likely to requires some form of initialization.
#endif
	if (err)
	{
		fprintf(stderr, "Failed to initialize OpenGL loader!\n");
		return 1;
	}

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls

	// Setup Platform/Renderer bindings
	ImGui_ImplSDL2_InitForOpenGL(window, gl_context);
	ImGui_ImplOpenGL3_Init(glsl_version);

	// Setup Style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsClassic();

	// Load Fonts
	// - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them. 
	// - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple. 
	// - If the file cannot be loaded, the function will return NULL. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
	// - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
	// - Read 'misc/fonts/README.txt' for more instructions and details.
	// - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
	//io.Fonts->AddFontDefault();
	//io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
	//io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
	//io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
	//io.Fonts->AddFontFromFileTTF("../../misc/fonts/ProggyTiny.ttf", 10.0f);
	//ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
	//IM_ASSERT(font != NULL);

	bool show_demo_window = true;
	bool show_another_window = false;
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	engine = GameEngine(window, window_width, window_height);
	engine.init();

	// Main loop
	bool done = false;
	while (!done)
	{
		// Poll and handle events (inputs, window resize, etc.)
		// You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
		// - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application.
		// - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application.
		// Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			ImGui_ImplSDL2_ProcessEvent(&event);
			if (event.type == SDL_QUIT)
				done = true;
			if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(window))
				done = true;
		}

		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplSDL2_NewFrame(window);
		ImGui::NewFrame();

		// 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
		if (show_demo_window)
			ImGui::ShowDemoWindow(&show_demo_window);

		// 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
		{
			static float f = 0.0f;
			static int counter = 0;

			ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

			ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
			ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
			ImGui::Checkbox("Another Window", &show_another_window);

			ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f    
			ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

			if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
				counter++;
			ImGui::SameLine();
			ImGui::Text("counter = %d", counter);

			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			ImGui::End();
		}

		// 3. Show another simple window.
		if (show_another_window)
		{
			ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
			ImGui::Text("Hello from another window!");
			if (ImGui::Button("Close Me"))
				show_another_window = false;
			ImGui::End();
		}
		engine.tick();
		engine.draw();

		// Rendering
		ImGui::Render();
		SDL_GL_MakeCurrent(window, gl_context);
		glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
		glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);


	//glEnable(GL_DEPTH_TEST);
	//glDepthFunc(GL_LESS);

	//glClearColor(1.0, 0.0, 0.0, 1.0);

		glClear(GL_COLOR_BUFFER_BIT);
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		SDL_GL_SwapWindow(window);
	}

	engine.stop();

	// Cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	SDL_GL_DeleteContext(gl_context);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}

int main(int argc, char* args[])
{	
	int setupResult = 0;
	/*
	setupResult = setup_renderer();

	if (setupResult != 0) {
		return setupResult;
	}

	initialize_map();
	run_game_loop();
	*/
	


	//setupResult = setup_imgui_test();

	//engine.init();
	setupResult = setup_render_loop();
	
	return setupResult;
}