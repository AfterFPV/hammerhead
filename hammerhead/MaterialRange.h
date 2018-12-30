#pragma once

#pragma once
#include <GL/gl3w.h>
#include <GL/glcorearb.h>

typedef struct {
	int triangle_start = 0;
	int triangle_end = 0;
	GLuint texture_id = 0;
	size_t material_id = 0;
} MaterialRange;
