/*
DS1307.h - 
*/

#ifndef DS1307_h
#define DS1307_h

#include <inttypes.h>
#include "/usr/share/arduino/libraries/Wire/Wire.h"
#include "/usr/share/arduino/hardware/arduino/cores/arduino/HardwareSerial.h"
/******************************************************************************
* Definitions
******************************************************************************/
#define DS1307_I2C_ADDRESS 0x68  // This is the I2C address of the RTC module

static inline char decToBcd(char val);
static inline char bcdToDec(char val);

extern char* monthsnames[12];

extern char* daysnames[7];

class DS1307
{
private:

public:
  // public methods
  DS1307();
  ~DS1307();
  
  void setDateTime(); 
	void getDateTime();
	
	void sendDateToSerial();
	
  // public data
  char second, minute, hour, dayOfWeek, dayOfMonth, month, year;
  
};

#endif
