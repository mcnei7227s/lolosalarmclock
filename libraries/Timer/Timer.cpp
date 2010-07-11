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


//#include <avr/io.h>
//#include <avr/interrupt.h>
#include "/usr/share/arduino/hardware/arduino/cores/arduino/WProgram.h"

#include "Timer.h"

static int ticks;

Timer RealTimer;

ISR(TIMER2_OVF_vect) {
	if(ticks == INTERRUPT_INTERVAL) {
		RealTimer.tick();
		ticks = 0;
	}
	else {
		ticks++;
	}
};

Timer::Timer() {
	i_timer = 0;
	
	for(int i=0; i<MAX_TIMER; i++) {
		timers[i].active = 0;
	}
	
	// Initialise Timer 2 characteristics:
	//  - disable PWM (TCCR2A)
	//  - /1024 prescaler
	//Timer2 Settings: Timer Prescaler /256, WGM mode 0
	TCCR2A = 0;
	TCCR2B = 1<<CS22 | 1<<CS21;

	// Initialise Timer 2 Overflow Interrupt
	TIMSK2 = 1<<TOIE2;

	// Initialise Timer 2:
	TCNT2 = 0;
}

timer_h Timer::once(int interval, callback_h callback) {
	if(i_timer == MAX_TIMER) return NULL_TIMER;
	
	int i;
	
	for(i=0; i<MAX_TIMER; i++) {
		if(timers[i].active == 0) break;
	}
	
	timers[i].active = 1;
	timers[i].enabled = 1;
	timers[i].interval = interval;
	timers[i].ticks = 0;
	timers[i].shots = 1;
	timers[i].callback = callback;
	
	return i;
}

timer_h Timer::repeat(int interval, callback_h callback) {
	if(i_timer == MAX_TIMER) return NULL_TIMER;
	
	int i;
	
	for(i=0; i<MAX_TIMER; i++) {
		if(timers[i].active == 0) break;
	}
	
	timers[i].active = 1;
	timers[i].enabled = 1;
	timers[i].interval = interval;
	timers[i].ticks = 0;
	timers[i].shots = 0;
	timers[i].callback = callback;
	
	return i;
}

timer_h Timer::repeat(int interval, int offset, callback_h callback) 
{
	int i=repeat(interval, callback);
	timers[i].ticks = offset;
	return i;
}


void Timer::trigger(timer_h timer) 
{
	timers[timer].enabled = 1;
	timers[timer].ticks = timers[timer].interval;
}

void Timer::enable(timer_h timer) {
	timers[timer].enabled = 1;
}

void Timer::disable(timer_h timer) {
	timers[timer].enabled = 0;
}

void Timer::enableAll() 
{
	for(int i=0; i<MAX_TIMER; i++) 
	{
		timers[i].enabled = 1;
	}
}

void Timer::disableAll() 
{
	for(int i=0; i<MAX_TIMER; i++) 
	{
		timers[i].enabled = 0;
	}
}


void Timer::destroy(timer_h timer) {
	timers[timer].active = 0;
}

void Timer::tick() {
	for(int i=0; i<MAX_TIMER; i++) 
	{
		timers[i].ticks++;
		if(timers[i].active == 1 && timers[i].enabled == 1) 
		{		
			
			if(timers[i].interval <= timers[i].ticks) 
			{
				timers[i].callback();
				timers[i].ticks = 0;
				
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

