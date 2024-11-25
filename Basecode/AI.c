#include "AI.h"

INLINE int Int_Max(int a, int b) { return a > b ? a : b; }
INLINE int Int_Min(int a, int b) { return a < b ? a : b; }

int Game_Eval(Game* self, int playerID)
{
    /////////
    // TODO : Implémentez votre fonction d'évaluation ici.
    // Si la partie est en faveur du joueur playerID, le score est positif.
    // Si elle est en faveur de l'autre joueur, le score est négatif.
    // Cette fonction est appelée par AINode_NewMinMax() pour initialiser
    // les scores des feuilles de l'arbre MinMax.

    int score = 0;
    for (int i = 0; i < GRID_H; i++)
    {
        // Score colonne centrale
        score += (self->grid[i][GRID_W / 2] == playerID) ? 60 : (self->grid[i][GRID_W / 2] == (3 - playerID) ? -60 : 0);
        for (int j = 0; j < GRID_W; j++)
        {
            // Score lignes
            if (j < GRID_W - 3)
            {
                int lineArray[4] = { self->grid[i][j], self->grid[i][j + 1], self->grid[i][j + 2], self->grid[i][j + 3] };
                int lineScore = 0;
                for (int k = 0; k < 4; k++)
                    lineScore += (lineArray[k] == playerID) ? 1 : (lineArray[k] == (3 - playerID) ? -1 : 0);
                score += (lineScore == 4) ? 1000 : (lineScore == -4) ? -1000 : (lineScore == 3) ? 100 : (lineScore == -3) ? -100 : (lineScore == 2) ? 10 : (lineScore == -2) ? -10 : (lineScore == 1) ? 1 : (lineScore == -1) ? -1 : 0;
            }
            // Score colonnes
            if (i < GRID_H - 3)
            {
                int columnArray[4] = { self->grid[i][j], self->grid[i + 1][j], self->grid[i + 2][j], self->grid[i + 3][j] };
                int columnScore = 0;
                for (int k = 0; k < 4; k++)
                    columnScore += (columnArray[k] == playerID) ? 1 : (columnArray[k] == (3 - playerID) ? -1 : 0);
                score += (columnScore == 4) ? 1000 : (columnScore == -4) ? -1000 : (columnScore == 3) ? 100 : (columnScore == -3) ? -100 : (columnScore == 2) ? 10 : (columnScore == -2) ? -10 : (columnScore == 1) ? 1 : (columnScore == -1) ? -1 : 0;
            }
            // Score diagonales à pentes positives
            if (i < GRID_H - 3 && j < GRID_W - 3)
            {
                int diagonalArray[4] = { self->grid[i][j], self->grid[i + 1][j + 1], self->grid[i + 2][j + 2], self->grid[i + 3][j + 3] };
                int diagonalScore = 0;
                for (int k = 0; k < 4; k++)
                    diagonalScore += (diagonalArray[k] == playerID) ? 1 : (diagonalArray[k] == (3 - playerID) ? -1 : 0);
                score += (diagonalScore == 4) ? 1000 : (diagonalScore == -4) ? -1000 : (diagonalScore == 3) ? 100 : (diagonalScore == -3) ? -100 : (diagonalScore == 2) ? 10 : (diagonalScore == -2) ? -10 : (diagonalScore == 1) ? 1 : (diagonalScore == -1) ? -1 : 0;
            }
            // Score diagonales à pentes négatives
            if (i > 2 && j < GRID_W - 3)
            {
                int diagonalArray[4] = { self->grid[i][j], self->grid[i - 1][j + 1], self->grid[i - 2][j + 2], self->grid[i - 3][j + 3] };
                int diagonalScore = 0;
                for (int k = 0; k < 4; k++)
                    diagonalScore += (diagonalArray[k] == playerID) ? 1 : (diagonalArray[k] == (3 - playerID) ? -1 : 0);
                score += (diagonalScore == 4) ? 1000 : (diagonalScore == -4) ? -1000 : (diagonalScore == 3) ? 100 : (diagonalScore == -3) ? -100 : (diagonalScore == 2) ? 10 : (diagonalScore == -2) ? -10 : (diagonalScore == 1) ? 1 : (diagonalScore == -1) ? -1 : 0;
            }
        }
    }
    return score;
}

