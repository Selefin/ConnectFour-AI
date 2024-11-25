#ifndef _GAME_H_
#define _GAME_H_

#include "Settings.h"

#define GRID_H 6
#define GRID_W 7

/// @brief Enumération des identifiants des joueurs.
typedef enum PlayerID_e
{
    /// @brief Identifiant du joueur 1.
    PLAYER_1 = 1,
    /// @brief Identifiant du joueur 2.
    PLAYER_2 = 2
} PlayerID;

/// @brief Renvoie l'identifiant de l'autre joueur.
/// On a donc PlayerID_GetOther(PLAYER_1) = PLAYER_2 et inversement.
/// @param playerID l'identifiant du joueur.
/// @return L'identifiant de l'autre joueur.
INLINE int PlayerID_GetOther(int playerID)
{
    return 3 - playerID;
}

/// @brief Enumération des états possibles d'une case du plateau.
typedef enum CellState_e
{
    /// @brief Indique que la case du plateau est vide.
    CELL_EMPTY,
    /// @brief Indique que la case du plateau
    /// est occupée par un pion du joueur 1.
    CELL_P1 = 1,
    /// @brief Indique que la case du plateau
    /// est occupée par un pion du joueur 2.
    CELL_P2 = 2
} CellState;

/// @brief Enumération des états possibles de la partie.
typedef enum GameState_e
{
    /// @brief Indique que la partie est en cours.
    GAME_IN_PROGRESS,
    /// @brief Indique que la partie est remportée par le joueur 1.
    GAME_P1_WON = 1,
    /// @brief Indique que la partie est remportée par le joueur 2.
    GAME_P2_WON = 2,
    /// @brief Indique que la partie s'est terminée sur une égalitée.
    GAME_IS_TIED
} GameState;

/// @brief Structure représentant une partie de puissance 4.
typedef struct Game_s
{
    /// @brief Plateau du jeu.
    /// L'indice de ligne 0 désigne la ligne du bas.
    /// L'indice de colonne 0 désigne la colonne de gauche.
    /// Les valeurs possibles des cases sont définies dans CellState.
    int grid[GRID_H][GRID_W];

    /// @brief Identifiant du joueur courant.
    /// Les valeurs possibles sont définies dans PlayerID.
    int playerID;

    /// @brief Etat de la partie.
    /// Les valeurs possibles sont définies dans GameState.
    int state;
} Game;

/// @brief Crée une nouvelle partie de puissance 4.
/// @return La partie créée.
Game *Game_New();

/// @brief Détruit une partie préalablement créée avec Game_New() ou Game_Copy().
/// @param self la partie.
void Game_Delete(Game *self);

/// @brief Copie une partie.
/// @param self la partie.
/// @return Une copie en profondeur de la partie.
Game *Game_Copy(Game *self);

/// @brief Affiche une partie sur la sortie standard.
/// @param self la partie.
void Game_Print(Game *self);

/// @brief Indique si le joueur courant peut jouer son pion dans une certaine colonne.
/// Cette fonction renvoie false si l'indice de la colonne n'est pas valide,
/// si la colonne est déjà remplie ou si la partie est déjà terminée.
/// Elle renvoie true autrement.
/// @param self la partie.
/// @param column l'indice de la colonne.
/// @return true s'il est possible de jouer dans la colonne, false sinon.
bool Game_CanPlayAt(Game *self, int column);

/// @brief Effectue l'action du joueur courant puis met à jour la partie.
/// On suppose qu'il est possible de jouer dans la colonne indiquée.
/// L'utilisateur doit le vérifier avec Game_CanPlayAt() avant d'appeler cette méthode.
/// @param self la partie.
/// @param column l'indice de la colonne.
void Game_PlayTurn(Game *self, int column);

/// @brief Renvoie l'état de la partie.
/// Les valeurs possibles sont définies dans l'énumération GameState.
/// @param self la partie.
/// @return L'état de la partie.
INLINE int Game_GetState(Game *self)
{
    return self->state;
}

/// @brief Revoie l'identifiant du joueur courant.
/// @param self la partie.
/// @return L'identifiant du joueur courant.
INLINE int Game_GetPlayerID(Game *self)
{
    return self->playerID;
}

#endif
