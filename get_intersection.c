#include "demo.h"

/**
 * calc_x_v - calculate x coordinates for vertical intersections
 * @p_pos: current player's x coordinate
 * @angel: ray's angel
 * @size: size of grid cell
 * @flag: a falg to indicate if it is the first intersection or a subsequant one
 *
 * Description: starting from player's position this function calculates according to the flag
 * the x coordinate of the first or a subsequant vertical intersection - of a ray casted on a 
 * specific angel - with a grid's cell.
 * 
 * Return: x coordinate of the intersection.
*/
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

/**
 * calc_y_v - calculate y coordinate for vertical intersections
 * @p_pos: current player's y coordinate
 * @angel: ray's angel
 * @ax: x coordinate of vertical intersection
 * @p_posx: current player's x coordinate
 * @flag: a falg to indicate if it is the first intersection or a subsequant one
 * y_a: the calculated segmental increment for y coordinates
 *
 * Description: starting from player's position this function calculates according to the flag
 * the y coordinate of the first or a subsequant vertical intersection - of a ray casted on a
 * specific angel - with a grid's cell.
 *
 * Return: y coordinate of the intersection.
*/
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

/**
 * calc_y_h - calculate y coordinates for horizontal intersections
 * @p_pos: current player's y coordinate
 * @angel: ray's angel
 * @size: size of grid cell
 * @flag: a falg to indicate if it is the first intersection or a subsequant one
 *
 * Description: starting from player's position this function calculates according to the flag
 * the y coordinate of the first or a subsequant horizontal intersection - of a ray casted on a
 * specific angel - with a grid's cell.
 *
 * Return: y coordinate of the intersection.
*/
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

/**
 * calc_x_h - calculate x coordinate for horizontal intersections
 * @p_pos: current player's x coordinate
 * @angel: ray's angel
 * @ay: y coordinate of horizontal intersection
 * @p_posy: current player's y coordinate
 * @flag: a falg to indicate if it is the first intersection or a subsequant one
 * x_a: the calculated segmental increment for x coordinates
 *
 * Description: starting from player's position this function calculates according to the flag
 * the x coordinate of the first or a subsequant vertical intersection - of a ray casted on a
 * specific angel - with a grid's cell.
 *
 * Return: x coordinate of the intersection.
*/
float calc_x_h(float p_pos, float angel, int ay, int p_posy, int flag, float x_a)
{
                if (flag == 1)
                {
                        if (angel > 90 && angel < 270)
                                return (p_pos - (absolute(ay - p_posy)) / absolute(tan(angel * M_PI / 180)));
                        else
                                return (p_pos + (absolute(ay - p_posy)) / absolute(tan(angel * M_PI / 180)));
                }
                else
                {
                        if (angel > 90 && angel < 270)
                                return (p_pos - x_a);
                        else
                                return (p_pos + x_a);
                }

}
