#include "demo.h"


void render_wall(SDL_Renderer *renderer, SDL_Texture *wall, int wall_segment_x_coor, float wall_distance, float angel, float v_wall_y_coor, float h_wall_x_coor, int flag)
{
	int pic_x;
	float rec_h, rec_y;
	// initiate the wall segment rectangel
        SDL_Rect column = {wall_segment_x_coor, 0, 1, 0};
	// initiate wall texture cut rectangel
        SDL_Rect pic = {0, 0, 28, 630};

        // remove fish eye effect
        wall_distance = wall_distance * absolute(cos((absolute(direction - (absolute(angel)))) * M_PI / 180));

        // calculate the height of wall segment
        rec_h = size * distance_to_viewport / wall_distance;

        // calculate y coordinate on the screen for wall segment to be rendered
        rec_y = (height / 2) - (rec_h / 2);

	column.y = rec_y;
	column.h = rec_h;

        // determine x coordinate of the wall texture to get according to if the wall is vertical or horizontal
        if(flag == 1)
        {
        	pic_x = (v_wall_y_coor - (((int)v_wall_y_coor / size) * size)) * 28;
        }
        else
        {
        	pic_x = (h_wall_x_coor - (((int)h_wall_x_coor / size) * size)) * 28;
        }
        pic.x = pic_x;

        // render the wall segment on the screen
        SDL_RenderCopy(renderer, wall, &pic, &column);
}

void render_map(SDL_Renderer *renderer, int **map, float *player_position, int player_x_coordinate, int player_y_coordinate)
{
	float x_constant, y_constant, player_x_position, player_y_position, ray_x_position, ray_y_position;
	SDL_Rect viewport = {0, 0, width / 4, height / 4};

	//get percentage between real map size and view port size
        x_constant = (float)viewport.w/(float)map_w;
        y_constant = (float)viewport.h/(float)map_h;

	//render map blocks on viewport
        SDL_RenderSetViewport(renderer, &viewport);
        for (int x = 0; x < map_w; x++)
        {
        	for (int y = 0; y < map_h; y++)
                {
                	SDL_Rect block = {x * x_constant , y * y_constant, x_constant, y_constant};
                	if (map[x][y] == 1)
                        	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
                        else
                        	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);
                 	SDL_RenderFillRect(renderer, &block);

                }
         }

	 // render player on viewport according to its position
         player_x_position = (player_position[0] / size) * x_constant;
         player_y_position = (player_position[1] / size) * y_constant;
         SDL_Rect player = {player_x_position - 2, player_y_position - 2, 5, 5};
         SDL_SetRenderDrawColor(renderer, 255, 0, 0, 0);
         SDL_RenderFillRect(renderer, &player);

	 // render the a ray indicating the player's vision direction
         ray_x_position = ((float)player_x_coordinate / 32.0) * x_constant;
         ray_y_position = ((float)player_y_coordinate / 32.0) * y_constant;
         SDL_RenderDrawLine(renderer, player_x_position, player_y_position, ray_x_position, ray_y_position);
         SDL_RenderSetViewport(renderer, NULL);
}
