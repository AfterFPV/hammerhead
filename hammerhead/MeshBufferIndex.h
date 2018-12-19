#pragma once
#include <string>
#include <GL/gl3w.h>
#include <GL/glcorearb.h>

using namespace std;

class MeshBufferIndex
{
public:
	string name;
	GLuint vao;
	GLuint position_vbo;
	GLuint texture_coord_vbo;
	GLuint normal_vbo;
	GLuint index_vbo;
};