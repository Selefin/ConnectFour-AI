#include "Timer.h"

Timer* Timer_New()
{
    Timer *self = (Timer*)calloc(1, sizeof(Timer));
    AssertNew(self);

    self->m_currentTime = 0.0f;
    self->m_previousTime = 0.0f;

    self->m_delta = 0.0f;
    self->m_unscaledDelta = 0.0f;
    self->m_elapsed = 0.0f;
    self->m_unscaledElapsed = 0.0f;

    self->m_maxDelta = 0.2f;
    self->m_scale = 1.0f;

    return self;
}

void Timer_Delete(Timer* self)
{
    if (!self) return;

    free(self);
}

void Timer_Start(Timer* self)
{
    assert(self);
    self->m_currentTime = SDL_GetTicks() / 1000.f;
    self->m_previousTime = SDL_GetTicks() / 1000.f;
    self->m_delta = 0.0f;
}

void Timer_Update(Timer* self)
{
    assert(self);
    self->m_previousTime = self->m_currentTime;
    self->m_currentTime = SDL_GetTicks() / 1000.f;

    self->m_unscaledDelta = self->m_currentTime - self->m_previousTime;
    self->m_delta = self->m_scale * self->m_unscaledDelta;

    self->m_unscaledElapsed += self->m_unscaledDelta;
    self->m_elapsed += self->m_delta;
}