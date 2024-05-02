#include "demo.h"
#include <SDL2/SDL.h>
#include <SDL.h>
#include <SDL2/SDL_image.h>

SDL_Texture *load(SDL_Renderer *renderer, int n)
{
	SDL_Surface *image = NULL;
	SDL_Texture *texture = NULL;
	char *images[] = {"foo.png", "walls/tile06.jpg", "walls/tile25.jpg", "background.png", "soccer.jpg"};
	image = IMG_Load(images[n]);
	if (!image)
	{
		printf("unable to load image %s\n", IMG_GetError());
		return NULL;
	}
	if (n == 0)
		SDL_SetColorKey(image, SDL_TRUE, SDL_MapRGB(image->format, 0, 255, 255));
	if (n == 2)
		SDL_SetColorKey(image, SDL_TRUE, SDL_MapRGB(image->format, 20, 244, 10));
	texture = SDL_CreateTextureFromSurface(renderer, image);
	if (texture == NULL)
		return NULL;
	SDL_FreeSurface(image);
	return texture;
}
