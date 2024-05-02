#include "demo.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <math.h>
#include <stdio.h>

const int width = 720;
const int height = 480;
const int fov = 60;
int init_all(SDL_Instance *instance);
float calc_x_v(float p_pos, float angel, int size, int flag);
float calc_y_v(float p_pos, float angel, int ax, int p_posx, int flag, float y_a);
float calc_y_h(float p_pos, float angel, int size, int flag);
float calc_x_h(float p_pos, float angel, int ay, int p_posy, int flag, float x_a);
float absolute(float n);
float circ(float n, float min, float max);
int main()
{
	SDL_Texture *image = NULL;
	float direction = 0;
	float p_pos[] = {80, 128};
	int size = 32;
	float d_to_vp = (width/2) / tan( (fov/2) * M_PI / 180);
	int quit = 0;
	float angel_inc = 60.0/width;
	int x_coor = 0;
	SDL_Instance instance;
	int map[][10] = {
		{1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
		{1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
		{1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
		{1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
		{1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
		{1, 1, 0, 0, 0, 0, 1, 0, 0, 1},
		{1, 0, 0, 0, 0, 0, 1, 0, 0, 1},
		{1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
		{1, 1, 0, 0, 0, 0, 0, 0, 0, 1},
		{1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
	};

	if (init_all(&instance) != 0)
		return (1);

	image = load(instance.renderer, 2);
        if (image == NULL)
        {
		printf("image load failed\n");
			return (1);
        }
	while (!quit)
	{
		SDL_SetRenderDrawColor(instance.renderer, 0, 0, 0, 0);
		SDL_RenderClear(instance.renderer);
		float angel = circ(direction + 30, 0, 359);
		SDL_Event event;
		for (x_coor = 0; x_coor < width; x_coor++)
		{
			float a_x, a_y, a_x_h, a_y_h, x_a, y_a, wall_distance = 10000, wall_distance_h = 10000, wall_distance_min = 10000, a_y_a, a_x_a;
			int b = 0, x_cor, y_cor, x_cor_h, y_cor_h, x_cor_v, y_cor_v, flag = 0;
			a_y_h = calc_y_h(p_pos[1], angel, size, 1);
			a_x_h = calc_x_h(p_pos[0], angel, a_y_h, p_pos[1], 1, 0);
			a_x = calc_x_v(p_pos[0], angel, size, 1);
			a_y = calc_y_v(p_pos[1], angel, a_x, p_pos[0], 1, 0);
			//printf("a_x: %f, a_y: %f\n", a_x, a_y);
			x_a = (float)size / absolute(tan(angel * M_PI / 180));
			if (angel == 90 || angel == 270)
				y_a = size;
			else
				y_a = (float)size * absolute(tan(angel * M_PI / 180));
			//printf("angel: %f\n", angel);
			wall_distance = absolute(a_x - p_pos[0]) / absolute(cos(angel * M_PI / 180));
			wall_distance_h = absolute(a_y_h - p_pos[1]) / absolute(sin(angel * M_PI / 180));
			while (b < 10)
			{
				x_cor_h = a_x_h / size;
				y_cor_h = a_y_h / size;
				x_cor_v = a_x / size;
				y_cor_v = a_y / size;
				if (wall_distance > 0 && wall_distance < wall_distance_min && x_cor_v <= 9 && x_cor_v >= 0 && y_cor_v >= 0 && y_cor_v <= 9 && map[x_cor_v][y_cor_v] == 1)
				{
					x_cor = x_cor_v;
					y_cor = y_cor_v;
					wall_distance_min = wall_distance;
					flag = 1;
					//printf("p_pos: %f, a_x: %f, absolute(a_x - p_pos): %f\n", p_pos[0], a_x, absolute(a_x - p_pos[0]));
					a_y_a = a_y;
					a_x_a = a_x;
				}
				if (wall_distance_h > 0 && wall_distance_h < wall_distance_min && x_cor_h <= 9 && x_cor_h >= 0 && y_cor_h >= 0 && y_cor_h <= 9 && map[x_cor_h][y_cor_h] == 1)
				{
					x_cor = x_cor_h;
                                        y_cor = y_cor_h;
					wall_distance_min = wall_distance_h;
					flag = 2;
					a_y_a = a_y_h;
					a_x_a = a_x_h;
				}
				a_y_h = calc_y_h(a_y_h, angel, size, 2);
				a_x_h = calc_x_h(a_x_h, angel, a_y_h, 0, 2, x_a);
				a_x = calc_x_v(a_x, angel, size, 2);
                                a_y = calc_y_v(a_y, angel, a_x, 0, 2, y_a);
				wall_distance = absolute(a_x - p_pos[0]) / absolute(cos(angel * M_PI / 180));
				wall_distance_h = absolute(a_y_h - p_pos[1]) / absolute(sin(angel * M_PI / 180));
				b++;
			}
			//printf("wall cell: (%d, %d)\n", x_cor, y_cor);
			//printf("wall distance: %f\n", wall_distance_min);
			if (x_cor <= 9 && x_cor >= 0 && y_cor >= 0 && y_cor <= 9 && map[x_cor][y_cor] == 1)
			{
				//printf("wall_distance: %f\n", wall_distance_min);
				/*printf("wall cell number: (%f, %f)\n", a_x/size, a_y/size);*/
				wall_distance_min = wall_distance_min * absolute(cos((absolute(direction - (absolute(angel)))) * M_PI / 180));
                                //printf("angel: %f, wall_distance: %f\n", angel, wall_distance_min);
                                float rec_h = size * d_to_vp / wall_distance_min;
                                float rec_y = (height / 2) - (rec_h / 2);
				//printf("wall_distance: %f, height of wall: %f\n", wall_distance_min, rec_h);
                                SDL_Rect column = {x_coor, rec_y, 1, rec_h};
				int pic_w = 640/ size;
				SDL_Rect pic = { 0, 0, 1, 32};
				if(flag == 1)
				{
					int pic_x = a_y_a - (((int)a_y_a / size) * size);
					pic.x = pic_x;
				}
				else
				{
					int pic_x = a_x_a - (((int)a_x_a / size) * size);
                                        pic.x = pic_x;
				}
				//printf("x_coor:%d, pic.x: %d\n",x_coor, pic.x);

                                SDL_RenderCopy(instance.renderer, image, &pic, &column);
                        }
			//printf("-----------------------\n");
			angel = circ(angel - angel_inc, 0, 359);
		}
		SDL_RenderPresent(instance.renderer);
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
				quit = 1;
			if (event.type == SDL_KEYDOWN)
			{
				SDL_KeyboardEvent key;
				key = event.key;
				if (key.keysym.scancode == 79)
					direction = circ(direction - 22.5, 0, 359);
				if (key.keysym.scancode == 80)
					direction = circ(direction + 22.5, 0, 359);
				if (key.keysym.scancode == 82)
				{
                                        p_pos[0] += 2 * cos(direction * M_PI / 180);
					p_pos[1] -= 2 * sin(direction * M_PI / 180);
				}
				if (key.keysym.scancode == 81)
                                {
					p_pos[0] -= 2 * cos(direction * M_PI / 180);
                                        p_pos[1] += 2 * sin(direction * M_PI / 180);
				}
				if (key.keysym.scancode == 44)
				{
					printf("x_pos: %f, y_pos: %f\n", p_pos[0], p_pos[1]);
					printf("direction: %f\n", direction);
					printf("--------------------------\n");
				}
			}
		}
	}
	SDL_DestroyTexture(image);
	SDL_DestroyRenderer(instance.renderer);
        SDL_DestroyWindow(instance.window);
        SDL_Quit();
	return (0);
}

int init_all(SDL_Instance *instance)
{
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		printf("initialization failed\n");
		return (1);
	}
	instance->window = SDL_CreateWindow("demo", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, 0);
	if (instance->window == NULL)
	{
		printf("window creation failed\n");
		return (1);
	}
	instance->renderer = SDL_CreateRenderer(instance->window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (instance->renderer == NULL)
	{
		printf("renderer creation failed\n");
		return (1);
	}
	return (0);
}
float calc_x_v(float p_pos, float angel, int size, int flag)
{
	if (flag == 1)
	{
		if (angel > 90 && angel < 270)
			return (((int)p_pos / size) * size - 1);
		else
			return (((int)p_pos / size) * size + size);
	}
	else
	{
		if (angel > 90 && angel < 270)
			return (p_pos - size);
		else
			return (p_pos + size);
	}
}

float calc_y_h(float p_pos, float angel, int size, int flag)
{
	if (flag == 1)
	{
		if (angel >= 0 && angel <= 180)
			return (((int)p_pos / size) * size - 1);
		else
			return (((int)p_pos / size) * size + size);
	}
	else
	{
		if (angel >= 0 && angel <= 180)
			return (p_pos - size);
		else
			return (p_pos + size);
	}
}
float calc_y_v(float p_pos, float angel, int ax, int p_posx, int flag, float y_a)
{
		if (flag == 1)
		{
			if (angel == 90 || angel == 270)
				return (p_pos);
			else if (angel >= 0 && angel <= 180)
				return (p_pos - (absolute(ax - p_posx)) * absolute(tan(angel * M_PI / 180)));
			else
				return (p_pos + (absolute(ax - p_posx)) * absolute(tan(angel * M_PI / 180)));
		}
		else
		{
			if (angel >= 0 && angel <= 180)
				return (p_pos - y_a);
			else
				return (p_pos + y_a);
		}

}
float calc_x_h(float p_pos, float angel, int ax, int p_posx, int flag, float x_a)
{
                if (flag == 1)
                {
                        if (angel > 90 && angel < 270)
                                return (p_pos - (absolute(ax - p_posx)) / absolute(tan(angel * M_PI / 180)));
                        else
                                return (p_pos + (absolute(ax - p_posx)) / absolute(tan(angel * M_PI / 180)));
                }
                else
                {
                        if (angel > 90 && angel < 270)
                                return (p_pos - x_a);
                        else
                                return (p_pos + x_a);
                }

}
float circ(float n, float min, float max)
{
	if (n > max)
		n = n - (max + 1);
	if (n < min)
		n = n + (max + 1);
	return n;
}
float absolute(float n)
{
	if (n < 0)
		return (n * -1);
	return n;
}
