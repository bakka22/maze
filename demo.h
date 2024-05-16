#ifndef _DEMO_H_
#define _DEMO_H_

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>
#include <math.h>

typedef struct SDl_Instance
{
	SDL_Window *window;
	SDL_Renderer *renderer;
} SDL_Instance;
//-----------global variables--------:
extern int width;
extern int height;
extern int field_of_view;
extern float distance_to_viewport;
extern float direction;
extern int size;
extern int map_w, map_h;
extern SDL_Texture *weapon;
extern SDL_Texture *wep_list[2][2];
extern char keyST[256];
//-----------functions---------------:
int init_instance(SDL_Instance *);
void apply_velo(int **map, float *player_position, int velo, int h_velo);
void poll_event(int *quit, int *view, int *wep_sh, int *wep_num, int *velo, int *h_velo, int *r);
void render_wall(SDL_Renderer *renderer, SDL_Texture *wall, int wall_segment_x_coor, float wall_distance, float angel, float v_wall_y_coor, float h_wall_x_coor, int flag);
void render_map(SDL_Renderer *renderer, int **map, float *player_position, int player_x_coordinate, int player_y_coordinate);
int poll_events();
int load_p_e(float p_pos[2], float ***e_pos, char *file_name, int ***map, int *map_w, int *map_h);
void free_e(float ***e_pos, int e_count);
float calc_x_v(float p_pos, float angel, int size, int flag);
float calc_y_v(float p_pos, float angel, int ax, int p_posx, int flag, float y_a);
float calc_y_h(float p_pos, float angel, int size, int flag);
float calc_x_h(float p_pos, float angel, int ay, int p_posy, int flag, float x_a);
void free_map(int ***map, int *map_h);
float circ(float n, float min, float max);
void get_enemies_distances(int enemies_count, float *player_position, float **enemy_position, float **enemies_angels, float **enemies_distances);
float absolute(float n);
SDL_Texture *load(SDL_Renderer *renderer, int n);
#endif
