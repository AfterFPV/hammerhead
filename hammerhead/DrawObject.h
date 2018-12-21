#pragma once
#include <GL/gl3w.h>
#include <GL/glcorearb.h>

typedef struct {
	GLuint vao_id;  // vertex array object id
	GLuint position_vbo;
	GLuint texture_coord_vbo;
	GLuint normal_vbo;
	GLuint color_vbo;
	int num_triangles;
	size_t material_id;
} DrawObject;