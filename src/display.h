#include <pebble.h>
	
#ifndef __MY_DISPLAY_H
#define __MY_DISPLAY_H

void update_display_with_time(int display);
void alert();
void warning();
void initialize_display();
void deinitialize_display();
Window* get_window();

#endif