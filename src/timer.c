#include <pebble.h>
#include "display.h"
#include "controller.h"

TimerController *controller;

void handle_tick(struct tm *tick_time, TimeUnits units_changed);
    
void tick_source_control(int enabled) {
    if (enabled) {
        tick_timer_service_subscribe(SECOND_UNIT, handle_tick);
    } else {
        tick_timer_service_unsubscribe();
    }
}

void handle_tick(struct tm *tick_time, TimeUnits units_changed) {
    if (units_changed & SECOND_UNIT) {
        timer_controller_tick(controller);
    }
}

void handle_init() {
    TimerDisplay *display = timer_display_create();
    controller = timer_controller_create(display, tick_source_control);
}

void handle_deinit(void) {
    timer_controller_destroy(controller);
}

int main(void) {
	handle_init();
	app_event_loop();
	handle_deinit();
}
