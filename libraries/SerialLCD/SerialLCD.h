/*
SerialLCD.h - 
*/

#ifndef SerialLCD_h
#define SerialLCD_h

//#include <inttypes.h>
#include <NewSoftSerial.h>
#include "/usr/share/arduino/hardware/arduino/cores/arduino/HardwareSerial.h"
#include "/usr/share/arduino/hardware/arduino/cores/arduino/wiring.h"
/******************************************************************************
* Definitions
******************************************************************************/



class SerialLCD : public NewSoftSerial
{
public:
  // public methods
  SerialLCD(int pinLCD);
  ~SerialLCD();
	
	void serCommand();
		
	void backLightCommand();
	void backlightOn();
	void backlightOff();
	void backLight(float level);
	void clear();
	void clearLineOne();
	void clearLineTwo();
	void goTo(int position);
	void goToLineOne();
	void goToLineTwo();
	void spaces(int nb);
	void moveLeft(int steps);
	void moveRight(int steps); 
	
	void blink();
	void noblink();
	void underline();
	void nounderline();
	void sright();
	void sleft();

	void setSplash();
	void reset();

	int intlevelold;
};

#endif
