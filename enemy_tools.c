#include "demo.h"

/**
 * get_enemies_distances - get every enemy's distance and angel from the player
 * @enemies_count: the number of enemies
 * @player_position: the position of the player in the grid
 * @enemies_positions: the positions of the enemies in the grid
 * @(*enemies_angels): the angel that the player forms with an enemy
 * @enemies_distances: the distance from the enemy to the player
 *
 * Description: this function uses the triangel formed by the player and the enemy to calculate the angel between
 * them and the distance from the player to the enemy
 *
 * Return: nothing
*/
void get_enemies_distances(int enemies_count, float *player_position, float **enemy_position, float **enemies_angels, float **enemies_distances)
{
	int y;
	// x side of the triangel of the enemy and player
	float x_e_p;
	// y side of the triangel of the enemy and player
	float y_e_p;
	// tan of the angel between the enemy and player
	float enemy_tan;
	for (int y = 0; y < enemies_count; y++)
        {
       		x_e_p = absolute(player_position[0] - enemy_position[y][0]);
                y_e_p = absolute(player_position[1] - enemy_position[y][1]);
                enemy_tan = y_e_p / x_e_p;
                (*enemies_angels)[y] = atan(enemy_tan) * 180 / M_PI;
                if (player_position[0] > enemy_position[y][0] && player_position[1] > enemy_position[y][1])
                	(*enemies_angels)[y] = circ(180 - (*enemies_angels)[y], 0, 359);
               	else if (player_position[0] < enemy_position[y][0] && player_position[1] < enemy_position[y][1])
                	(*enemies_angels)[y] = circ(360 - (*enemies_angels)[y], 0, 359);
                else if (player_position[0] > enemy_position[y][0] && player_position[1] < enemy_position[y][1])
                	(*enemies_angels)[y] = circ((*enemies_angels)[y] + 180, 0, 359);
                 (*enemies_distances)[y] = sqrt((x_e_p * x_e_p) + (y_e_p * y_e_p));
                }
}
