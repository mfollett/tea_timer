//#include <time.h> //already defined in <pebble.h>!
#include "display.h"
#include "time_handler.h"

#ifndef TOGGLE
#define TOGGLE(x) x = !x
#endif

bool flash_background = true;
bool light_enabled    = false;

Window *window = NULL;
TextLayer *timer = NULL;

void update_display_with_time(int time_left) {
	if (!timer) { return; }
	
    static char time_text[] = "00:00:00";
    struct tm pebble_time_left = {
        .tm_sec = time_left%60,
        .tm_min = time_left/60
    };

	strftime(time_text, sizeof(time_text), "%M:%S", &pebble_time_left);

    text_layer_set_text(timer, time_text);
}

void alert() {
    vibes_short_pulse();
    TOGGLE(light_enabled);
    // XXX Need to figure out how to do this in a way that doesn't cause the
    // light to just stay on.
    //light_enable(light_enabled);
}

void warning() {
    int foreground = (flash_background ? GColorBlack : GColorWhite);
    int background = (flash_background ? GColorWhite : GColorBlack);
    text_layer_set_text_color(timer, foreground);
    window_set_background_color(window, background);
    TOGGLE(flash_background);
}

Window* get_window() {
    return window;
}

void initialize_display() {
	Window *old_window = window;
	TextLayer *old_timer = timer;
	
	window = window_create();
	window_stack_push(window, true);
  	window_set_background_color(window, GColorBlack);

	timer = text_layer_create(GRect(14, 49, 130, 50));
	text_layer_set_text_color(timer, GColorWhite);
	text_layer_set_background_color(timer, GColorClear);
	//text_layer_set_text_alignment(&timer, GTextAlignmentCenter);
	//text_layer_set_font(&timer, fonts_get_system_font(FONT_KEY_GOTHAM_30_BLACK));
	text_layer_set_font(timer, fonts_get_system_font(FONT_KEY_BITHAM_42_MEDIUM_NUMBERS));
	
	layer_add_child(window_get_root_layer(window), text_layer_get_layer(timer));
	text_layer_set_text(timer, "Starting...");
	
	update_display_with_time((int) current_time());
	
	if (old_window) { window_destroy(old_window); }
	if (old_timer) { text_layer_destroy(old_timer); }
}

void deinitialize_display() {
	if (window) { window_destroy(window); window = NULL; }
	if (timer) { text_layer_destroy(timer); timer = NULL; }
}
