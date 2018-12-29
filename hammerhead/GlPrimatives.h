#pragma once
#pragma once
#include "Color.h"
#include <math.h>
#include <vector>
#include <SDL2/SDL.h>
#include <GL/gl3w.h>
#include <GL/glcorearb.h>
#include "DrawObject.h"

//constexpr double M_PI = 3.14159265358979323846;

using namespace std;

class GlPrimatives
{
public:
	static DrawObject register_circle(float cx, float cy, float radius, float width, Color color, int num_segments)
	{
		DrawObject o;
		float outer_radius = radius;
		float inner_radius = radius - width;
		float color_r = (float)color.r / 255.0f;
		float color_g = (float)color.g / 255.0f;
		float color_b = (float)color.b / 255.0f;
		vector<float> v_positions;
		vector<float> v_colors;
		float phase_increment = 2.0f * M_PI / (float)num_segments;

		for (int i = 0; i < num_segments; i++) {
			float theta_start = i * phase_increment; // get the current angle
			float theta_end = (i + 1) * phase_increment; // get the current angle

			//each section has four vertices and two triangles
			float cos_theta_start = cos(theta_start);
			float cos_theta_end = cos(theta_end);
			float sin_theta_start = sin(theta_start);
			float sin_theta_end = sin(theta_end);

			float out_x_start = cx + outer_radius * cos_theta_start;
			float out_x_end = cx + outer_radius * cos_theta_end;
			float in_x_start = cx + cos_theta_start * inner_radius;
			float in_x_end = cx + cos_theta_end * inner_radius;

			float out_y_start = cy + sin_theta_start * outer_radius;
			float out_y_end = cy + sin_theta_end * outer_radius;
			float in_y_start = cy + sin_theta_start * inner_radius;
			float in_y_end = cy + sin_theta_end * inner_radius;

			//Create two triangles from the four vertex positions
			//Triangle 1				
			v_positions.push_back(out_x_start);
			v_positions.push_back(out_y_start);
			v_positions.push_back(0.0);
			v_positions.push_back(in_x_start);
			v_positions.push_back(in_y_start);
			v_positions.push_back(0.0);
			v_positions.push_back(out_x_end);
			v_positions.push_back(out_y_end);
			v_positions.push_back(0.0);			
			
			//Triangle 2
			v_positions.push_back(out_x_end);
			v_positions.push_back(out_y_end);
			v_positions.push_back(0.0);
			v_positions.push_back(in_x_start);
			v_positions.push_back(in_y_start);
			v_positions.push_back(0.0);
			v_positions.push_back(in_x_end);
			v_positions.push_back(in_y_end);
			v_positions.push_back(0.0);			

			int color_vertex_count = 6;
			for (int j = 0; j < color_vertex_count; j++) {
				v_colors.push_back(color_r);
				v_colors.push_back(color_g);
				v_colors.push_back(color_b);
			}
		}

		//Allocate and assign a vertex array object to our handle
		glGenVertexArrays(1, &o.vao_id);

		//Bind our vertex array object as the current used object
		glBindVertexArray(o.vao_id);

		//Allocate and assign to vertex buffer object to our handle
		GLuint vbo[2];
		glGenBuffers(2, vbo);

		//Setup position attributes
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
		glBufferData(GL_ARRAY_BUFFER, v_positions.size() * sizeof(float), &v_positions.at(0), GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);


		//Setup color attributes
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
		glBufferData(GL_ARRAY_BUFFER, v_colors.size() * sizeof(float), &v_colors.at(0), GL_STATIC_DRAW);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);


		o.position_vbo = vbo[0];
		o.color_vbo = vbo[1];
		o.num_triangles = v_positions.size() / 3;

		return o;
	}

private:
	GlPrimatives() {}
};
