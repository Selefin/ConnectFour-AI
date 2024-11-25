#ifndef _AI_H_
#define _AI_H_

#include "Settings.h"
#include "Game.h"

/// @brief Fonction calculant l'indice de la colonne calculé par votre IA.
/// @param game la partie courante.
/// @param playerID l'identifiant de votre IA.
/// @return L'indice de la colonne choisie par l'IA.
int AI_ComputeMove(Game* game, int playerID, int depth);


//------------------------------------------------------------------------------
//  IMPORTANT
//  Les structures et prototypes suivants représentent une suggestion
//  d'implémentation, il ne sont pas imposés.
//------------------------------------------------------------------------------


/// @brief Structure représentant un noeud dans l'arbre de l'algorithme MinMax.
typedef struct AINode_s
{
    /// @brief Partie associée au noeud.
    /// Il s'agit d'une copie (modifiée) de la partie en cours.
    Game *gameState;

    /// @brief Score associée au noeud.
    int score;

    /// @brief Si le noeud n'est pas une feuille, ce champ désigne l'indice de
    /// colonne choisi par l'algorithme pour la partie associée à ce noeud.
    /// En particulier, l'action choisie par l'algorithme est la valeur
    /// du champ move de la racine de l'arbre.
    int move;

    /// @brief Pointeurs vers les 7 enfants du noeud.
    /// S'il n'est plus possible de jouer en colonne c, children[c] vaut NULL.
    /// Si le noeud est une feuille, les 7 pointeurs vallent NULL.
    struct AINode_s *children[GRID_W];
} AINode;

/// @brief Structure représentant l'arbre de l'algorithme MinMax.
typedef struct AITree_s
{
    /// @brief Pointeur vers la racine de l'arbre.
    AINode *root;
} AITree;

/// @brief Crée l'arbre de l'algorithme MinMax.
/// @param game la partie courante.
/// @param depth la hauteur maximale de la racine de l'arbre.
/// @param playerID identifiant du joueur dont l'algorithme calcule l'action.
/// @return L'arbre créé (et initialisé).
AITree *AITree_NewMinMax(Game *game, int depth, int playerID);

/// @brief Détruit l'arbre MinMax.
/// @param self l'arbre.
void AITree_Delete(AITree *self);

#endif
