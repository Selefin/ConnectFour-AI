#include "Assets.h"

typedef struct TextureSpec_s
{
    SDL_Texture **ptr;
    char *path;
} TextureSpec;

Assets *Assets_New(SDL_Renderer *renderer)
{
    Assets *self = (Assets *)calloc(1, sizeof(Assets));
    AssertNew(self);

    // -------------------------------------------------------------------------
    // Chargement des textures

    TextureSpec texSpecs[] = {
        { &self->grille, "../Assets/grille.png" },
		{ &self->pionrouge, "../Assets/pionrouge.png" },
		{ &self->pionjaune, "../Assets/pionjaune.png" },
		{ &self->pionrougetrans, "../Assets/pionrougetrans.png" },
		{ &self->pionjaunetrans, "../Assets/pionjaunetrans.png" },
		{ &self->modemenu, "../Assets/modemenu.png" },
		{ &self->mode1v1, "../Assets/mode1v1.png" },
		{ &self->modeJvIA, "../Assets/modeJvIA.png" },
		{ &self->modeIAvIA, "../Assets/modeIAvIA.png" },
		{ &self->joueurwin, "../Assets/Joueurwin.png" },
		{ &self->iawin, "../Assets/IAwin.png" },
		{ &self->lvl0, "../Assets/niveauIA0.png" },
		{ &self->lvl1, "../Assets/niveauIA1.png" },
		{ &self->lvl2, "../Assets/niveauIA2.png" },
		{ &self->lvl3, "../Assets/niveauIA3.png" },
		{ &self->lvl4, "../Assets/niveauIA4.png" },
		{ &self->lvl5, "../Assets/niveauIA5.png" },
		{ &self->lvl6, "../Assets/niveauIA6.png" },
		{ &self->lvl7, "../Assets/niveauIA7.png" },
		{ &self->lvl8, "../Assets/niveauIA8.png" },
		{ &self->j1win, "../Assets/j1win.png" },
		{ &self->j2win, "../Assets/j2win.png" },
		{ &self->draw, "../Assets/draw.png" },
    };
    int texSpecCount = sizeof(texSpecs) / sizeof(TextureSpec);

    for (int i = 0; i < texSpecCount; i++)
    {
        SDL_Texture **texPtr = texSpecs[i].ptr;
        char *path = texSpecs[i].path;

        *texPtr = IMG_LoadTexture(renderer, path);
        if (*texPtr == NULL)
        {
            printf("ERROR - Loading texture %s\n", path);
            printf("      - %s\n", SDL_GetError());
            assert(false);
            abort();
        }
    }

    return self;
}

void Assets_Delete(Assets *self)
{
    if (!self) return;

    // -------------------------------------------------------------------------
    // Libère les textures

    SDL_Texture** texPointers[] = {
        &self->grille,
        &self->pionrouge,
        &self->pionjaune,
        &self->pionrougetrans,
        &self->pionjaunetrans,
        &self->modemenu,
        &self->mode1v1,
        &self->modeJvIA,
        &self->modeIAvIA,
        &self->joueurwin,
        &self->iawin,
		& self->lvl0,
		& self->lvl1,
		& self->lvl2,
        & self->lvl3,
        & self->lvl4,
        & self->lvl5,
        & self->lvl6,
        & self->lvl7,
        & self->lvl8,
        & self->j1win,
        & self->j2win,
        & self->draw,
	};
    int count = sizeof(texPointers) / sizeof(SDL_Texture **);

    for (int i = 0; i < count; i++)
    {
        if (*texPointers[i])
            SDL_DestroyTexture(*(texPointers[i]));
    }

    free(self);
}