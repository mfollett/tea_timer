#pragma once

#include "display.h"
typedef struct TimerController TimerController;
typedef void (*TickSource)(int enabled);

TimerController *timer_controller_create(TimerDisplay *display, TickSource source);
void timer_controller_destroy(TimerController *controller);
void timer_controller_tick(TimerController *controller);