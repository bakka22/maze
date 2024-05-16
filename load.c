#include "demo.h"
#include <SDL2/SDL.h>
#include <SDL.h>
#include <SDL2/SDL_image.h>
#include <stdlib.h>

void load_map(int ***map, int *map_w, int *map_h, FILE *file);

SDL_Texture *load(SDL_Renderer *renderer, int n)
{
	SDL_Surface *image = NULL;
	SDL_Texture *texture = NULL;
	char *images[] = {"images/wall.jpg", "images/base.png", "images/shoot.png", "images/akbase.png", "images/akshoot.png", "images/rain1.png", "images/rain2.png", "images/rain3.png", "images/rain4.png", "images/rain5.png", "images/rain6.png", "images/rain7.png", "images/rain8.png", "images/rain9.png", "images/rain10.png", "images/demon.jpg"};
	image = IMG_Load(images[n]);
	if (!image)
	{
		printf("unable to load image %s\n", IMG_GetError());
		return NULL;
	}
	if (n == 0)
		SDL_SetColorKey(image, SDL_TRUE, SDL_MapRGB(image->format, 0, 255, 255));
	if (n >= 1 && n < 15)
	{
		SDL_SetColorKey(image, SDL_TRUE, SDL_MapRGB(image->format, 0, 0, 0));
	}
	if (n == 15)
	{
		SDL_SetColorKey(image, SDL_TRUE, SDL_MapRGB(image->format, 255, 255, 255));
	}
	texture = SDL_CreateTextureFromSurface(renderer, image);
	if (texture == NULL)
		return NULL;
	SDL_FreeSurface(image);
	return texture;
}
int load_p_e(float p_pos[2], float ***e_pos, char *file_name,int ***map, int *map_w, int *map_h)
{
	char buffer[1024], buf1[200], buf2[200];
	FILE *file = fopen(file_name, "r");
	int v = 0, g, c = 0, e_count = 0;

	fgets(buffer, sizeof(buffer), file);
        for (v = 0; v < sizeof(buffer); v++)
        {
                if (buffer[v] != ',')
                        buf1[v] = buffer[v];
                else
                        break;
        }
        p_pos[0] = atoi(buf1);
        v++;
        for (g = 0; g < sizeof(buffer); g++)
        {
                if (buffer[v] != ';')
                {
                        buf2[g] = buffer[v];
                        v++;
                }
                else
                        break;
        }
        p_pos[1] = atoi(buf2);
	fgets(buffer, sizeof(buffer), file);
	v = 0;
	for (int i = 0;; i++)
	{
		if (buffer[i] == ';')
			e_count += 1;
		else if (buffer[i] == '\0')
			break;
	}
	printf("e_count: %d\n", e_count);
	*e_pos = (float **)malloc(sizeof(float *) * e_count);
	for (int i = 0; i < e_count; i++)
	{
		(*e_pos)[i] = malloc(sizeof(float) * 2);
		c = 0;
		for (; v < sizeof(buffer); v++)
		{
			if (buffer[v] != ',')
			{
				buf1[c] = buffer[v];
				c++;
			}
			else
				break;
		}
		buf1[c] = '\0';
		(*e_pos)[i][0] = atoi(buf1);
		v++;
		for (g = 0; g < sizeof(buffer); g++)
        	{
                	if (buffer[v] != ';')
                	{
                        	buf2[g] = buffer[v];
                        	v++;

                	}
                	else
                        	break;
        	}
		v++;
		buf2[g] = '\0';
		(*e_pos)[i][1] = atoi(buf2);
	}
	load_map(map, map_w, map_h, file);
	fclose(file);
	return e_count;
}
void free_e(float ***e_pos, int e_count)
{
	for (int i = 0; i < e_count; i++)
	{
		free((*e_pos)[i]);
	}
	free(*e_pos);
}
void load_map(int ***map, int *map_w, int *map_h, FILE *file)
{
	char buffer[1024], buf1[200];
	int i, j = 0, v = 0, x = 0;
	fgets(buffer, sizeof(buffer), file);
	for (i = 0; i < sizeof(buffer); i++)
	{
		if (buffer[i] != ',')
			buf1[i] = buffer[i];
		else
			break;
	}
	*map_w = atoi(buf1);
	i++;
	for (; i < sizeof(buffer); i++)
	{
		if (buffer[i] != ';')
		{
			buf1[j] = buffer[i];
			j++;
		}
		else
			break;
	}
	buf1[j] = '\0';
	*map_h = atoi(buf1);
	*map = malloc(sizeof(int *) * (*map_w));
	for (i = 0; i < *map_w; i++)
	{
		(*map)[i] = malloc(sizeof(int) * (*map_h));
	}
	for (i = 0; i < *map_h; i++)
	{
		v = 0;
		x = 0;
		fgets(buffer, sizeof(buffer), file);
		for (j = 0; j < *map_w; j++)
		{
			for (v = 0; v < sizeof(buffer); v++)
			{
				if (buffer[x] != ',' && buffer[x] != ';')
				{
					buf1[v] = buffer[x];
					x++;
				}
				else
					break;
			}
			x++;
			buf1[v] = '\0';
			(*map)[j][i] = atoi(buf1);
		}
	}
}
void free_map(int ***map, int *map_h)
{
	int i;
	for (i = 0; i < *map_h; i++)
		free((*map)[i]);
	free(*map);
}
