#include "demo.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <math.h>
#include <stdio.h>

int init_all(SDL_Instance *instance);

int width = 720;
int height = 480;
int field_of_view = 60;
float direction = 0;
int size = 32;
float distance_to_viewport;
int map_w, map_h;
SDL_Texture *wep_list[2][2] = {{NULL, NULL}, {NULL, NULL}}, *weapon = NULL;
char keyST[256] = {0};
int main(int argc, char **argv)
{
	SDL_Texture *image = NULL, *enemy = NULL;
	float **e_pos = NULL, p_pos[2];
	int e_w, e_h;
	int quit = 0;
	SDL_Texture *rain[10];
	int wep_num = 0;
	int wep_zig = 4, wep_sh = 0;
	float angel_inc = 60.0/width;
	float *e_angel = NULL;
	int x_coor = 0;
	int *enemy_flag = NULL, e_count;
	int *e_x_coor = NULL;
        float *e_dist = NULL;
	int velo = 0, h_velo = 0;
	int view = 1;
	int r = 1, rain_c = 0;
	SDL_Instance instance;
	int a, b, c = 1, v = 0;
	int **map = NULL;

	// calculate distance to viewport
	distance_to_viewport = (width/2) / tan( (field_of_view/2) * M_PI / 180);

	// get enemies count, player position, and parses map
	e_count = load_p_e(p_pos, &e_pos, argv[1], &map, &map_w, &map_h);

	// initialize a list of enemies angels from player
	e_angel = malloc(sizeof(double) * e_count);

	// allocate memory for a list of enemy flags (wether to render an enemy or not, enemies x coordinatea and enemies distance
	enemy_flag = malloc(sizeof(int) * e_count);
	e_x_coor = malloc(sizeof(int) * e_count);
	e_dist = malloc(sizeof(float) * e_count);

	// initialize the enemies angels and flags lists
	for (v = 0; v < e_count; v++)
	{
		e_angel[v] = 0;
		enemy_flag[v] = 1;
	}

	// initialize window and renderer
	if (init_all(&instance) != 0)
		return (1);

	// load weapon textures into a list
	for (a = 0; a <= 1; a++)
	{
		for (b = 0; b <= 1; b++)
		{
			wep_list[a][b] = load(instance.renderer, c);
			c++;
		}
	}

	// load rain texture into a list
	for(a = 0; a < 10; a++)
	{
		rain[a] = load(instance.renderer, c);
		c++;
	}

	// load wall, weapon and  enemy textures
	image = load(instance.renderer, 0);
	weapon = wep_list[0][0];
        if (image == NULL || weapon == NULL)
        {
		printf("image load failed\n");
			return (1);
        }
	enemy = load(instance.renderer, c);
	
	// main game loop
	while (!quit)
	{
		SDL_SetRenderDrawColor(instance.renderer, 4, 24, 92, 0);
		SDL_RenderClear(instance.renderer);
		SDL_Rect floor = {0, height / 2, width, height / 2};
		SDL_Rect cut = {50, 50, 1, 1};
		SDL_SetRenderDrawColor(instance.renderer, 12, 61, 2, 0);
		SDL_RenderFillRect(instance.renderer, &floor);
		float angel = circ(direction + 30, 0, 359);
		SDL_Event event;
		int v_flag = 0, h_flag = 0;
		int x_cor_p = 0, y_cor_p = 0;

		// reset the enemies flag list to 1
                for (v = 0; v < e_count; v++)
                        enemy_flag[v] = 1;

		// get enemies distances
		get_enemies_distances(e_count, p_pos, e_pos, &e_angel, &e_dist);

		// each iteration in this loop represents a ray being casted and its intersections and renders the wall it hits.
		for (x_coor = 0; x_coor < width; x_coor++)
		{
			float a_x, a_y, a_x_h, a_y_h, x_a, y_a, wall_distance = 10000, wall_distance_h = 10000, wall_distance_min = 10000, a_y_a, a_x_a;
			int b = 0, x_cor, y_cor, x_cor_h, y_cor_h, x_cor_v, y_cor_v, flag = 0;
			v_flag = 0;
			h_flag = 0;
			
			// calculate the first y,x coordinate for vertical and horizontal intersection.
			a_y_h = calc_y_h(p_pos[1], angel, size, 1);
			a_x_h = calc_x_h(p_pos[0], angel, a_y_h, p_pos[1], 1, 0);
			a_x = calc_x_v(p_pos[0], angel, size, 1);
			a_y = calc_y_v(p_pos[1], angel, a_x, p_pos[0], 1, 0);
			
			// calculate the x increment for horizontal intersections
			x_a = (float)size / absolute(tan(angel * M_PI / 180));
			
			// calculate the y increment for vertical intersections according to angel.
			if (angel == 90 || angel == 270)
				y_a = size;
			else
				y_a = (float)size * absolute(tan(angel * M_PI / 180));
			
			//calculate the first vertical and horizontal intersection with a grid cell.
			wall_distance = absolute(a_x - p_pos[0]) / absolute(cos(angel * M_PI / 180));
			wall_distance_h = absolute(a_y_h - p_pos[1]) / absolute(sin(angel * M_PI / 180));
			
			// calculates the subsequant vertical and horizontal intersections with grid cells and exit if a wall is found or out of the grid bounds.
			while (b < map_w && b < map_h)
			{
				int e_cal = 1;
				x_cor_h = a_x_h / size;
				y_cor_h = a_y_h / size;
				x_cor_v = a_x / size;
				y_cor_v = a_y / size;
				
				
				// check if vertical wall is found.
				if (wall_distance > 0 && wall_distance < wall_distance_min && x_cor_v < map_w && x_cor_v >= 0 && y_cor_v >= 0 && y_cor_v < map_h && map[x_cor_v][y_cor_v] == 1)
				{
					v_flag = 1;
					x_cor = x_cor_v;
					y_cor = y_cor_v;
					wall_distance_min = wall_distance;
					flag = 1;
					a_y_a = a_y;
					if (x_coor == width/2 - 1)
					{
						y_cor_p = a_y;
                                        	x_cor_p = a_x;
					}
					if (v_flag && h_flag)
						break;
					
				}
				
				// check if a horizontal wall is found
				if (wall_distance_h > 0 && wall_distance_h < wall_distance_min && x_cor_h < map_w && x_cor_h >= 0 && y_cor_h >= 0 && y_cor_h < map_h && map[x_cor_h][y_cor_h] == 1)
				{
					h_flag = 1;
					x_cor = x_cor_h;
                                       	y_cor = y_cor_h;
					wall_distance_min = wall_distance_h;
					flag = 2;
					a_x_a = a_x_h;
					if (x_coor == width/2 - 1)
					{
						y_cor_p = a_y_h;
                                        	x_cor_p = a_x_h;
					}
					if (v_flag && h_flag)
						break;
				}
				
				// calculate the next intersection with a gird cell.
				a_y_h = calc_y_h(a_y_h, angel, size, 2);
				a_x_h = calc_x_h(a_x_h, angel, a_y_h, 0, 2, x_a);
				a_x = calc_x_v(a_x, angel, size, 2);
                                a_y = calc_y_v(a_y, angel, a_x, 0, 2, y_a);
				
				// calculate the distance between the player and the intersection.
				wall_distance = absolute(a_x - p_pos[0]) / absolute(cos(angel * M_PI / 180));
				wall_distance_h = absolute(a_y_h - p_pos[1]) / absolute(sin(angel * M_PI / 180));
				b++;
			}

			
			// render a wall on screen if a wall is found.
			if (x_cor < map_w && x_cor >= 0 && y_cor >= 0 && y_cor < map_h && map[x_cor][y_cor] == 1)
			{
				render_wall(instance.renderer, image, x_coor, wall_distance_min, angel, a_y_a, a_x_a, flag);
                        }
			
			// check if an enemy is in the current field of view.
			for (int y = 0; y < e_count; y++)
			{
				if (e_angel[y] >= angel && e_angel[y] < angel + angel_inc && e_dist[y] < wall_distance_min)
				{
					e_x_coor[y] = x_coor;
					enemy_flag[y] = 0;
				}
			}
			
			// decrement the angel by angel increment value.
			angel = circ(angel - angel_inc, 0, 359);
		}
		
		// render every enemy if it was detected in the field of view.
		for (int y = 0; y < e_count; y++)
		{
			if(!enemy_flag[y])
			{
				float e_rec_h = size * distance_to_viewport / e_dist[y];
				float e_y = (height / 2) - (e_rec_h / 2);
				float e_rec_w = size * distance_to_viewport / e_dist[y];
				SDL_Rect e_rec = {e_x_coor[y], e_y, e_rec_w, e_rec_h};
				SDL_RenderCopy(instance.renderer, enemy, NULL, &e_rec);
			}
		}
		
		// render rain texture if rain flag (r) is on.
		if (r)
		{
			SDL_RenderCopy(instance.renderer, rain[rain_c/3], NULL, NULL);
			rain_c = circ(rain_c + 1, 0, 29);
		}
		
		// render mini map if map flag (view) is on.
		if (view)
			render_map(instance.renderer, map, p_pos, x_cor_p, y_cor_p);

		
		// add shooting effect to the weapon by switching between two images if shoot button is held or reset variable if not.
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
		
		// render weapon.
		SDL_Rect weap = {width/2 - 80, height/2, width/2, height / 2};
		SDL_RenderCopy(instance.renderer, weapon, NULL, &weap);
		SDL_RenderPresent(instance.renderer);
		
		// change player's position according to its velocity
		apply_velo(map, p_pos, velo, h_velo);
		
		// check if a button is pressed and apply changes.
		poll_event(&quit, &view, &wep_sh, &wep_num, &velo, &h_velo, &r);
	}
	
	// destroy textures, window ,renderer and free allocated memory  then quit
	for (a = 0; a <= 1; a++)
        {
                for (b = 0; b <= 1; b++)
                {
                        SDL_DestroyTexture(wep_list[a][b]);
                }
        }
	for (a = 0; a < 10; a++)
		SDL_DestroyTexture(rain[a]);
	free(e_angel);
	free(enemy_flag);
	free(e_x_coor);
	free(e_dist);
	free_e(&e_pos, e_count);
	free_map(&map, &map_h);
	SDL_DestroyTexture(enemy);
	SDL_DestroyTexture(image);
	SDL_DestroyRenderer(instance.renderer);
        SDL_DestroyWindow(instance.window);
        SDL_Quit();
	return (0);
}

int init_all(SDL_Instance *instance)
{
	// init sdl vidoe
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		printf("initialization failed\n");
		return (1);
	}
	
	// initialize window
	instance->window = SDL_CreateWindow("demo", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, 0);
	if (instance->window == NULL)
	{
		printf("window creation failed\n");
		return (1);
	}
	
	// initialize renderer.
	instance->renderer = SDL_CreateRenderer(instance->window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (instance->renderer == NULL)
	{
		printf("renderer creation failed\n");
		return (1);
	}
	return (0);
}
