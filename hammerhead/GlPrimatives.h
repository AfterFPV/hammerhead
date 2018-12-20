#pragma once
#pragma once
#include "Color.h"
#include <math.h>
#include <vector>
#include <GL/gl3w.h>
#include <GL/glcorearb.h>

using namespace std;

class GlPrimatives
{
public:
	static void draw_circle(float cx, float cy, float r, Color color, int num_segments)
	{
		float radius = 0.5;
		std::vector<float> vertexBuffer;
		for (double i = 0; i < 2 * M_PI; i += 2 * M_PI / num_segments) {
			vertexBuffer.push_back(cos(i) * radius);    //X coordinate
			vertexBuffer.push_back(sin(i) * radius);    //Y coordinate
			vertexBuffer.push_back(0.0);                //Z coordinate
		}
	}


private:
	GlPrimatives() {}
};
