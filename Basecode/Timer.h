#pragma once

#include "Settings.h"

/// @brief Structure représentant un chronomètre.
typedef struct Timer_s
{
    /// @brief Temps de départ.
    float m_startTime;

    /// @brief Temps du dernier appel à Timer_Update().
    float m_currentTime;

    /// @brief Temps de l'avant dernier appel à Timer_Update().
    float m_previousTime;

    /// @brief Ecart entre les deux derniers appels à Timer_Update().
    float m_delta;
    float m_unscaledDelta;

    float m_scale;

    float m_maxDelta;

    float m_elapsed;
    float m_unscaledElapsed;
} Timer;

/// @brief Crée un nouveau timer.
/// @return Le timer créé ou NULL en cas d'erreur.
Timer* Timer_New();

/// @brief Détruit un timer préalablement alloué avec Timer_New().
/// @param[in,out] self le timer à détruire.
void Timer_Delete(Timer* self);

/// @brief Lance le timer.
/// @param[in,out] self le timer.
void Timer_Start(Timer* self);

/// @brief Met à jour le timer en fonction du temps actuel.
/// Cette fonction est typiquement appelée à chaque tour de la boucle de rendu.
/// @param[in,out] self le timer.
void Timer_Update(Timer* self);

INLINE void Timer_SetMaximumDeltaTime(Timer *self, float maxDelta)
{
    self->m_maxDelta = maxDelta;
}

INLINE void Timer_SetTimeScale(Timer *self, float scale)
{
    assert(scale >= 0.0f);
    self->m_scale = scale;
}

INLINE float Timer_GetTimeScale(Timer *self)
{
    return self->m_scale;
}

/// @brief Renvoie l'écart de temps (en secondes) entre les deux derniers appels à la fonction
/// Timer_Update().
/// @param[in] self le timer.
/// @return L'écart de temps entre les deux dernières mises à jour.
INLINE float Timer_GetDelta(Timer *self)
{
    return self->m_delta;
}

INLINE float Timer_GetUnscaledDelta(Timer *self)
{
    return self->m_unscaledDelta;
}

/// @brief Renvoie l'écart de temps (en secondes) entre le lancement du timer avec la fonction
/// Timer_Start() et le dernier appel à la fonction Timer_Update().
/// @param[in] self le timer.
/// @return Le nombre de secondes écoulées depuis le lancement du timer et la dernière mise à jour.
INLINE float Timer_GetElapsed(Timer *self)
{
    return self->m_elapsed;
}

INLINE float Timer_GetUnscaledElapsed(Timer *self)
{
    return self->m_unscaledElapsed;
}
