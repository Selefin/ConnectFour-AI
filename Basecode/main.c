#include "Settings.h"
#include "Common.h"
#include "Input.h"
#include "Assets.h"
#include "Game.h"
#include "AI.h"

// IMPORTANT :
// Les librairies SDL_TTF et SDL_Mixer sont configurées dans le projet mais ne
// sont pas initialisées.
// Vous devez pour cela modifier les fonctions App_Init() et App_Quit() dans
// le fichier Common.c

int main(int argc, char* argv[])
{
    // Initialisation de la SDL
    App_Init(SDL_INIT_VIDEO, IMG_INIT_PNG);

    // Crée la fenêtre et le moteur de rendu
    int sdlFlags = 0;
#ifdef FULLSCREEN
    sdlFlags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
#endif

    SDL_Window* window = SDL_CreateWindow(
        u8"Basecode SDL", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        WINDOW_WIDTH, WINDOW_HEIGHT, sdlFlags
    );

    if (!window)
    {
        printf("ERROR - Create window %s\n", SDL_GetError());
        assert(false); abort();
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(
        window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );

    SDL_RenderSetLogicalSize(renderer, LOGICAL_WIDTH, LOGICAL_HEIGHT);
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");

    if (!renderer)
    {
        printf("ERROR - Create renderer %s\n", SDL_GetError());
        assert(false); abort();
    }

    // Crée le temps global du jeu
    g_time = Timer_New();
    AssertNew(g_time);

    // Crée le gestionnaire des entrées utilisateur
    Input* input = Input_New();

    // Chargement des assets
    Assets* assets = Assets_New(renderer);

    //--------------------------------------------------------------------------
    // Boucle de rendu

    // Position et vitesse du logo (exemple)
    float x = 0.0f;
    float y = 0.0f;
    float w = LOGICAL_WIDTH;
    float h = LOGICAL_HEIGHT;

    float xpion = LOGICAL_WIDTH / 7 * 3 - 2;
    float ypion = 0.0f;
    float wpion = LOGICAL_WIDTH;
    float hpion = LOGICAL_HEIGHT;



    Game* game = Game_New();
    int ia = 2;
    int column = 3;
	int depth = 0;
    while (!input->spacePressed)
    {
        // Met à jour le temps
        Timer_Update(g_time);

        // Met à jour les entrées
        Input_Update(input);

        // Efface le rendu précédent
        SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
        SDL_RenderClear(renderer);

        SDL_FRect menu;

        menu.x = x;
        menu.y = y;
        menu.w = w;
        menu.h = h;

        if (input->quitPressed)
        {
            ia = 0;
            break;
        }

        if (ia > 1 && input->leftPressed)
        {
            ia--;
        }
        else if (ia < 3 && input->rightPressed)
        {
            ia++;
        }
        
		if (depth > 0 && input->downPressed)
		{
			depth--;
		}
		else if (depth < 8 && input->upPressed)
		{
			depth++;
		}

        if(ia == 1)
            SDL_RenderCopyF(renderer, assets->mode1v1, NULL, &menu);
        else if(ia == 2)
            SDL_RenderCopyF(renderer, assets->modeJvIA, NULL, &menu);
        else if(ia == 3)
            SDL_RenderCopyF(renderer, assets->modeIAvIA, NULL, &menu);
        
		if (depth == 0)
			SDL_RenderCopyF(renderer, assets->lvl0, NULL, &menu);
		else if (depth == 1)
			SDL_RenderCopyF(renderer, assets->lvl1, NULL, &menu);
		else if (depth == 2)
			SDL_RenderCopyF(renderer, assets->lvl2, NULL, &menu);
		else if (depth == 3)
			SDL_RenderCopyF(renderer, assets->lvl3, NULL, &menu);
		else if (depth == 4)
			SDL_RenderCopyF(renderer, assets->lvl4, NULL, &menu);
		else if (depth == 5)
			SDL_RenderCopyF(renderer, assets->lvl5, NULL, &menu);
		else if (depth == 6)
			SDL_RenderCopyF(renderer, assets->lvl6, NULL, &menu);
		else if (depth == 7)
			SDL_RenderCopyF(renderer, assets->lvl7, NULL, &menu);
		else if (depth == 8)
			SDL_RenderCopyF(renderer, assets->lvl8, NULL, &menu);
        
        // Affiche le nouveau rendu
        SDL_RenderPresent(renderer);
    }
    if (ia == 2)
    {
        printf("Mode IA vs Joueur\n\n");
        // Boucle de jeu
        while (Game_GetState(game) == GAME_IN_PROGRESS)
        {
            // Met à jour le temps
            Timer_Update(g_time);

            // Met à jour les entrées
            Input_Update(input);

            if (input->quitPressed)
                break;

            // Efface le rendu précédent
            SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
            SDL_RenderClear(renderer);
            SDL_FRect dstRect;
            SDL_FRect pion[7][6];


            if (Game_GetPlayerID(game) == PLAYER_2)
            {
                int move = AI_ComputeMove(game, PLAYER_2, depth);
                int c = 0;
                while (game->grid[c][move] != 0 && c <= 5)
                {
                    c++;
                }
                pion[c][move].x = move * (LOGICAL_WIDTH / 7 - 1);
                pion[c][move].y = LOGICAL_HEIGHT - (c + 1) * (LOGICAL_HEIGHT / 6);
                pion[c][move].w = wpion;
                pion[c][move].h = hpion;
                Game_PlayTurn(game, move);
                printf("----------------------------------------\n");
                printf("\n L'ordinateur a joue en colonne %d\n\n", move);
            }
            else
            {
                if (input->leftPressed && xpion > 10)
                {
                    xpion -= LOGICAL_WIDTH / 7 - 1;
                    column--;
                }
                else if (input->rightPressed && xpion < LOGICAL_WIDTH - 2 * LOGICAL_WIDTH / 7)
                {
                    xpion += LOGICAL_WIDTH / 7 - 1;
                    column++;
                }
                dstRect.x = xpion;
                dstRect.y = ypion;
                dstRect.w = wpion;
                dstRect.h = hpion;
                if (game->playerID == PLAYER_1)
                    SDL_RenderCopyF(renderer, assets->pionrougetrans, NULL, &dstRect);
                else
                    SDL_RenderCopyF(renderer, assets->pionjaunetrans, NULL, &dstRect);

                // Action du joueur
                if (input->spacePressed && Game_CanPlayAt(game, column))
                {
                    int c = 0;
                    while (game->grid[c][column] != 0 && c <= 5)
                    {
                        c++;
                    }
                    pion[c][column].x = column * (LOGICAL_WIDTH / 7 - 1);
                    pion[c][column].y = LOGICAL_HEIGHT - (c + 1) * (LOGICAL_HEIGHT / 6);
                    pion[c][column].w = wpion;
                    pion[c][column].h = hpion;
                    Game_PlayTurn(game, column);
                    Game_Print(game);
                    printf("\n");
                }
            }
            for (int i = 0; i < GRID_W; i++)
            {
                for (int j = 0; j < GRID_H; j++)
                {
                    if (game->grid[i][j] == CELL_P1)
                        SDL_RenderCopyF(renderer, assets->pionrouge, NULL, &pion[i][j]);
                    else if (game->grid[i][j] == CELL_P2)
                        SDL_RenderCopyF(renderer, assets->pionjaune, NULL, &pion[i][j]);
                }
            }

            //renvoi de la grille
            dstRect.x = x;
            dstRect.y = y;
            dstRect.w = w;
            dstRect.h = h;
            SDL_RenderCopyF(renderer, assets->grille, NULL, &dstRect);
            // Affiche le nouveau rendu
            SDL_RenderPresent(renderer);
        }
    }
    else if (ia == 1)
    {
        printf("Mode 1 vs 1\n\n");
        // Boucle de jeu
        while (Game_GetState(game) == GAME_IN_PROGRESS)
        {

            // Met à jour le temps
            Timer_Update(g_time);

            // Met à jour les entrées
            Input_Update(input);

            if (input->quitPressed)
                break;

            // Efface le rendu précédent
            SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
            SDL_RenderClear(renderer);
            SDL_FRect dstRect;
            SDL_FRect pion[7][6];

            // Choix de la colonne
            //déplacement du pion transparent
            if (input->leftPressed && xpion > 10)
            {
                xpion -= LOGICAL_WIDTH / 7 - 1;
                column--;
            }
            else if (input->rightPressed && xpion < LOGICAL_WIDTH - 2 * LOGICAL_WIDTH / 7)
            {
                xpion += LOGICAL_WIDTH / 7 - 1;
                column++;
            }
            dstRect.x = xpion;
            dstRect.y = ypion;
            dstRect.w = wpion;
            dstRect.h = hpion;
            if (game->playerID == PLAYER_1)
                SDL_RenderCopyF(renderer, assets->pionrougetrans, NULL, &dstRect);
            else
                SDL_RenderCopyF(renderer, assets->pionjaunetrans, NULL, &dstRect);

            // Action du joueur
            if (input->spacePressed && Game_CanPlayAt(game, column))
            {
                int c = 0;
                while (game->grid[c][column] != 0 && c <= 5)
                {
                    c++;
                }
				pion[c][column].x = LOGICAL_WIDTH - LOGICAL_WIDTH / 7;
                pion[c][column].x = column * (LOGICAL_WIDTH / 7 - 1);
                pion[c][column].y = LOGICAL_HEIGHT - (c+1) * (LOGICAL_HEIGHT / 6);
                pion[c][column].w = wpion;
                pion[c][column].h = hpion;
                Game_PlayTurn(game, column);
                Game_Print(game);
                printf("\n");
            }

            for (int i = 0; i < GRID_H; i++)
            {
                for (int j = 0; j < GRID_W; j++)
                {
                    if (game->grid[i][j] == CELL_P1)
                        SDL_RenderCopyF(renderer, assets->pionrouge, NULL, &pion[i][j]);
                    if (game->grid[i][j] == CELL_P2)
						SDL_RenderCopyF(renderer, assets->pionjaune, NULL, &pion[i][j]);
                }
            }
            //renvoie de la grille
            dstRect.x = x;
            dstRect.y = y;
            dstRect.w = w;
            dstRect.h = h;
            SDL_RenderCopyF(renderer, assets->grille, NULL, &dstRect);
            // Affiche le nouveau rendu
            SDL_RenderPresent(renderer);
        }
    }
    else if (ia == 3)
    {
        printf("Mode IA vs IA\n\n");
        while (Game_GetState(game) == GAME_IN_PROGRESS)
        {
            // Met à jour le temps
            Timer_Update(g_time);

            // Met à jour les entrées
            Input_Update(input);

            if (input->quitPressed)
                break;

            // Efface le rendu précédent
            SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
            SDL_RenderClear(renderer);
            SDL_FRect dstRect;
            SDL_FRect pion[7][6];


            if (Game_GetPlayerID(game) == PLAYER_1)
            {
                int move = AI_ComputeMove(game, PLAYER_1, depth);
                int c = 0;
                while (game->grid[c][move] != 0 && c <= 5)
                {
                    c++;
                }
                pion[c][move].x = move * (LOGICAL_WIDTH / 7 - 1);
                pion[c][move].y = LOGICAL_HEIGHT - (c + 1) * (LOGICAL_HEIGHT / 6);
                pion[c][move].w = wpion;
                pion[c][move].h = hpion;
                Game_PlayTurn(game, move);
                printf("----------------------------------------\n");
                printf("\n L'ordinateur 1 a joue en colonne %d\n\n", move);
            }
            else if (Game_GetPlayerID(game) == PLAYER_2)
            {
                int move = AI_ComputeMove(game, PLAYER_2, depth);
                Game_PlayTurn(game, move);
                printf("----------------------------------------\n");
                printf("\n L'ordinateur 2 a joue en colonne %d\n\n", move);
            }

            for (int i = 0; i < GRID_W; i++)
            {
                for (int j = 0; j < GRID_H; j++)
                {
                    pion[i][j].x = j * (LOGICAL_WIDTH / 7 - 1);
                    pion[i][j].y = LOGICAL_HEIGHT - (i + 1) * (LOGICAL_HEIGHT / 6);
                    pion[i][j].w = wpion;
                    pion[i][j].h = hpion;
                    
                    if (game->grid[i][j] == CELL_P1)
                        SDL_RenderCopyF(renderer, assets->pionrouge, NULL, &pion[i][j]);
                    else if (game->grid[i][j] == CELL_P2)
                        SDL_RenderCopyF(renderer, assets->pionjaune, NULL, &pion[i][j]);
                }
            }
            //renvoie de la grille
            dstRect.x = x;
            dstRect.y = y;
            dstRect.w = w;
            dstRect.h = h;
            SDL_RenderCopyF(renderer, assets->grille, NULL, &dstRect);
            // Affiche le nouveau rendu
            SDL_RenderPresent(renderer);
        }
    }

    // Affichage du résultat
    printf("----------------------------------------\n");
    switch (Game_GetState(game))
    {
    case GAME_P1_WON:
        while (true)
        {
			if (input->quitPressed)
				break;
            
            // Met à jour le temps
            Timer_Update(g_time);

            // Met à jour les entrées
            Input_Update(input);

            if (input->quitPressed)
                break;

            // Efface le rendu précédent
            SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
            SDL_RenderClear(renderer);
            SDL_FRect dstRect;

            //renvoie de la grille
            dstRect.x = x;
            dstRect.y = y;
            dstRect.w = w;
            dstRect.h = h;
            if(ia == 2)
                SDL_RenderCopyF(renderer, assets->joueurwin, NULL, &dstRect);
            else
                SDL_RenderCopyF(renderer, assets->j1win, NULL, &dstRect);
            // Affiche le nouveau rendu
            SDL_RenderPresent(renderer);
        }
    case GAME_P2_WON:
        while (true)
        {
            if (input->quitPressed)
                break;

            // Met à jour le temps
            Timer_Update(g_time);

            // Met à jour les entrées
            Input_Update(input);

            if (input->quitPressed)
                break;

            // Efface le rendu précédent
            SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
            SDL_RenderClear(renderer);
            SDL_FRect dstRect;

            //renvoie de la grille
            dstRect.x = x;
            dstRect.y = y;
            dstRect.w = w;
            dstRect.h = h;
            if (ia == 2)
                SDL_RenderCopyF(renderer, assets->iawin, NULL, &dstRect);
            else
                SDL_RenderCopyF(renderer, assets->j2win, NULL, &dstRect);
            // Affiche le nouveau rendu
            SDL_RenderPresent(renderer);
        }

    case GAME_IS_TIED:
        while (true)
        {
            if (input->quitPressed)
                break;

            // Met à jour le temps
            Timer_Update(g_time);

            // Met à jour les entrées
            Input_Update(input);

            if (input->quitPressed)
                break;

            // Efface le rendu précédent
            SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
            SDL_RenderClear(renderer);
            SDL_FRect dstRect;

            //renvoie de la grille
            dstRect.x = x;
            dstRect.y = y;
            dstRect.w = w;
            dstRect.h = h;
            if (ia == 2)
                SDL_RenderCopyF(renderer, assets->iawin, NULL, &dstRect);
            else
                SDL_RenderCopyF(renderer, assets->j2win, NULL, &dstRect);
            // Affiche le nouveau rendu
            SDL_RenderPresent(renderer);
        }

    default:
        printf("Partie annulee\n");
        return EXIT_FAILURE;
        break;
    }

    Game_Print(game);
    Game_Delete(game);

    //--------------------------------------------------------------------------
    // Libération de la mémoire

    Assets_Delete(assets);
    assets = NULL;

    Input_Delete(input);
    input = NULL;

    Timer_Delete(g_time);
    g_time = NULL;

    SDL_DestroyRenderer(renderer);
    renderer = NULL;

    SDL_DestroyWindow(window);
    window = NULL;

    App_Quit();

    return EXIT_SUCCESS;
}