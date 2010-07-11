/*
AirSerial.cpp 
*/

#include "AirSerial.h"

#define STARTSIGNALLENGTH	2
#define BEGINOFPACKET ((char)B10101010)
#define ENDOFPACKET ((char)B00000000)
// 
// Includes
// 


//
// Private methods
//





//
// Constructor
//
AirSerial::AirSerial(HardwareSerial* _serial) 
{
	serial = _serial;
	clearpacket();
}

//
// Destructor
//
AirSerial::~AirSerial()
{
  end();
}



//
// Public methods
//

void AirSerial::begin(long _speed)
{
  speed = _speed;
  serial->begin(speed);
  clearpacket();
}

void AirSerial::end()
{

}


void AirSerial::beginpacket()
{
	check1=1;
	check2=1;
	check3=1;
	check4=1;
	for(int i=0; i<=(speed/1200); i++)  
  {
    serial->print((char)B11111111);
    serial->print((char)B11110000);
    serial->print((char)B11100001);
    serial->print((char)B11000011);
    serial->print((char)B10000111);
    serial->print((char)B11001100);
    serial->print((char)B10011001);
    serial->print((char)B10101010);
  }/**/
  for(int i=0; i<STARTSIGNALLENGTH; i++)  
  {
		serial->print(BEGINOFPACKET);
	}
}

void AirSerial::endpacket()
{
	serial->print('$');
	serial->print(check1);
	serial->print(check2);
	serial->print(check3);
	serial->print(check4);
	serial->print(ENDOFPACKET);	
}

void AirSerial::checksum(char b)
{
	check1 ^= b+check4;
	if(check1==ENDOFPACKET) check1=1;
	check2 += check1^b;
	if(check2==ENDOFPACKET) check2=1;
	check3 = check3*b+check2;
	if(check3==ENDOFPACKET) check3=1;
	check4 = check4^(check3>>1);
	if(check4==ENDOFPACKET) check4=1;
}

void AirSerial::write(uint8_t b)
{
	checksum(b);
	serial->Print::print(b);
};


void AirSerial::receive()
{
	
	if (!available && serial->available()) 
  {	
			if(nextbyte+1<BUFFERSIZE)
			{
				char receivedchar = (char)serial->read();
				//Serial.print(receivedchar);
				//Serial.print(" s:");
				//Serial.println(state);
				if(receivedchar==BEGINOFPACKET && state!=32)
				{
					state++;
					//Serial.println((int)state);	
				}
				else
				{
					if(state>=2) 
					{
						//Serial.print("!!!!!!!!!!!!!!!!1");
						state = 32;
					}
					else if(state>0 && state!=32)
					{
						state=0;
					}
   				//Serial.println((int)state);	
				}
				
				if(state==32)
				{
					//Serial.print(receivedchar);
					buffer[nextbyte]=receivedchar;
					nextbyte++;
					if(receivedchar==ENDOFPACKET) 
					{
						available=true;
						overflow=false;
						buffer[nextbyte-6]=0x00;// hide checksum;
					}
				}
			}
      else
      {
				available=true;
				overflow=true;
				buffer[BUFFERSIZE-1]=0x00; //has to be a null terminated string...
			}
  }
}

bool AirSerial::packetavailable()
{
	return available;	
};

bool validpayload(unsigned char byte)
{
	//Serial.println((int)byte);
	return (byte==9)||(byte==13)||(byte==11)||(byte==10)||(byte>=32 && byte<=126);
}

bool AirSerial::packetok()
{
	for(int i=0; i<nextbyte-6; i++)
	{
		if(!validpayload(buffer[i]))
			return false;
	}
	
	check1=1;
	check2=1;
	check3=1;
	check4=1;
	//Serial.print("'");
	for(int i=0; i<nextbyte-6; i++)
	{
		//Serial.print(buffer[i]);
		checksum(buffer[i]);
	}
	//Serial.print("'");
	//Serial.println((int)xorcheck);
	//Serial.println((int)sumcheck);
	char packetcheck1 = buffer[nextbyte-5];
  char packetcheck2 = buffer[nextbyte-4];
  char packetcheck3 = buffer[nextbyte-3];
  char packetcheck4 = buffer[nextbyte-2];
	//Serial.println(packetxorcheck);
	//Serial.println(packetsumcheck);
	//Serial.println("packet checksum");
	//Serial.println((int)packetxorcheck);
  //Serial.println((int)packetsumcheck);
	return (packetcheck1==check1)&&(packetcheck2==check2)&&(packetcheck3==check3)&&(packetcheck4==check4); 
}

int  AirSerial::packetlength()
{
		return nextbyte;
}

bool AirSerial::packetoverflow()
{
	return overflow;
}

char* AirSerial::getpacket()
{
	return buffer;
}

void AirSerial::clearpacket()
{
	state=0;
	nextbyte=0;
	available=false;
	overflow=false;	
	check1=1;
	check2=1;
	check3=1;
	check4=1;
}



