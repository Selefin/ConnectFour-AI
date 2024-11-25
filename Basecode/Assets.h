#pragma once

#include "Settings.h"

typedef struct Assets_s
{
    SDL_Texture *grille;
	SDL_Texture* pionrouge;
	SDL_Texture* pionjaune;
	SDL_Texture* pionrougetrans;
	SDL_Texture* pionjaunetrans;
	SDL_Texture* modemenu;
	SDL_Texture* mode1v1;
	SDL_Texture* modeJvIA;
	SDL_Texture* modeIAvIA;
	SDL_Texture* joueurwin;
	SDL_Texture* iawin;
	SDL_Texture* lvl0;
	SDL_Texture* lvl1;
	SDL_Texture* lvl2;
	SDL_Texture* lvl3;
	SDL_Texture* lvl4;
	SDL_Texture* lvl5;
	SDL_Texture* lvl6;
	SDL_Texture* lvl7;
	SDL_Texture* lvl8;
	SDL_Texture* j1win;
	SDL_Texture* j2win;
	SDL_Texture* draw;

} Assets;

Assets *Assets_New(SDL_Renderer *renderer);
void Assets_Delete(Assets *self);
