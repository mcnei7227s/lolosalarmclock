#include <AirSerial.h>

#define baudrate 2400

int ledPin =  13;    // LED connected to digital pin 13
AirSerial airserial = AirSerial(&Serial3);
long timelastmessage;

void setup()  
{
  Serial.begin(115200);
  Serial.println("BaseStation Listening");
  pinMode(ledPin, OUTPUT);    
  airserial.begin(baudrate);
}

void cleandamaged(char* buffer)
{
  int firstnull=BUFFERSIZE-1;
  for(int i=0; i<BUFFERSIZE; i++)
  {
    if(buffer[i]=='\n')
    {
      buffer[i]=' ';
    }
    else if(buffer[i]==0 && i<firstnull)
    {
      firstnull=i;
    }    
  }
  
  buffer[firstnull-1]='\n';
  buffer[firstnull]=0;
  
}

void loop()                     // run over and over again
{

  
  airserial.receive();
  
  if(airserial.packetavailable())
  {
    //Serial.println("packet received:");
    if(!airserial.packetok()) 
    {
      Serial.print("D!");    
      digitalWrite(ledPin, LOW);// set the LED off
      cleandamaged(airserial.getpacket());
    }
    else if(airserial.packetoverflow()) 
    {
      Serial.print("Of");
      digitalWrite(ledPin, LOW); // set the LED off
    }
    else 
    {
      Serial.print("Ok");
      digitalWrite(ledPin, HIGH); // set the LED on
      timelastmessage = millis();
    }  
    
    //if(!airserial.packetok()) Serial.println("PACKET DAMAGED!");
    //if(airserial.packetoverflow()) Serial.println("PACKET OVERFLOW!");
    //airserial.trimchecksum();
    Serial.print(airserial.getpacket());    
    airserial.clearpacket();    
    
  }
  else if((millis()-timelastmessage)>10*1000)
  {
     digitalWrite(ledPin, LOW);
  }
  else if((millis()-timelastmessage)>60*1000)
  {
     digitalWrite(ledPin, HIGH);
     delay(200);
     digitalWrite(ledPin, LOW);
     delay(200);
  }
  
  //Serial.println(airserial.packetlength());
  
  
  //SerialReceiver.print("hello");
  //SerialReceiver.print(i);
//  if (Serial3.available()) 
//  {
//      char receivedchar = (char)Serial3.read();
//      if(receivedchar=='2' && message==0) message=1;
//      else if(receivedchar=='5' && message==1) message=2;
//      else if(receivedchar=='5' && message==2) message=3;
//      else if(receivedchar=='\t' && message==3) message=4;
//      else if(message==4) message=255;
//      
//      if (message==255);
//      Serial.print(receivedchar);
//      if(receivedchar=='\n') message=0;
//  }
  //Serial.println(".");
  
  
}
