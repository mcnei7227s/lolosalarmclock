/*
 * Timer.h - A Real-time Timer Library for Arduino & Wiring
 * Copyright (c) 2009 Daniel Bradberry.  All right reserved.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#ifndef Timer_h
#define Timer_h

//#include "WConstants.h"

#define MAX_TIMER 10
#define NULL_TIMER -1

#define INTERRUPT_INTERVAL 200



typedef void (*callback_h)(void);
typedef int timer_h;
typedef struct {
	int active;
	int enabled;
	int interval;
	int ticks;
	int shots;
	callback_h callback;
} timer_t;

class Timer {
public:
	Timer();
	timer_h once(int interval, callback_h callback);
	timer_h repeat(int interval, callback_h callback);
	timer_h repeat(int interval, int offset, callback_h callback);
	void enableAll();
	void disableAll();
	void trigger(timer_h timer);
	void enable(timer_h timer);
	void disable(timer_h timer);
	void destroy(timer_h timer);
	void tick();
	
private:
	int i_timer;
	timer_t timers[MAX_TIMER];
};

extern Timer RealTimer;

#endif
