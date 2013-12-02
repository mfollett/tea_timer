// gives me timer_t
#include "sys/types.h"

#ifndef __MY_TIME_HANDLER_H
#define __MY_TIME_HANDLER_H

void add_time(timer_t amount);
void subtract_time(timer_t amount);

timer_t current_time();
void set_time(timer_t t);
void decrement_time();

void initialize_time_handler();
void deinitialize_time_handler();

#endif