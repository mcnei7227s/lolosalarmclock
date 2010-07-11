/*
AirSerial.h
*/

#ifndef AirSerial_h
#define AirSerial_h

#include <inttypes.h>
#include "Print.h"
#include "binary.h"
#include "HardwareSerial.h"
//#include "/usr/share/arduino/hardware/arduino/cores/arduino/HardwareSerial.h"


/******************************************************************************
* Definitions
******************************************************************************/

#define VERSION		1
// RX buffer size:
#define BUFFERSIZE 	92 


class AirSerial : public Print
{
private:

  // per object data - sending side:
  char check1,check2,check3,check4;

  // per object data - receiving side:
  HardwareSerial* serial;
  long speed;
  char state;
  char buffer[BUFFERSIZE]; 
  int nextbyte;
  bool available;
  bool overflow;
  
  

  // private methods

public:
  // public methods
  AirSerial(HardwareSerial* _serial);
  ~AirSerial();
  void begin(long speed);
  void end();
  
  //sending side:
  // all Print methods should work.
  void beginpacket();
  void endpacket();
  void checksum(char b);
  virtual void write(uint8_t b);
  
  //receiving side:
  void receive();
  bool packetavailable();
  bool packetoverflow();
  bool packetok();
  int  packetlength();
  
  char* getpacket();
  void clearpacket();

  static int library_version() { return VERSION; }
};

#endif
