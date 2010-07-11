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

#ifndef TimerPolling_h
#define TimerPolling_h

//#include "WConstants.h"

#define MAX_TIMER 20
#define NULL_TIMER -1

#define secondsU 1000
#define minutesU 1000*60
#define hoursU 1000*60*60



typedef void (*callback_h)(void);
typedef int timer_h;
typedef struct {
	char active;
	char enabled;
	long interval;
	long lastshot;
	int shots;
	callback_h callback;
} timer_t;

class TimerPolling {
public:
	TimerPolling();
	timer_h once(long interval, callback_h callback);
	void repeat(long interval, callback_h callback, timer_h timer);
	timer_h repeat(long interval, callback_h callback);
	void repeat(long interval, long offset, callback_h callback, timer_h timer);
	timer_h repeat(long interval, long offset, callback_h callback);
	void enableAll();
	void disableAll();
	void trigger(timer_h timer);
	void changeSchedule(timer_h timer, long interval);
	void enable(timer_h timer);
	void disable(timer_h timer);
	void destroy(timer_h timer);
	void tick();
	
private:
	int i_timer;
	timer_t timers[MAX_TIMER];
};

extern TimerPolling Timer;

#endif
