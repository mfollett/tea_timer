#include <pebble.h>
#include "controller.h"
#include "util.h"

#define BUTTON_CLICK_TIME_SHIFT 30
#define INITIAL_TIME 3*60

typedef struct TimerController {
    TimerDisplay *display;
    int timer_running;
    int time_left;
    TickSource tick_source;
} TimerController;

void click_config_provider(void *context);
void toggle_timer(ClickRecognizerRef recognizer, void *context);
void handle_up(ClickRecognizerRef recognizer, void *context);
void handle_down(ClickRecognizerRef recognizer, void *context);

TimerController *timer_controller_create(TimerDisplay *display, TickSource source) {
    TimerController *self = calloc(1, sizeof(TimerController));
    self->display = display;
    self->time_left = INITIAL_TIME;
    self->tick_source = source;
    timer_display_set_click_config_provider(self->display, click_config_provider, self);
    timer_display_update(self->display, self->time_left);
    return self;
}

void timer_controller_destroy(TimerController *controller) {
    timer_display_destroy(controller->display);
    free(controller);
}

void timer_controller_tick(TimerController *controller) {
    controller->time_left = MAX(0, controller->time_left-1);

    if(controller->time_left == 0) {
        timer_display_alert(controller->display);
    }

    if(controller->time_left < 5 && controller->time_left > 0) {
        timer_display_warning(controller->display);
    }

    timer_display_update(controller->display, controller->time_left);
}

void click_config_provider(void *context) {
	window_set_click_context(BUTTON_ID_UP, context);
	window_set_click_context(BUTTON_ID_SELECT, context);
	window_set_click_context(BUTTON_ID_DOWN, context);
	window_single_repeating_click_subscribe(BUTTON_ID_UP, 60, handle_up);
	window_single_click_subscribe(BUTTON_ID_SELECT, toggle_timer);
	window_single_repeating_click_subscribe(BUTTON_ID_DOWN, 60, handle_down);
}

void toggle_timer(ClickRecognizerRef recognizer, void *context) {
    TimerController *controller = (TimerController *)context;
	TOGGLE(controller->timer_running);
    timer_display_cancel(controller->display);
    timer_display_set_mode(controller->display, controller->timer_running);
    controller->tick_source(controller->timer_running);
}

void handle_up(ClickRecognizerRef recognizer, void *context) {
    TimerController *controller = (TimerController *)context;
    if(!controller->timer_running) {
        controller->time_left = MIN(controller->time_left
                                    + BUTTON_CLICK_TIME_SHIFT
                                    - (controller->time_left % BUTTON_CLICK_TIME_SHIFT),
                                    99*60);
    }

    timer_display_update(controller->display, controller->time_left);
}

void handle_down(ClickRecognizerRef recognizer, void *context) {
    TimerController *controller = (TimerController *)context;
    if(!controller->timer_running) {
        int mod = controller->time_left % BUTTON_CLICK_TIME_SHIFT;
        controller->time_left = MAX(controller->time_left - (mod > 0 ? mod : BUTTON_CLICK_TIME_SHIFT), 0);
    }

    timer_display_update(controller->display, controller->time_left);
}