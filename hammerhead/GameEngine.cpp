#include "GameEngine.h"

GameEngine::GameEngine() : window(nullptr), window_width(0), window_height(0) {

}

GameEngine::GameEngine(SDL_Window* window, int window_width, int window_height) : window(window), window_width(window_width), window_height(window_height) {

}

void GameEngine::init_assets() {
	assets.load();
	assets.setup_models();
}

void GameEngine::init_settings() {
	this->show_axes = true;
	this->is_camera_moving = false;
	this->is_camera_panning = false;

	this->invert_pan = false;
	this->pan_speed = 1.0f;
	this->pan_factor = 1.0f;
}

void GameEngine::input_camera_move_start()
{
	this->is_camera_moving = true;
	this->is_camera_panning = false;
}

void GameEngine::input_camera_move_end()
{
	this->is_camera_moving = false;
	this->is_camera_panning = false;
}

void GameEngine::input_camera_pan_start()
{
	this->is_camera_moving = true;
	this->is_camera_panning = true;
}

void GameEngine::input_camera_pan_end()
{
	this->is_camera_moving = false;
	this->is_camera_panning = false;
}

void GameEngine::input_camera_zoom_in()
{
	this->camera.zoom_in();
	this->calculate_pan_factor();
}

void GameEngine::input_camera_zoom_out()
{
	this->camera.zoom_out();
	this->calculate_pan_factor();
}

void GameEngine::input_move(float x, float y, float delta_x, float delta_y) {
	if (this->is_camera_panning) {
		glm::vec3 camera_position = this->camera.get_position();
		glm::vec3 camera_look_at = this->camera.get_look_at();

		this->camera.set_position(glm::vec3(camera_position.x + (this->pan_factor * delta_x), camera_position.y, camera_position.z + (this->pan_factor * delta_y)));
		this->camera.set_look_at(glm::vec3(camera_look_at.x + (this->pan_factor * delta_x), camera_look_at.y, camera_look_at.z + (this->pan_factor * delta_y)));
	}
	else if (this->is_camera_moving) {
		glm::vec3 camera_position = this->camera.get_position();

		this->camera.set_position(glm::vec3(camera_position.x + (this->pan_factor * delta_x), camera_position.y, camera_position.z + (this->pan_factor * delta_y)));
	}
}

void GameEngine::set_invert_pan(bool value)
{
	this->invert_pan = value;
	calculate_pan_factor();
}

void GameEngine::init_camera()
{
	this->camera = Camera();

	this->camera.set_aspect_width(this->window_width);
	this->camera.set_aspect_height(this->window_height);
	

	this->orientation_axes = OrientationAxes();
	this->orientation_axes.set_camera(this->camera);
}

void GameEngine::update_camera()
{
	this->camera.calculate_matrices();
	space.set_matrices(this->camera.get_projection(), this->camera.get_view());

	if (this->show_axes) {
		this->orientation_axes.set_camera(this->camera);
		this->orientation_axes.update();
	}
}

void GameEngine::init_space() {
	this->space = Space(SPACE_SIZE, SPACE_SIZE);

	space.set_window_size(window_width, window_height);
}

void GameEngine::bind_models() {
	assets.bind_models();
}

void GameEngine::calculate_pan_factor()
{
	if (this->invert_pan) {
		this->pan_factor = -1.0f * this->pan_speed;
	}
	else {
		this->pan_factor = this->pan_speed;
	}

	this->pan_factor = this->pan_factor * (1.0f / this->camera.get_zoom());
}

