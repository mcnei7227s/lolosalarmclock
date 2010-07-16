/*
SerialLCD.cpp - 
*/

// 
// Includes
// 
#include "SerialLCD.h"


//
// Statics
//


//
// Private methods
//


//
// Constructor
//
SerialLCD::SerialLCD(int pinLCD) : NewSoftSerial(0,pinLCD)
{
  pinMode(pinLCD, OUTPUT);
  //serial = NewSoftSerial(0, pinLCD);
  //serial.begin(9600);
  begin(9600);
  intlevelold = 100;
}

//
// Destructor
//
SerialLCD::~SerialLCD()
{
	end();
}


//
// Public methods
//

// Initiates a function command to the display
void SerialLCD::serCommand() 
{
  print(0xFE, BYTE);
}

// Initiates a backlight command to the display
void SerialLCD::backLightCommand() 
{
  print(0x7C, BYTE);
}

// Turns the backlight on
void SerialLCD::backlightOn() 
{
  backLightCommand();
  print(157, BYTE);
}
 
// Turns the backlight off
void SerialLCD::backlightOff() 
{
  backLightCommand();
  print(128, BYTE);
}


void SerialLCD::backLight(float floatlevel)
{
	if(floatlevel<0) floatlevel=0;
	else if(floatlevel>=1) floatlevel=1;
  int intlevel = (int)(floatlevel*29);
  if(abs(intlevelold-intlevel)>=2)
  {
    SerialLCD::backLightCommand();
    print((char)intlevel, BYTE);    
    SerialLCD::intlevelold = intlevel;
  }
};

// Resets the display
void SerialLCD::clear() 
{
  serCommand();
  print(0x01, BYTE);
} 

void SerialLCD::clearLineOne() 
{
  goToLineOne();
  print("                ");
  goToLineOne();
} 

void SerialLCD::clearLineTwo() 
{
  goToLineTwo();
  print("                ");
  goToLineTwo();  
} 

// Sets the cursor to the given position
// line 1: 0-15, line 2: 16-31, 31+ defaults back to 0
void SerialLCD::goTo(int position) 
{
  if (position < 16) 
  {
    SerialLCD::serCommand(); //command flag
    SerialLCD::print((position+128), BYTE);
  } 
  else if (position < 32) 
  {
    SerialLCD::serCommand(); //command flag
    SerialLCD::print((position+48+128), BYTE);
  } 
  else 
  {
    goTo(0);
  }
}

void SerialLCD::goToLineOne()
{  
	 //puts the cursor at line 0 char 0.
   serCommand();   //command flag
   print(128, BYTE);    //position
}

void SerialLCD::goToLineTwo()
{  //puts the cursor at line 0 char 0.
   serCommand();   //command flag
   print(192, BYTE);    //position
}

void SerialLCD::spaces(int nb) 
{
	for(int i=0; i<nb; i++)
	{
   SerialLCD::print(" ");
	}
}

void SerialLCD::moveLeft(int steps) 
{
	for(int i=0; i<steps; i++)
	{
   SerialLCD::serCommand(); //command flag
   SerialLCD::print(0x10, BYTE);
	}
}

void SerialLCD::moveRight(int steps) 
{
	for(int i=0; i<steps; i++)
	{
   SerialLCD::serCommand(); //command flag
   SerialLCD::print(0x14, BYTE);
	}
}

void SerialLCD::blink() 
{
  serCommand();
  print(0x0D, BYTE);
} 

void SerialLCD::noblink() 
{
  serCommand();
  print(0x0C, BYTE);
} 

void SerialLCD::underline() 
{
  serCommand();
  print(0x0E, BYTE);
} 

void SerialLCD::nounderline() 
{
  serCommand();
  print(0x0C, BYTE);
} 

void SerialLCD::sright() 
{
  serCommand();
  print(0x1C, BYTE);
} 

void SerialLCD::sleft() 
{
  serCommand();
  print(0x18, BYTE);
} 

void SerialLCD::setSplash() 
{ 
	backLightCommand();
	print(10, BYTE); 
} 

// Reset LCD when everything else is kapput...
void SerialLCD::reset() 
{
	while(1)
	{
		print(0x07, BYTE); 
		print(18, BYTE); 
		delay(100);
		SerialLCD::print("test "); 
  }
}


 
 






