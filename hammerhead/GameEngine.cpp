#include "GameEngine.h"

GameEngine::GameEngine() : window(nullptr), window_width(0), window_height(0) {

}

GameEngine::GameEngine(SDL_Window* window, int window_width, int window_height) : window(window), window_width(window_width), window_height(window_height) {

}

void GameEngine::init_assets() {
	assets.load();
	assets.setup_models();
}

void GameEngine::init_space() {
	this->space = Space(SPACE_SIZE, SPACE_SIZE);

	space.set_window_size(window_width, window_height);
}

void GameEngine::init() {
	init_assets();
	init_space();
	init_map();

	//GLuint vao, vbo[2];

	int is_compiled_vs, is_compiled_fs;
	int is_linked;
	int max_length;
	char* vertex_info_log;
	char* fragment_info_log;
	char* shader_program_info_log;

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
	//GLchar* vertex_source;
	//GLchar* fragment_source;




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
	//vertex_source = this->vertex_shader_source.c_str();
	//fragment_source = this->fragment_shader_source.c_str();

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
}

char* GameEngine::file_read(const char* filename) {
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

void GameEngine::draw() {

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

	int i = space.get_tick_count();


	//Load the identity matrix into model matrix, rotate the model, and move it back by 5
	//memcpy(model_matrix, identity_matrix, sizeof(GLfloat) * 16);
	//glClearColor(0.0, 0.0, 0.0, 1.0);
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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
	//SDL_GL_SwapWindow(this->window);

}

/*

void GameEngine::draw() {
	
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

	int i = space.get_tick_count();


	//Load the identity matrix into model matrix, rotate the model, and move it back by 5
	//memcpy(model_matrix, identity_matrix, sizeof(GLfloat) * 16);
	//glClearColor(0.0, 0.0, 0.0, 1.0);
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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
	//SDL_GL_SwapWindow(this->window);
	
}*/

void GameEngine::stop() {
	//Cleanup all the things we bound and allocated
	glUseProgram(0);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDetachShader(shader_program, vertex_shader);
	glDetachShader(shader_program, fragment_shader);
	glDeleteProgram(shader_program);
	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);
	//glDeleteBuffers(2, vbo);
	//glDeleteVertexArrays(1, &vao);
	//free(vertex_source);
	//free(fragment_source);
}

void GameEngine::set_fragment_shader_source(string value) {
	this->fragment_shader_source = value;
}

void GameEngine::set_vertex_shader_source(string value) {
	this->vertex_shader_source = value;
}

void GameEngine::init_map() {
	Coord sun_pos(320, 320);
	float sun_radius = 50.0;
	unique_ptr<SpaceObject> sun = CelestialBodyFactory::create_sun("Sun", sun_pos, sun_radius, assets.get_asset("sphere"), Color(251, 184, 41));

	Coord earth_pos(140, 140);
	float earth_radius = 18.0;
	unique_ptr<SpaceObject> earth = CelestialBodyFactory::create_planet("Earth", earth_pos, earth_radius, assets.get_asset("sphere"), Color(42, 143, 189));
	Orbit* sun_orbit = new Orbit(sun.get(), 2);
	earth->set_orbit(sun_orbit);

	Coord moon_pos(240, 240);
	float moon_radius = 4.5;
	unique_ptr<SpaceObject> moon = CelestialBodyFactory::create_moon("Moon", moon_pos, moon_radius, assets.get_asset("sphere"), Color(216, 216, 216));
	Orbit* earth_orbit = new Orbit(earth.get(), 5);
	moon->set_orbit(earth_orbit);

	Coord ship_pos(300, 300);
	Vector2 ship_direction(1, 1);
	Vector2 ship_size(8, 8);
	unique_ptr<Ship> ship = ShipFactory::create("intrepid", ship_pos, ship_size, assets.get_asset("ship-1"));
	ship->set_direction(ship_direction);

	space.add_object(move(sun));
	space.add_object(move(earth));
	space.add_object(move(moon));
	space.add_object(move(ship));
}

void GameEngine::tick() {
	space.tick();
}
