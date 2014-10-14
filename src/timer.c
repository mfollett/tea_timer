#include <pebble.h>
#include "time_handler.h"
#include "display.h"

#ifndef TOGGLE
#define TOGGLE(x) x = !x
#endif

#define BUTTON_CLICK_TIME_SHIFT 30
#define INITIAL_TIME 3*60

bool timer_running = false;

void handle_second_tick() {
    int time = current_time();

	decrement_time();

    if(time == 0) {
        alert();
    }

    if( time < 5 && time > 0) {
        warning();
    }

    update_display_with_time(time);
}

void toggle_timer(ClickRecognizerRef recognizer, void *context) {
	TOGGLE(timer_running);
	if ( timer_running ) {
		tick_timer_service_subscribe(SECOND_UNIT, (TickHandler) handle_second_tick);
	} else {
		tick_timer_service_unsubscribe();
	}
}

void handle_up(ClickRecognizerRef recognizer, void *context) {
    if( ! timer_running ) {
        add_time(BUTTON_CLICK_TIME_SHIFT);
        update_display_with_time(current_time());
    }
}

void handle_down(ClickRecognizerRef recognizer, void *context) {
    if( ! timer_running ) {
        subtract_time(BUTTON_CLICK_TIME_SHIFT);
        update_display_with_time(current_time());
    }
}

void click_config_provider(void *context) {
	window_set_click_context(BUTTON_ID_UP, context);
	window_set_click_context(BUTTON_ID_SELECT, context);
	window_set_click_context(BUTTON_ID_DOWN, context);
	window_single_repeating_click_subscribe(BUTTON_ID_UP, 60, handle_up);
	window_single_click_subscribe(BUTTON_ID_SELECT, toggle_timer);
	window_single_repeating_click_subscribe(BUTTON_ID_DOWN, 60, handle_down);
}

void handle_init() {
	initialize_time_handler();
	set_time(INITIAL_TIME);
	initialize_display();
	window_set_click_config_provider(get_window(), click_config_provider);
}

void handle_deinit(void) {
	deinitialize_time_handler();
	deinitialize_display();
}

int main(void) {
	handle_init();
	app_event_loop();
	handle_deinit();
}
