/*
DS1307.cpp - 
*/

// 
// Includes
// 
#include "DS1307.h"
#include "/usr/share/arduino/libraries/Wire/Wire.h"


//
// Statics
//

char* monthsnames[]={"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

char* daysnames[]={"Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun"};


// Convert normal decimal numbers to binary coded decimal
static inline char decToBcd(char val)
{
  return ( (val/10*16) + (val%10) );
}

// Convert binary coded decimal to normal decimal numbers
static inline char bcdToDec(char val)
{
  return ( (val/16*10) + (val%16) );
}

//
// Private methods
//


//
// Constructor
//
DS1307::DS1307() 
{
		Wire.begin();
		//getDateTime();
}

//
// Destructor
//
DS1307::~DS1307()
{
		
}


//
// Public methods
//

// 1) Sets the date and time on the ds1307
// 2) Starts the clock
// 3) Sets hour mode to 24 hour clock
// Assumes you're passing in valid numbers, Probably need to put in checks for valid numbers.
 
void DS1307::setDateTime()                
{
   Wire.beginTransmission(DS1307_I2C_ADDRESS);
   Wire.send(0x00);
   Wire.send(decToBcd(DS1307::second));    // 0 to bit 7 starts the clock
   Wire.send(decToBcd(DS1307::minute));
   Wire.send(decToBcd(DS1307::hour));      // If you want 12 hour am/pm you need to set
                                   // bit 6 (also need to change readDateDs1307)
   Wire.send(decToBcd(DS1307::dayOfWeek));
   Wire.send(decToBcd(DS1307::dayOfMonth));
   Wire.send(decToBcd(DS1307::month));
   Wire.send(decToBcd(DS1307::year));
   Wire.endTransmission();
}

// Gets the date and time from the ds1307 and prints result
void DS1307::getDateTime()
{
  // Reset the register pointer
  Wire.beginTransmission(DS1307_I2C_ADDRESS);
  Wire.send(0x00);
  Wire.endTransmission();

  Wire.requestFrom(DS1307_I2C_ADDRESS, 7);

  // A few of these need masks because certain bits are control bits
  DS1307::second     = bcdToDec(Wire.receive() & 0x7f);
  DS1307::minute     = bcdToDec(Wire.receive());
  DS1307::hour       = bcdToDec(Wire.receive() & 0x3f);  // Need to change this if 12 hour am/pm
  DS1307::dayOfWeek  = bcdToDec(Wire.receive());
  DS1307::dayOfMonth = bcdToDec(Wire.receive());
  DS1307::month      = bcdToDec(Wire.receive());
  DS1307::year       = bcdToDec(Wire.receive());
  
}

void DS1307::sendDateToSerial()
{
  Serial.print((int)DS1307::year); 
  Serial.print(":");
  Serial.print((int)DS1307::month); 
  Serial.print(":");
  Serial.print((int)DS1307::dayOfMonth); 
  Serial.print(":");
  Serial.print((int)DS1307::dayOfWeek); 
  Serial.print(":");
  Serial.print((int)DS1307::hour); 
  Serial.print(":");
  Serial.print((int)DS1307::minute);
  Serial.print(":");
  Serial.print((int)DS1307::second);
  Serial.println("");
}