/// @brief Crée un noeud de l'arbre de l'algorithme MinMax.
/// Cette méthode est appelée par AITree_NewMinMax().
/// @param game la partie associée au noeud.
/// @param depth la hauteur maximale du noeud.
/// @param minLayer indique si le noeud est un noeud MIN.
/// @param playerID identifiant du joueur dont l'algorithme calcule l'action.
/// @return Le noeud créé (et initialisé).
AINode* AINode_NewMinMax(Game* game, int depth, bool maxLayer, int playerID, int alpha, int beta)
{
    AINode* self = (AINode*)calloc(1, sizeof(AINode));
    AssertNew(self);

    /////////
    // TODO : Implémentez l'algorithme MinMax
    self->gameState = game;
    if (self->gameState->state != GAME_IN_PROGRESS)
    {
        if (self->gameState->state == playerID)
            self->score = 1000;
        else if (self->gameState->state == PlayerID_GetOther(playerID))
            self->score = -1000;
        else
            self->score = 0;
        return self;
    }
    if (depth == 0)
    {
        self->score = Game_Eval(self->gameState, playerID);
        return self;
    }

    int bestScore = (maxLayer) ? -1000000 : 1000000;
    int bestMove = -1;
    for (int i = 0; i < GRID_W; i++)
    {
        if (Game_CanPlayAt(self->gameState, i))
        {
            Game* newGame = Game_Copy(self->gameState);
            Game_PlayTurn(newGame, i);
            self->children[i] = AINode_NewMinMax(newGame, depth - 1, !maxLayer, playerID, alpha, beta);
            if (maxLayer)
            {
                if (self->children[i]->score > bestScore)
                {
                    bestScore = self->children[i]->score;
                    bestMove = i;
                }
                alpha = Int_Max(alpha, bestScore);
                if (alpha >= beta)
                    break;
            }
            else
            {
                if (self->children[i]->score < bestScore)
                {
                    bestScore = self->children[i]->score;
                    bestMove = i;
                }
                beta = Int_Min(beta, bestScore);
                if (beta <= alpha)
                    break;
            }
        }
    }

    self->score = bestScore;
    self->move = bestMove;
    return self;
}

AITree* AITree_NewMinMax(Game* game, int depth, int playerID)
{
    AITree* self = (AITree*)calloc(1, sizeof(AITree));
    AssertNew(self);

    Game* curGame = Game_Copy(game);
    self->root = AINode_NewMinMax(curGame, depth, true, playerID, -1000000, 1000000);

    return self;
}

/// @brief Détruit récursivement un noeud et tous ses sous-arbres.
/// @param self le noeud.
void AINode_DeleteRec(AINode* self)
{
    /////////
    // TODO : Implémentez la destruction du noeud et de ses enfants
    // Pensez également à détruire la copie de la partie de chaque noeud.
    if (!self) return;
    Game_Delete(self->gameState);
    for (int i = 0; i < GRID_W; i++)
        AINode_DeleteRec(self->children[i]);
    free(self);
}

///Détruit l'arbre passé en paramètre
void AITree_Delete(AITree* self)
{
    if (!self) return;

    AINode_DeleteRec(self->root);
    free(self);
}

int AI_ComputeMove(Game* game, int playerID, int depth)
{
    /////////
    // TODO : Cette fonction représente votre IA.
    // Modifiez-la comme vous le souhaitez.
    if (depth == 0)
    {
        srand(time(NULL));
        do
        {
            depth = rand() % GRID_W;
        } while (!Game_CanPlayAt(game, depth)); // depth correspond au mouvement à faire ici
        return depth;
    }
    AITree* aiTree = AITree_NewMinMax(game, depth, playerID);
    int move = 0;
    int bestScore = -1000000;
    for (int i = 0; i < GRID_W; i++)
    {
        if (aiTree->root->children[i] && aiTree->root->children[i]->score > bestScore)
        {
            bestScore = aiTree->root->children[i]->score;
            move = i;
        }
    }
    AITree_Delete(aiTree);

    return move;
}