#include "display.h"
#include "time_handler.h"

#ifndef TOGGLE
#define TOGGLE(x) x = !x
#endif

bool flash_background = true;
bool light_enabled    = false;

static Window *window = NULL;
static TextLayer *timer = NULL;
static ActionBarLayer *action_bar = NULL;

static GBitmap *bitmap_plus_icon = NULL;
static GBitmap *bitmap_play_icon = NULL;
static GBitmap *bitmap_minus_icon = NULL;

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
	ActionBarLayer *old_action_bar = action_bar;

    if (!bitmap_play_icon) {
        bitmap_play_icon = gbitmap_create_with_resource(RESOURCE_ID_PLAY_ICON);
    }
    
    if (!bitmap_plus_icon) {
        bitmap_plus_icon = gbitmap_create_with_resource(RESOURCE_ID_PLUS_ICON);
    }

    if (!bitmap_minus_icon) {
        bitmap_minus_icon = gbitmap_create_with_resource(RESOURCE_ID_MINUS_ICON);
    }

	window = window_create();
	window_stack_push(window, true);
  	window_set_background_color(window, GColorBlack);

	timer = text_layer_create(GRect(0, 48, 144 - ACTION_BAR_WIDTH, 138));
	text_layer_set_text_color(timer, GColorWhite);
    text_layer_set_text_alignment(timer, GTextAlignmentCenter);
	text_layer_set_background_color(timer, GColorClear);
	text_layer_set_font(timer, fonts_get_system_font(FONT_KEY_BITHAM_42_MEDIUM_NUMBERS));
	
	layer_add_child(window_get_root_layer(window), text_layer_get_layer(timer));
	text_layer_set_text(timer, "Starting...");

    action_bar = action_bar_layer_create();
    action_bar_layer_set_background_color(action_bar, GColorClear);
    action_bar_layer_set_icon(action_bar, BUTTON_ID_UP, bitmap_plus_icon);
    action_bar_layer_set_icon(action_bar, BUTTON_ID_SELECT, bitmap_play_icon);
    action_bar_layer_set_icon(action_bar, BUTTON_ID_DOWN, bitmap_minus_icon);

    action_bar_layer_add_to_window(action_bar, window);

	update_display_with_time((int) current_time());
	
	if (old_window) { window_destroy(old_window); }
	if (old_timer) { text_layer_destroy(old_timer); }
    if (old_action_bar) { action_bar_layer_destroy(old_action_bar); }
}

void deinitialize_display() {
	if (window) { window_destroy(window); window = NULL; }
	if (timer) { text_layer_destroy(timer); timer = NULL; }
    if (action_bar) { action_bar_layer_destroy(action_bar); action_bar = NULL; }
    if (bitmap_play_icon) { gbitmap_destroy(bitmap_play_icon); bitmap_play_icon = NULL; }
    if (bitmap_plus_icon) { gbitmap_destroy(bitmap_plus_icon); bitmap_plus_icon = NULL; }
    if (bitmap_minus_icon) { gbitmap_destroy(bitmap_minus_icon); bitmap_minus_icon = NULL; }
}
