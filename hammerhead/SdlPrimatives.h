#pragma once
#include <SDL2/SDL.h>

typedef int32_t s32;

class SdlPrimatives
{
public:
	static void draw_circle(SDL_Renderer* renderer, s32 _x, s32 _y, s32 radius)
	{
		s32 x = radius - 1;
		s32 y = 0;
		s32 tx = 1;
		s32 ty = 1;
		s32 err = tx - (radius << 1); // shifting bits left by 1 effectively
									  // doubles the value. == tx - diameter

		
		SDL_SetRenderDrawColor(renderer, 236, 205, 53, 255);
		//SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

		while (x >= y)
		{
			//  Each of the following renders an octant of the circle
			SDL_RenderDrawPoint(renderer, _x + x, _y - y);
			SDL_RenderDrawPoint(renderer, _x + x, _y + y);
			SDL_RenderDrawPoint(renderer, _x - x, _y - y);
			SDL_RenderDrawPoint(renderer, _x - x, _y + y);
			SDL_RenderDrawPoint(renderer, _x + y, _y - x);
			SDL_RenderDrawPoint(renderer, _x + y, _y + x);
			SDL_RenderDrawPoint(renderer, _x - y, _y - x);
			SDL_RenderDrawPoint(renderer, _x - y, _y + x);

			if (err <= 0)
			{
				y++;
				err += ty;
				ty += 2;
			}
			if (err > 0)
			{
				x--;
				tx += 2;
				err += tx - (radius << 1);
			}
		}
	}


private:
	SdlPrimatives() {}
};

