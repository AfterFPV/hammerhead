#pragma once
#include <GL/gl3w.h>
#include <GL/glcorearb.h>
#include <vector>
#include "MaterialRange.h"

using namespace std;

typedef struct {
	GLuint vao_id;  // vertex array object id
	GLuint position_vbo;
	GLuint texture_coord_vbo;
	GLuint normal_vbo;
	GLuint color_vbo;
	int num_triangles;
	vector<MaterialRange> material_ranges;
} DrawObject;