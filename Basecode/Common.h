#pragma once

#include "Settings.h"
#include "Timer.h"

void App_Init(int sdlFlags, int imgFlags);
void App_Quit();

extern Timer *g_time;
