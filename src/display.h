#pragma once
#include <pebble.h>
	
typedef struct TimerDisplay TimerDisplay;
typedef enum {
    STOPPED, RUNNING
} TimerDisplayMode;

TimerDisplay *timer_display_create();
void timer_display_destroy(TimerDisplay *display);

void timer_display_set_click_config_provider(TimerDisplay *display, ClickConfigProvider provider, void *context);
void timer_display_set_mode(TimerDisplay *display, TimerDisplayMode mode);

void timer_display_update(TimerDisplay *display, int value);
void timer_display_alert(TimerDisplay *display);
void timer_display_warning(TimerDisplay *display);
void timer_display_cancel(TimerDisplay *display);