#ifndef _DEMO_H_
#define _DEMO_H_

#include <SDL2/SDL.h>
#include <stdbool.h>

typedef struct SDl_Instance
{
	SDL_Window *window;
	SDL_Renderer *renderer;
} SDL_Instance;

int init_instance(SDL_Instance *);
int poll_events();
SDL_Texture *load(SDL_Renderer *renderer, int n);
#endif