void GameEngine::draw_axes()
{
	this->orientation_axes.draw();
	/*
	glColor3f(1.0, 0.0, 0.0); // red x
	glBegin(GL_LINES);
	// x aix

	glVertex3f(-4.0, 0.0f, 0.0f);
	glVertex3f(4.0, 0.0f, 0.0f);

	// arrow
	glVertex3f(4.0, 0.0f, 0.0f);
	glVertex3f(3.0, 1.0f, 0.0f);

	glVertex3f(4.0, 0.0f, 0.0f);
	glVertex3f(3.0, -1.0f, 0.0f);
	glEnd();
	glFlush();



	// y 
	glColor3f(0.0, 1.0, 0.0); // green y
	glBegin(GL_LINES);
	glVertex3f(0.0, -4.0f, 0.0f);
	glVertex3f(0.0, 4.0f, 0.0f);

	// arrow
	glVertex3f(0.0, 4.0f, 0.0f);
	glVertex3f(1.0, 3.0f, 0.0f);

	glVertex3f(0.0, 4.0f, 0.0f);
	glVertex3f(-1.0, 3.0f, 0.0f);
	glEnd();
	glFlush();

	// z 
	glColor3f(0.0, 0.0, 1.0); // blue z
	glBegin(GL_LINES);
	glVertex3f(0.0, 0.0f, -4.0f);
	glVertex3f(0.0, 0.0f, 4.0f);

	// arrow
	glVertex3f(0.0, 0.0f, 4.0f);
	glVertex3f(0.0, 1.0f, 3.0f);

	glVertex3f(0.0, 0.0f, 4.0f);
	glVertex3f(0.0, -1.0f, 3.0f);
	glEnd();
	glFlush();
	*/
}

void GameEngine::init_shaders() {
	int is_compiled_vs, is_compiled_fs;
	int is_linked;
	int max_length;
	char* vertex_info_log;
	char* fragment_info_log;
	char* shader_program_info_log;

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
	glBindAttribLocation(shader_program, 2, "in_Normal");
	glBindAttribLocation(shader_program, 3, "in_TextureCoord");

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

void GameEngine::update() {
	space.tick();
	this->update_camera();
}

void GameEngine::init()
{
	init_settings();
	init_assets();
	init_camera();
	init_space();
	init_map();
	bind_models();
	init_shaders();
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
	space.set_shader_program(this->shader_program);
	space.draw_map();

	if (this->show_axes) {
		draw_axes();
	}
}

void GameEngine::dispose() {
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

void GameEngine::set_show_axes(bool value)
{
	this->show_axes = value;
}

void GameEngine::set_pan_speed(float value)
{
	this->pan_speed = value;
	calculate_pan_factor();
}

void GameEngine::set_fragment_shader_source(string value) {
	this->fragment_shader_source = value;
}

void GameEngine::set_vertex_shader_source(string value) {
	this->vertex_shader_source = value;
}

void GameEngine::init_map() {
	Coord sun_pos(0, 0);
	float sun_radius = 50.0;
	unique_ptr<SpaceObject> sun = CelestialBodyFactory::create_sun("Sun", sun_pos, sun_radius, assets.get_asset("sphere"), Color(251, 184, 41));
	sun->set_rotation(glm::vec3(0, 0.01, 0));

	Coord earth_pos(280, 0);
	float earth_radius = 18.0;
	unique_ptr<SpaceObject> earth = CelestialBodyFactory::create_planet("Earth", earth_pos, earth_radius, assets.get_asset("sphere"), Color(42, 143, 189));
	Orbit* sun_orbit = new Orbit(sun.get(), 2);
	earth->set_orbit(sun_orbit);
	earth->set_rotation(glm::vec3(0, 0.05, 0));

	Coord moon_pos(380, 0);
	float moon_radius = 4.5;
	unique_ptr<SpaceObject> moon = CelestialBodyFactory::create_moon("Moon", moon_pos, moon_radius, assets.get_asset("sphere"), Color(216, 216, 216));
	Orbit* earth_orbit = new Orbit(earth.get(), 5);
	moon->set_orbit(earth_orbit);
	moon->set_rotation(glm::vec3(0, 0.10, 0));

	Coord ship_pos(0, 0);
	Vector2 ship_direction(1, 1);
	Vector2 ship_size(20, 20);
	unique_ptr<Ship> ship = ShipFactory::create("intrepid", ship_pos, ship_size, assets.get_asset("ship-1"));
	ship->set_direction(ship_direction);

	space.add_object(move(sun));
	space.add_object(move(earth));
	space.add_object(move(moon));
	space.add_object(move(ship));

	space.set_window_size(window_width, window_height);
}
