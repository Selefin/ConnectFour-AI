#include "Game.h"

Game* Game_New()
{
    Game* self = (Game*)calloc(1, sizeof(Game));
    AssertNew(self);

    self->state = GAME_IN_PROGRESS;
    self->playerID = PLAYER_1;

    return self;
}

void Game_Delete(Game* self)
{
    if (!self) return;
    free(self);
}

Game* Game_Copy(Game* self)
{
    /////////
    // TODO : Allouez une nouvelle partie puis copiez les membres de self
    Game* copy = Game_New();
    AssertNew(copy);
    *copy = *self;
    return copy;
}

/// @brief Met à jour l'état de la partie.
/// Cette méthode est appelée par Game_PlayTurn().
/// @param self la partie.
void Game_UpdateState(Game* self)
{
    /////////
    // TODO : Calculez ici l'état de la partie
    // Parcourez tous les alignements possibles
    // Le résultat est ensuite affecté à self->state

    // Iterate over the rows.
    for (int i = 0; i < GRID_H; i++)
    {
        // Iterate over the columns.
        for (int j = 0; j < GRID_W; j++)
        {
            // Check for horizontal wins.
            if (j <= GRID_W - 4)
            {
                if ((self->grid[i][j] != CELL_EMPTY) && (self->grid[i][j] == self->grid[i][j + 1]) && (self->grid[i][j + 2] == self->grid[i][j + 3]) && (self->grid[i][j + 1] == self->grid[i][j + 2]))
                {
                    self->state = self->grid[i][j];
                    return;
                }
            }

            // Check for vertical wins.
            if (i <= GRID_H - 4)
            {
                if ((self->grid[i][j] != CELL_EMPTY) && (self->grid[i][j] == self->grid[i + 1][j]) && (self->grid[i + 2][j] == self->grid[i + 3][j]) && (self->grid[i + 1][j] == self->grid[i + 2][j]))
                {
                    self->state = self->grid[i][j];
                    return;
                }
            }

            // Check for diagonal wins.
            if (i <= GRID_H - 4 && j <= GRID_W - 4)
            {
                if ((self->grid[i][j] != CELL_EMPTY) && (self->grid[i][j] == self->grid[i + 1][j + 1]) && (self->grid[i + 2][j + 2] == self->grid[i + 3][j + 3]) && (self->grid[i + 1][j + 1] == self->grid[i + 2][j + 2]))
                {
                    self->state = self->grid[i][j];
                    return;
                }
            }
            if (i <= GRID_H - 4 && j >= 3)
            {
                if ((self->grid[i][j] != CELL_EMPTY) && (self->grid[i][j] == self->grid[i + 1][j - 1]) && (self->grid[i + 2][j - 2] == self->grid[i + 3][j - 3]) && (self->grid[i + 1][j - 1] == self->grid[i + 2][j - 2]))
                {
                    self->state = self->grid[i][j];
                    return;
                }
            }
        }
    }
    if ((self->grid[5][0] != CELL_EMPTY) && (self->grid[5][1] != CELL_EMPTY) && (self->grid[5][2] != CELL_EMPTY) && (self->grid[5][3] != CELL_EMPTY) && (self->grid[5][4] != CELL_EMPTY) && (self->grid[5][5] != CELL_EMPTY) && (self->grid[5][6] != CELL_EMPTY))
    {
        self->state = GAME_IS_TIED;
        return;

    }
    self->state = GAME_IN_PROGRESS;
}

bool Game_CanPlayAt(Game* self, int column)
{
    assert(self);

    /////////
    // TODO : Implémentez cette fonction.
    if (column > 6 || column < 0)
    {
        printf("\n");
        printf("\xC9\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xBB\n");
        printf("\xBA veuillez entrer un chiffre entre 0 et 6 \xBA\n");
        printf("\xC8\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xBC\n");
        printf("\n");
        return false;
    }
    return self->grid[5][column] ? false : true;
}

void Game_PlayTurn(Game* self, int column)
{
    assert(self);
    assert(Game_CanPlayAt(self, column));

    /////////
    // TODO : Effectuez l'action du joueur self->playerID
    for (size_t i = 0; i < GRID_H; i++)
    {
        if (!self->grid[i][column])
        {
            self->grid[i][column] = self->playerID;
            i = GRID_H;
        }
    }

    // Met à jour l'état de la partie
    Game_UpdateState(self);

    // Change le joueur courant
    self->playerID = PlayerID_GetOther(self->playerID);
}

void Game_Print(Game* self)
{
    /////////
    // TODO : Implémentez un bel affichage de la partie
    printf("  0   1   2   3   4   5   6\n-----------------------------\n");
    for (int i = 5; i >= 0; i--)
    {
        for (int j = 0; j < GRID_W; j++)
        {
            if (self->grid[i][j] == CELL_EMPTY)
                printf("|   ");
            else if (self->grid[i][j] == CELL_P1)
                printf("| X ");
            else if (self->grid[i][j] == CELL_P2)
                printf("| 0 ");
        }
        printf("|\n-----------------------------\n");
    }
}