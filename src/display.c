#include "display.h"
#include "util.h"

typedef struct TimerDisplay {
    Window *window;
    TextLayer *time_text;
    ActionBarLayer *action_bar;
    InverterLayer *inverter;
    GBitmap *bitmap_plus_icon;
    GBitmap *bitmap_play_icon;
    GBitmap *bitmap_minus_icon;
    GBitmap *bitmap_pause_icon;
} TimerDisplay;

TimerDisplay* timer_display_create() {
    TimerDisplay* self = calloc(1, sizeof(TimerDisplay));

    self->bitmap_plus_icon = gbitmap_create_with_resource(RESOURCE_ID_PLUS_ICON);
    self->bitmap_play_icon = gbitmap_create_with_resource(RESOURCE_ID_PLAY_ICON);
    self->bitmap_minus_icon = gbitmap_create_with_resource(RESOURCE_ID_MINUS_ICON);
    self->bitmap_pause_icon = gbitmap_create_with_resource(RESOURCE_ID_PAUSE_ICON);
    
    self->window = window_create();
    window_stack_push(self->window, true);
  	window_set_background_color(self->window, GColorWhite);

	self->time_text = text_layer_create(GRect(0, 48, 144 - ACTION_BAR_WIDTH, 138));
	text_layer_set_text_color(self->time_text, GColorBlack);
    text_layer_set_text_alignment(self->time_text, GTextAlignmentCenter);
	text_layer_set_background_color(self->time_text, GColorClear);
	text_layer_set_font(self->time_text, fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_DOSIS_WATCH_46)));
	layer_add_child(window_get_root_layer(self->window), text_layer_get_layer(self->time_text));

    self->action_bar = action_bar_layer_create();
    action_bar_layer_add_to_window(self->action_bar, self->window);

    self->inverter = inverter_layer_create(GRect(0, 0, 144, 168));
    layer_set_hidden(inverter_layer_get_layer(self->inverter), true);
    layer_add_child(window_get_root_layer(self->window), inverter_layer_get_layer(self->inverter));

    timer_display_update(self, 0);
    timer_display_set_mode(self, STOPPED);
    return self;
}

void timer_display_destroy(TimerDisplay *display) {
	window_destroy(display->window);
	text_layer_destroy(display->time_text);
    action_bar_layer_destroy(display->action_bar);
    inverter_layer_destroy(display->inverter);
    gbitmap_destroy(display->bitmap_play_icon);
    gbitmap_destroy(display->bitmap_plus_icon);
    gbitmap_destroy(display->bitmap_minus_icon);
    gbitmap_destroy(display->bitmap_pause_icon);
    free(display);
}

void timer_display_update(TimerDisplay *display, int time_left) {
    static char time_text[] = "00:00";
    struct tm pebble_time_left = {
        .tm_sec = time_left%60,
        .tm_min = time_left/60
    };

	strftime(time_text, sizeof(time_text), "%M:%S", &pebble_time_left);
    text_layer_set_text(display->time_text, time_text);
}

void timer_display_set_click_config_provider(TimerDisplay *display, ClickConfigProvider provider, void *context) {
    action_bar_layer_set_context(display->action_bar, context);
    action_bar_layer_set_click_config_provider(display->action_bar, provider);
}

void timer_display_set_mode(TimerDisplay *display, TimerDisplayMode mode) {
    switch (mode) {
        case STOPPED:
            action_bar_layer_set_icon(display->action_bar, BUTTON_ID_UP, display->bitmap_plus_icon);
            action_bar_layer_set_icon(display->action_bar, BUTTON_ID_SELECT, display->bitmap_play_icon);
            action_bar_layer_set_icon(display->action_bar, BUTTON_ID_DOWN, display->bitmap_minus_icon);
            break;
        default:
            action_bar_layer_set_icon(display->action_bar, BUTTON_ID_UP, NULL);
            action_bar_layer_set_icon(display->action_bar, BUTTON_ID_SELECT, display->bitmap_pause_icon);
            action_bar_layer_set_icon(display->action_bar, BUTTON_ID_DOWN, NULL);
            break;
    }
}

void timer_display_alert(TimerDisplay *display) {
    layer_set_hidden(inverter_layer_get_layer(display->inverter), true);
    vibes_short_pulse();
}

void timer_display_warning(TimerDisplay *display) {
    layer_set_hidden(inverter_layer_get_layer(display->inverter),
                     !layer_get_hidden(inverter_layer_get_layer(display->inverter)));
}

void timer_display_cancel(TimerDisplay *display) {
    layer_set_hidden(inverter_layer_get_layer(display->inverter), true);
    vibes_cancel();
}
