#include "demo.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

void poll_event(int *quit, int *view, int *wep_sh, int *wep_num, int *velo, int *h_velo, int *r)
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
                {
                        if (event.type == SDL_QUIT)
                                *quit = 1;
                        if (event.type == SDL_KEYDOWN)
                        {
                                SDL_KeyboardEvent key;
                                key = event.key;
				switch (key.keysym.scancode)
				{
                                	case 16:
                                        	if (*view)
                                                	*view = 0;
                                        	else
                                                	*view = 1;
						break;
                                	case 79:
                                        	direction = circ(direction - 22.5, 0, 359);
						break;
                                	case 80:
                                        	direction = circ(direction + 22.5, 0, 359);
						break;
                                	case 82:
                                        	if (!keyST[82])
                                                	*velo += 1;
                                        	keyST[82] = 1;
						break;
                                	case 81:
                                        	if (!keyST[81])
                                                	*velo -= 1;
                                        	keyST[81] = 1;
						break;
                                	case 44:
                                        	*wep_sh = 1;
						break;
                                	case 20:
                                        	*wep_num = circ(*wep_num + 1, 0, 1);
                                        	weapon = wep_list[*wep_num][0];
                              			break;
                               		case 26:
                                        	if (!keyST[26])
                                                	*velo += 1;
                                        	keyST[26] = 1;
                                		break;
                                	case 22:
                                        	if (!keyST[22])
                                                	*velo -= 1;
                                        	keyST[22] = 1;
                                		break;
                                	case 7:
                                        	if (!keyST[7])
                                                	*h_velo += 1;
                                        	keyST[7] = 1;
                                		break;
                                	case 4:
                                        	if (!keyST[4])
                                                	*h_velo -= 1;
                                        	keyST[4] = 1;
                                		break;
                                	case 21:
                                        	if (*r == 0)
                                                	*r = 1;
                                        	else
                                                	*r = 0;
                                		break;
				}
			}
                        if (event.type == SDL_KEYUP)
                        {
                                SDL_KeyboardEvent key = event.key;
				switch (key.keysym.scancode)
				{
                                	case 82:
                                        	*velo -= 1;
                                        	keyST[82] = 0;
                              			break;
                                	case 81:
                                        	*velo += 1;
                                        	keyST[81] = 0;
                                		break;
                                	case 44:
                                        	*wep_sh = 0;
                                		break;
                                	case 26:
                                        	*velo -= 1;
                                        	keyST[26] = 0;
                                		break;
                                	case 22:
                                        	*velo += 1;
                                        	keyST[22] = 0;
                                		break;
                                	case 7:
                                        	*h_velo -= 1;
                                        	keyST[7] = 0;
                                		break;
                                	case 4:
                                        	*h_velo += 1;
                                        	keyST[4] = 0;
						break;
                                }
                        }
                }
}
void apply_velo(int **map, float *player_position, int velo, int h_velo)
{
	int map_x, map_y;

	// change player's x coordinate by velocity * cos(direction)
	player_position[0] += velo * cos(direction * M_PI / 180);
                map_x = (int)player_position[0] / size;
                map_y = (int)player_position[1] / size;
		
		// reverse if there is a wall
                if (map[map_x][map_y] == 1)
                {
                        player_position[0] -= velo * cos(direction * M_PI / 180);
                }

		// same thing done again for y position and horizontal velocity (when moving left or right)
                player_position[1] -= velo * sin(direction * M_PI / 180);
                map_x = (int)player_position[0] / size;
                map_y = (int)player_position[1] / size;
                if (map[map_x][map_y] == 1)
                {
                        player_position[1] += velo * sin(direction * M_PI / 180);
                }
                player_position[0] += h_velo * cos(circ(direction-90, 0, 359) * M_PI / 180);
                map_x = (int)player_position[0] / size;
                map_y = (int)player_position[1] / size;
                if (map[map_x][map_y] == 1)
                {
                        player_position[0] -= h_velo * cos(circ(direction-90, 0, 359) * M_PI / 180);
                }

                player_position[1] -= h_velo * sin(circ(direction-90, 0, 359) * M_PI / 180);
                map_x = (int)player_position[0] / size;
                map_y = (int)player_position[1] / size;
                if (map[map_x][map_y] == 1)
                {
                        player_position[1] += h_velo * sin(circ(direction-90, 0, 359) * M_PI / 180);
                }
}
