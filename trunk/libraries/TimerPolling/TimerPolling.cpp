/*
 * Timer.cpp - A Real-time Timer Library for Arduino & Wiring
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

#include "/usr/share/arduino/hardware/arduino/cores/arduino/WProgram.h"

#include "TimerPolling.h"

TimerPolling Timer;


TimerPolling::TimerPolling() 
{
	i_timer = 0;
	
	for(int i=0; i<MAX_TIMER; i++) 
	{
		timers[i].active = 0;
	}
}

timer_h TimerPolling::once(long interval, callback_h callback) 
{
	if(i_timer == MAX_TIMER) return NULL_TIMER;
	
	int i;
	
	for(i=0; i<MAX_TIMER; i++) 
	{
		if(timers[i].active == 0) break;
	}
	
	timers[i].active = 1;
	timers[i].enabled = 1;
	timers[i].interval = interval;
	timers[i].lastshot = millis();
	timers[i].shots = 1;
	timers[i].callback = callback;
	
	return i;
}

void TimerPolling::repeat(long interval, callback_h callback, timer_h timer) 
{
	timers[timer].active = 1;
	timers[timer].enabled = 1;
	timers[timer].interval = interval;
	timers[timer].lastshot = millis();
	timers[timer].shots = -1;
	timers[timer].callback = callback;
}

timer_h TimerPolling::repeat(long interval, callback_h callback) 
{
	if(i_timer == MAX_TIMER) return NULL_TIMER;
	
	timer_h i;
	
	for(i=0; i<MAX_TIMER; i++) 
	{
		if(timers[i].active == 0) break;
	}
	
	repeat(interval,callback,i);
	
	return i;
}

void TimerPolling::repeat(long interval, long offset, callback_h callback, timer_h timer) 
{
	repeat(interval, callback, timer);
	timers[timer].lastshot = millis()+offset;
}

timer_h TimerPolling::repeat(long interval, long offset, callback_h callback) 
{
	int i = repeat(interval, callback);
	timers[i].lastshot = millis()+offset;
	return i;
}


void TimerPolling::trigger(timer_h timer) 
{
	timers[timer].enabled = 2;
}

void TimerPolling::changeSchedule(timer_h timer, long interval) 
{
	timers[timer].interval = interval;
}


void TimerPolling::enable(timer_h timer) 
{
	timers[timer].enabled = 1;
}

void TimerPolling::disable(timer_h timer) 
{
	timers[timer].enabled = 0;
}

void TimerPolling::enableAll() 
{
	for(int i=0; i<MAX_TIMER; i++) 
	{
		timers[i].enabled = 1;
	}
}

void TimerPolling::disableAll() 
{
	for(int i=0; i<MAX_TIMER; i++) 
	{
		timers[i].enabled = 0;
	}
}


void TimerPolling::destroy(timer_h timer) 
{
	timers[timer].active = 0;
}

void TimerPolling::tick() 
{
	for(int i=0; i<MAX_TIMER; i++) 
	{
		if(timers[i].active == 1 &&  timers[i].enabled > 0 ) 
		{		
			if(timers[i].enabled == 2)
			{
				timers[i].lastshot = millis();
				timers[i].callback();
				timers[i].enabled == 1;
			}
			else if((timers[i].interval <= (millis() - timers[i].lastshot))) 
			{
				timers[i].lastshot = millis();
				timers[i].callback();
				if(timers[i].enabled == 2)
				{
					timers[i].enabled == 1;
				}
								
				if(timers[i].shots > 0) 
				{
					timers[i].shots--;
					
					if(timers[i].shots == 0) 
					{
						timers[i].enabled = 0;
						timers[i].active = 0;
					}
				}
			}
		}
	}
}

