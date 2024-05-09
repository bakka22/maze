#include "demo.h"
#include <SDL2/SDL.h>
#include <SDL.h>
#include <SDL2/SDL_image.h>

SDL_Texture *load(SDL_Renderer *renderer, int n)
{
	SDL_Surface *image = NULL;
	SDL_Texture *texture = NULL;
	char *images[] = {"wall.jpg", "base.png", "shoot.png", "akbase.png", "akshoot.png", "rain1.png", "rain2.png", "rain3.png", "rain4.png", "rain5.png", "rain6.png", "rain7.png", "rain8.png", "rain9.png", "rain10.png", "demon.jpg"};
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
		//SDL_SetColorKey(image, SDL_TRUE, SDL_MapRGB(image->format, 219, 219, 219));
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
