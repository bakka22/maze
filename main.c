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
	SDL_Texture *weapon = NULL;
	SDL_Texture *enemy = NULL;
	float direction = 0;
	float e_pos[] = {240, 112};
	float p_pos[] = {80, 128};
	int e_w, e_h;
	int size = 32;
	float d_to_vp = (width/2) / tan( (fov/2) * M_PI / 180);
	int quit = 0;
	SDL_Texture *wep_list[2][2];
	SDL_Texture *rain[10];
	int wep_num = 0;
	int wep_zig = 4, wep_sh = 0;
	float angel_inc = 60.0/width;
	double e_angel;
	int x_coor = 0;
	int enemy_flag = 1;
	int velo = 0, h_velo = 0;
	int view = 1;
	int r = 1, rain_c = 0;
	SDL_Instance instance;
	int a, b, c = 1;
	char keyST[256] = {0};
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
	for (a = 0; a <= 1; a++)
	{
		for (b = 0; b <= 1; b++)
		{
			wep_list[a][b] = load(instance.renderer, c);
			c++;
		}
	}
	for(a = 0; a < 10; a++)
	{
		rain[a] = load(instance.renderer, c);
		c++;
	}
	image = load(instance.renderer, 0);
	weapon = wep_list[0][0];
        if (image == NULL || weapon == NULL)
        {
		printf("image load failed\n");
			return (1);
        }
	enemy = load(instance.renderer, c);
	
	SDL_QueryTexture(enemy, NULL, NULL, &e_w, &e_h);
	while (!quit)
	{
		enemy_flag = 1;
		int e_x_coor = width/2;
		float e_dist;
		SDL_SetRenderDrawColor(instance.renderer, 4, 24, 92, 0);
		SDL_RenderClear(instance.renderer);
		SDL_Rect floor = {0, height / 2, width, height / 2};
		SDL_Rect cut = {50, 50, 1, 1};
		SDL_SetRenderDrawColor(instance.renderer, 12, 61, 2, 0);
		SDL_RenderFillRect(instance.renderer, &floor);
		float angel = circ(direction + 30, 0, 359);
		float e_tan;
		SDL_Event event;
		int x_cor_p = 0, y_cor_p = 0;
		float x_e_p, y_e_p;

		x_e_p = absolute(p_pos[0] - e_pos[0]);
		y_e_p = absolute(p_pos[1] - e_pos[1]);
		e_tan = y_e_p / x_e_p;
		e_angel = atan(e_tan) * 180 / M_PI;
		if (p_pos[0] > e_pos[0] && p_pos[1] > e_pos[1])
			e_angel = circ(180 - e_angel, 0, 359);
		else if (p_pos[0] < e_pos[0] && p_pos[1] < e_pos[1])
			e_angel = circ(360 - e_angel, 0, 359);
		else if (p_pos[0] > e_pos[0] && p_pos[1] < e_pos[1])
			e_angel = circ(e_angel + 180, 0, 359);
		printf("e_tan: %f\n", e_tan);
		printf("x_e_p: %f, y_e_p: %f\n", x_e_p, y_e_p);
		printf("e_angel: %f\n", e_angel);
		e_dist = sqrt((x_e_p * x_e_p) + (y_e_p * y_e_p));
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
				int e_cal = 1;
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
					if (x_coor == width/2 - 1)
					{
						y_cor_p = a_y;
                                        	x_cor_p = a_x;
					}
					
				}
				if (wall_distance_h > 0 && wall_distance_h < wall_distance_min && x_cor_h <= 9 && x_cor_h >= 0 && y_cor_h >= 0 && y_cor_h <= 9 && map[x_cor_h][y_cor_h] == 1)
				{
					x_cor = x_cor_h;
                                       	y_cor = y_cor_h;
					wall_distance_min = wall_distance_h;
					flag = 2;
					a_y_a = a_y_h;
					a_x_a = a_x_h;
					if (x_coor == width/2 - 1)
					{
						y_cor_p = a_y_h;
                                        	x_cor_p = a_x_h;
					}
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
				SDL_Rect pic = { 0, 0, 28, 630};
				if(flag == 1)
				{
					int pic_x = (a_y_a - (((int)a_y_a / size) * size)) * 28;
					pic.x = pic_x;
				}
				else
				{
					int pic_x = (a_x_a - (((int)a_x_a / size) * size)) * 28;
                                        pic.x = pic_x;
				}
				//printf("x_coor:%d, pic.x: %d\n",x_coor, pic.x);

                                SDL_RenderCopy(instance.renderer, image, &pic, &column);
                        }
			//printf("-----------------------\n");
			if (e_angel >= angel && e_angel < angel + angel_inc && e_dist < wall_distance_min)
			{
				e_x_coor = x_coor;
				enemy_flag = 0;
				printf("e_angel: %f, angel: %f, e_x_coor: %d\n", e_angel, angel, e_x_coor);
			}
			angel = circ(angel - angel_inc, 0, 359);
		}
		if(!enemy_flag)
		{
			float e_rec_h = size * d_to_vp / e_dist;
			float e_y = (height / 2) - (e_rec_h / 2);
			float e_rec_w = size * d_to_vp / e_dist;
			printf("e_h: %d, e_w: %d\n", e_h, e_w);
			printf("e_rec_h: %f, e_rec_w: %f\n", e_rec_h, e_rec_w);
			SDL_Rect e_rec = {e_x_coor, e_y, e_rec_w, e_rec_h};
			SDL_RenderCopy(instance.renderer, enemy, NULL, &e_rec);
		}
		if (r)
		{
			SDL_RenderCopy(instance.renderer, rain[rain_c/3], NULL, NULL);
			rain_c = circ(rain_c + 1, 0, 29);
		}
		if (view)
		{
			SDL_Rect viewport = {0, 0, width / 4, height / 4};
			//printf("w: %d, h: %d\n", viewport.w, viewport.h);
			float x_c = (float)viewport.w/10.0;
                        float y_c = (float)viewport.h/10.0;
			SDL_RenderSetViewport(instance.renderer, &viewport);
			for (int x = 0; x < 10; x++)
			{
				for (int y = 0; y < 10; y++)
				{
					SDL_Rect block = {x * x_c , y * y_c, x_c, y_c};
					if (map[x][y] == 1)
						SDL_SetRenderDrawColor(instance.renderer, 0, 0, 0, 0);
					else
						SDL_SetRenderDrawColor(instance.renderer, 255, 255, 255, 0);
					SDL_RenderFillRect(instance.renderer, &block);
					
				}
			}
			float p_x = (p_pos[0] / 32.0) * x_c;
			float p_y = (p_pos[1] / 32.0) * y_c;
			SDL_Rect p = {p_x - 2, p_y - 2, 5, 5};
			SDL_SetRenderDrawColor(instance.renderer, 255, 0, 0, 0);
			SDL_RenderFillRect(instance.renderer, &p);
			//printf("x_cor: %d, y_cor: %d\n", x_cor_p, y_cor_p);
			float r_x = ((float)x_cor_p / 32.0) * x_c;
			float r_y = ((float)y_cor_p / 32.0) * y_c;
			SDL_RenderDrawLine(instance.renderer, p_x, p_y, r_x, r_y);
			SDL_RenderSetViewport(instance.renderer, NULL);
		}
		if (wep_sh == 0)
		{
			weapon = wep_list[wep_num][0];
                        wep_zig = 8;
		}
		else
		{
			weapon = wep_list[wep_num][wep_zig/8];
                        wep_zig = circ(wep_zig + 1, 0, 15);
		}
		SDL_Rect weap = {width/2 - 80, height/2, width/2, height / 2};
		SDL_RenderCopy(instance.renderer, weapon, NULL, &weap);
		SDL_RenderPresent(instance.renderer);
		p_pos[0] += velo * cos(direction * M_PI / 180);
		int map_x = (int)p_pos[0] / size;
                int map_y = (int)p_pos[1] / size;
                if (map[map_x][map_y] == 1)
		{
			p_pos[0] -= velo * cos(direction * M_PI / 180);
		}
		p_pos[1] -= velo * sin(direction * M_PI / 180);
		map_x = (int)p_pos[0] / size;
                map_y = (int)p_pos[1] / size;
		if (map[map_x][map_y] == 1)
                {
                        p_pos[1] += velo * sin(direction * M_PI / 180);
                }
		p_pos[0] += h_velo * cos(circ(direction-90, 0, 359) * M_PI / 180);
		map_x = (int)p_pos[0] / size;
                map_y = (int)p_pos[1] / size;
		if (map[map_x][map_y] == 1)
                {
                        p_pos[0] -= h_velo * cos(circ(direction-90, 0, 359) * M_PI / 180);
                }

		p_pos[1] -= h_velo * sin(circ(direction-90, 0, 359) * M_PI / 180);
		map_x = (int)p_pos[0] / size;
                map_y = (int)p_pos[1] / size;
		if (map[map_x][map_y] == 1)
                {
                        p_pos[1] += h_velo * sin(circ(direction-90, 0, 359) * M_PI / 180);
                }
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
				quit = 1;
			if (event.type == SDL_KEYDOWN)
			{
				SDL_KeyboardEvent key;
				key = event.key;
				if (key.keysym.scancode == 16)
				{
					if (view)
						view = 0;
					else
						view = 1;
				}
				if (key.keysym.scancode == 79)
					direction = circ(direction - 22.5, 0, 359);
				if (key.keysym.scancode == 80)
					direction = circ(direction + 22.5, 0, 359);
				if (key.keysym.scancode == 82)
				{
					if (!keyST[82])
                                        	velo += 1;
					keyST[82] = 1;
				}
				if (key.keysym.scancode == 81)
                                {
					if (!keyST[81])
						velo -= 1;
					keyST[81] = 1;
				}
				if (key.keysym.scancode == 44)
				{
					wep_sh = 1;
				}
				if (key.keysym.scancode == 20)
				{
					wep_num = circ(wep_num + 1, 0, 1);
					weapon = wep_list[wep_num][0];
				}
				if (key.keysym.scancode == 26)
				{
					if (!keyST[26])
                                                velo += 1;
                                        keyST[26] = 1;
				}
				if (key.keysym.scancode == 22)
                                {
					if (!keyST[22])
                                                velo -= 1;
                                        keyST[22] = 1;
                                }
				if (key.keysym.scancode == 7)
                                {
					if (!keyST[7])
                                                h_velo += 1;
                                        keyST[7] = 1;
                                }
				if (key.keysym.scancode == 4)
                                {
					if (!keyST[4])
                                                h_velo -= 1;
                                        keyST[4] = 1;
                                }
				if (key.keysym.scancode == 21)
				{
					if (r == 0)
						r = 1;
					else
						r = 0;
				}
			}
			if (event.type == SDL_KEYUP)
			{
				SDL_KeyboardEvent key = event.key;
				if (key.keysym.scancode == 82)
                                {
                                        velo -= 1;
                                        keyST[82] = 0;
                                }
				if (key.keysym.scancode == 81)
                                {
                                        velo += 1;
                                        keyST[81] = 0;
                                }
				if (key.keysym.scancode == 44)
                                {
					wep_sh = 0;
                                }
				if (key.keysym.scancode == 26)
				{
					velo -= 1;
                                        keyST[26] = 0;
				}
				if (key.keysym.scancode == 22)
				{
					velo += 1;
                                        keyST[22] = 0;
				}
				if (key.keysym.scancode == 7)
				{
					h_velo -= 1;
                                        keyST[7] = 0;
				}
				if (key.keysym.scancode == 4)
				{
					h_velo += 1;
                                        keyST[4] = 0;
				}
			}
		}
	}
	for (a = 0; a <= 1; a++)
        {
                for (b = 0; b <= 1; b++)
                {
                        SDL_DestroyTexture(wep_list[a][b]);
                }
        }
	for (a = 0; a < 10; a++)
		SDL_DestroyTexture(rain[a]);
	SDL_DestroyTexture(enemy);
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
