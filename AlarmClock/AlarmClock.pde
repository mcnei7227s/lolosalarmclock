


#include <AirSerial.h>
#include <TimerPolling.h>
#include <SHT1x.h>
#include <Wire.h>
#include <NewSoftSerial.h>
#include <CapSense.h>
#include <DS1307.h>
#include <SerialLCD.h>
#include <EEPROM.h>
#include <Tone.h>


/*
 Created by Loic Royer
 
 */

#define baudrate        1200

#define pinLCD          2
#define pinAccel        0
#define pinLightSensor  1
#define pinGasSensor    3
#define pinSpeak        3    
#define pinSHTdata      5
#define pinSHTclock     4

#include <avr/eeprom.h>
#define eeprom_read_to(dst_p, eeprom_field, dst_size) eeprom_read_block(dst_p, (void *)offsetof(config, eeprom_field), MIN(dst_size, sizeof((config*)0)->eeprom_field))
#define eeprom_read(dst, eeprom_field) eeprom_read_to(&dst, eeprom_field, sizeof(dst))
#define eeprom_write_from(src_p, eeprom_field, src_size) eeprom_write_block(src_p, (void *)offsetof(config, eeprom_field), MIN(src_size, sizeof((config*)0)->eeprom_field))
#define eeprom_write(src, eeprom_field) { typeof(src) x = src; eeprom_write_from(&x, eeprom_field, sizeof(x)); }
#define MIN(x,y) ( x > y ? y : x )


struct config
{
  int alarmthreshold;
  int alarmevents;
  int alarmduration;
  int alarmhourstop;
  int alarmminutestop;
} ;


int counter = 0;

float valAccel= 0;
float zeroAccel=0;
float energyAccel=0;
float maxenergyAccel=0;

int valLightSensorOld= 500;
int valLightSensor= 500; 
int valGasSensor= 0; 
int valGasSensorCal= 0; 
float valTemp=-100;
float valRelHum=-100;
float valDewPoint=-100;

int mainmode=0;
int displaymode;

int changedatetimemode;

int alarmthreshold;
int alarmevents;
int alarmduration;
int alarmhourstop;
int alarmminutestop;

// 8 9 10 11 12
//CapSense cap1  = CapSense(7, 8); 
CapSense capA  = CapSense(7, 12); 
CapSense capB  = CapSense(7, 10); 
CapSense capD  = CapSense(7, 9); 
CapSense capC  = CapSense(7, 8); 
//CapSense cap5  = CapSense(7, 12); /**/

byte calA =0;
byte calC =0;
byte calB =0;
byte calD =0;

float valCapA = 0;
float valCapC = 0;
float valCapB = 0;
float valCapD = 0;

long lastA =0;
long lastC =0;
long lastB =0;
long lastD =0;
long last =0;

AirSerial airserial = AirSerial(&Serial);
SerialLCD lcd = SerialLCD(pinLCD);
SHT1x sht1x(pinSHTdata, pinSHTclock);
DS1307 clock = DS1307();

Tone mytone;


int available()
{
    char stack = 1;
    extern char *__brkval;
    extern char *__malloc_heap_end;
    extern size_t __malloc_margin;
    if (__malloc_heap_end)
	  return __malloc_heap_end - __brkval;
    return (&stack - __brkval) - __malloc_margin;
} /**/


void setup() 
{
  eeprom_read(alarmthreshold, alarmthreshold);
  eeprom_read(alarmevents, alarmevents);
  eeprom_read(alarmduration, alarmduration);
  eeprom_read(alarmhourstop, alarmhourstop);
  eeprom_read(alarmminutestop, alarmminutestop);

  airserial.begin(baudrate);
  
  valAccel = analogRead(pinAccel);
  zeroAccel = valAccel;
  energyAccel=0;
  maxenergyAccel=0;

  pinMode(pinSpeak, OUTPUT);
  //digitalWrite(14+pinLightSensor, HIGH);
  //pinMode(pinAccel, OUTPUT);
  
  /*cap1.set_CS_AutocaL_Millis(0xFFFFFFFF);  
  cap2.set_CS_AutocaL_Millis(0xFFFFFFFF);  
  cap3.set_CS_AutocaL_Millis(0xFFFFFFFF);  
  cap4.set_CS_AutocaL_Millis(0xFFFFFFFF);  
  cap5.set_CS_AutocaL_Millis(0xFFFFFFFF); /**/ 
  mytone.begin(pinSpeak);
  
  

  
  //lcd.reset();
  lcd.clear();  
  datetimerefresh();
  temphumrefresh();
  gasrefresh();
  backlightrefresh();
  serialrefresh();
  lcdrefreshtime();
  lcdrefreshinfo();  

  Timer.repeat(1*secondsU,datetimerefresh,0);
  Timer.repeat(1*secondsU,lcdrefreshtime,1);
  Timer.repeat(1*hoursU,lcdrefreshinfo,2);
  Timer.repeat(30*secondsU,0,  temphumrefresh,3);
  Timer.repeat(1*secondsU,15, gasrefresh,4);
  Timer.repeat(0.1*secondsU, backlightsensingrefresh,5);
  Timer.repeat(10*minutesU,10*secondsU, backlightrefresh,6);
  Timer.repeat(0.001*secondsU,500, accelsensingrefresh,7);
  Timer.repeat(1*secondsU,500, accelrefresh,8);
  Timer.repeat(4*secondsU, serialrefresh,9);
  
  Timer.repeat(1*secondsU, alarm,10);
  
  //callibrate touch sensor:
  for(int i=0; i<100; i++)
  {
    calibratetouch();
  }
  /*
  lcd.clear();
  lcd.clearLineOne();
  lcd.clearLineTwo();  
  lcd.goToLineOne();
  lcd.print("Lolo's");
  lcd.goToLineTwo();
  lcd.print("Alarm Clock");
  lcd.setSplash();
  delay(1000);
  lcd.clear();
  delay(1000);/**/
  
}

void calibratetouch()
{
    calC = max(calC,capC.capSenseRaw(1));
    calA = max(calA,capA.capSenseRaw(1)); //
    calB = max(calB,capB.capSenseRaw(1));
    calC = max(calC,capC.capSenseRaw(1));
    calA = max(calA,capA.capSenseRaw(1)); //
    calB = max(calB,capB.capSenseRaw(1));
}

void inline lcdrefreshinfo(void) 
{
  lcd.noblink();
  lcd.goToLineTwo();
  //Serial.println((int)displaymode);
  if(mainmode==0)
  {
    if(displaymode == 0)
    {
      lcd.print(daysnames[(int)clock.dayOfWeek-1]);
      lcd.print(" ");
      lcd.print(monthsnames[(int)clock.month-1]); 
      lcd.print(" ");
      if((int)clock.dayOfMonth<10) lcd.print(" ");
      lcd.print((int)clock.dayOfMonth);
      lcd.print("  20");
      lcd.print((int)clock.year); 
      Timer.changeSchedule(2,1*hoursU);
    }
    
    else if(displaymode == 1)
    {
      if(valTemp>=0 && valTemp<10) lcd.print(" ");
      lcd.print(valTemp);
      lcd.print(B11011111,BYTE);
      lcd.print("  ");
      if(valRelHum<10) lcd.print(" ");
      lcd.print(valRelHum);
      lcd.print("%rH");
      Timer.changeSchedule(2,15*secondsU);
    }
    else if(displaymode == 2)
    {
      if(valDewPoint>=0 && valDewPoint<10) lcd.print(" ");
      lcd.print(valDewPoint);
      lcd.print(B11011111,BYTE);
      lcd.print("dp ");
      if(valGasSensorCal<100) lcd.print(" ");
      if(valGasSensorCal<10) lcd.print(" ");
      lcd.print(valGasSensorCal);
      lcd.print("%");
      lcd.print(B00101110,BYTE);
      lcd.print("aq");
      Timer.changeSchedule(2,1*secondsU);
    }
    else if(displaymode == 3)
    {
      if(energyAccel<100) lcd.print(" ");
      if(energyAccel<10) lcd.print(" ");
      lcd.print((int)energyAccel);
      lcd.print("%");
      lcd.print(B00101110,BYTE);
      lcd.print("ma  ");
      if(valLightSensor<100) lcd.print(" ");
      if(valLightSensor<10) lcd.print(" ");
      lcd.print(valLightSensor);
      lcd.print("%");
      lcd.print(B00101110,BYTE);
      lcd.print("ll");
      Timer.changeSchedule(2,0.5*secondsU);
    }
    else if(displaymode == 4)
    {
      lcd.print("Air(raw): ");
      if(valGasSensor<100) lcd.print(" ");
      if(valGasSensor<10) lcd.print(" ");
      lcd.print(valGasSensor);
      lcd.print("   ");
      Timer.changeSchedule(2,1*secondsU);
    }
  }
  else if(mainmode == 1)
  {
    lcd.clearLineTwo();
    lcd.spaces(5);
    lcd.print(B01011110,BYTE);
  }
  else if(mainmode == 2)
  {
    lcd.clearLineTwo();
    lcd.spaces(8);
    lcd.print(B01011110,BYTE);
  }
  else if(mainmode == 3)
  {
    lcd.clearLineTwo();
    lcd.print("  [");
    if(alarmhourstop<10) lcd.print(" ");
    lcd.print(alarmhourstop);
    lcd.print("]:");
    if(alarmminutestop<10) lcd.print("0");
    lcd.print(alarmminutestop);

  }
  else if(mainmode == 4)
  {
    lcd.clearLineTwo();
    lcd.print("    ");
    if(alarmhourstop<10) lcd.print(" ");
    lcd.print(alarmhourstop);
    lcd.print(":[");
    if(alarmminutestop<10) lcd.print("0");
    lcd.print(alarmminutestop);
    lcd.print("]");
  }
  else if(mainmode == 5 || mainmode == 6 || mainmode == 7)
  {
    lcd.clearLineTwo();
    lcd.print("    ");
    if(alarmhourstop<10) lcd.print(" ");
    lcd.print(alarmhourstop);
    lcd.print(":");
    if(alarmminutestop<10) lcd.print("0");
    lcd.print(alarmminutestop);
    if(mainmode == 5)
    {
      lcd.print(" d=[");
      lcd.print(alarmduration);
    }
    else if(mainmode == 6)
    {
      lcd.print(" e=[");
      lcd.print(alarmevents);
    }
    else if(mainmode == 7)
    {
      lcd.print(" t=[");
      lcd.print(alarmthreshold);
    }
    lcd.print("]");
  }
  

  
}

void inline lcdrefreshtime(void) 
{
  lcd.goToLineOne();
  lcd.print("    ");
  if((int)clock.hour<10) lcd.print(" ");
  lcd.print((int)clock.hour); 
  lcd.print(":");
  if((int)clock.minute<10) lcd.print("0");
  lcd.print((int)clock.minute);
  lcd.print(":");
  if((int)clock.second<10) lcd.print("0");
  lcd.print((int)clock.second);
  //lcd.print("  ");
  //lcd.print((int)energyAccel);
  //lcd.print(" ");
  //lcd.print((int)counter);
  //lcd.print(" ");  
  
  //lcd.print(available()); 
}

void inline datetimerefresh(void) 
{
  clock.getDateTime();
}

void inline temphumrefresh(void) 
{
  sht1x.update();
  valTemp   = sht1x.getTemperatureC();
  valRelHum = sht1x.getHumidity();
  valDewPoint = sht1x.getDewPoint();
  
}

void inline gasrefresh(void) 
{
  valGasSensor = analogRead(pinGasSensor); 
  valGasSensorCal = valGasSensor-430+60+(valDewPoint*100)/16;
  //valGasSensor-430+(valDewPoint*100)/16;
}

void inline backlightsensingrefresh(void) 
{
  valLightSensor = (15*valLightSensor + analogRead(pinLightSensor))/16;
}

void inline backlightrefresh(void) 
{
  lcd.backLight(((float)valLightSensor)/1024); //
}

void inline serialrefresh(void) 
{
  airserial.beginpacket();
  airserial.print("\t");
  airserial.print((long)available());
  airserial.print("\t");
  long hundredsofhour = (long)(millis()/36000);
  airserial.print((long)(hundredsofhour)); 
  airserial.print("\t");
  airserial.print((int)clock.month);
  if((int)clock.dayOfMonth<10) airserial.print("0");
  airserial.print((int)clock.dayOfMonth);
  airserial.print("\t");
  airserial.print((int)clock.dayOfWeek); 
  airserial.print("\t");
  airserial.print((int)clock.hour); 
  airserial.print("\t");
  airserial.print((int)clock.minute);
  airserial.print("\t");
  airserial.print((int)clock.second);
  airserial.print("\t"); 
  airserial.print((int)energyAccel); 
  airserial.print("\t"); 
  airserial.print(valLightSensor);
  airserial.print("\t");  
  airserial.print(valGasSensor); 
  airserial.print("\t");  
  airserial.print(valTemp); 
  airserial.print("\t");  
  airserial.print(valRelHum); 
  airserial.print("\t");  
  airserial.print(valDewPoint); 
  airserial.print("\t");  
  airserial.print(valGasSensorCal); 
  /*Serial.print("\t");  
  Serial.print(valCapA);
  Serial.print("\t");  
  Serial.print(valCapB); 
  Serial.print("\t");  
  Serial.print(valCapC);/**/
  airserial.print("\n");  
  airserial.endpacket();
  
  //mytone.play(NOTE_A4+4*energyAccel,10);
}

byte inline touchdetector(CapSense cap, byte cal, float &valCap, long &last, long timming, float intensity)
{
  long time = millis();
  valCap += (float)(cap.capSenseRaw(1)>cal ? 1 : 0 );
  if(valCap>intensity && (time-last)>timming)
  { 
    valCap=0;
    last = time;
    return 1;
  }
  else
  {
    valCap *= 0.95;
    return 0;
  }
}

//////////////////
byte alarmlock = 0;

void eventA()
{
  if(!mytone.isPlaying()) mytone.playSync(NOTE_A6,10);
  if(mainmode==0)
  {
    displaymode = (displaymode-1+5) % 5;
    lcdrefreshinfo(); 
  }
  else if(mainmode==1)
  {
    clock.hour = (clock.hour-1+24)%24;
    clock.setDateTime();
    lcdrefreshtime();
  }
  else if(mainmode==2)
  {
    clock.minute = (clock.minute-1+60)%60;
    clock.setDateTime();
    lcdrefreshtime();
  }
  else if(mainmode==3)
  {
    alarmhourstop = (alarmhourstop-1+24)%24;
    lcdrefreshinfo();
  }
  else if(mainmode==4)
  {
    alarmminutestop= (alarmminutestop-1+60)%60;
    lcdrefreshinfo();
  }
  else if(mainmode==5)
  {
    alarmduration = (alarmduration-1+180)%180;
    lcdrefreshinfo();
  }
  else if(mainmode==6)
  {
    alarmevents = (alarmevents-1+255)%255;
    lcdrefreshinfo();
  }
  else if(mainmode==7)
  {
    alarmthreshold = (alarmthreshold-1+100)%100;
    lcdrefreshinfo();
  }
  
  alarmlock = 0;
}

void eventC()
{
  if(!mytone.isPlaying()) mytone.playSync(NOTE_A7,10);
  if(mainmode==0)
  {
    displaymode = (displaymode+1) % 5;
    lcdrefreshinfo(); 
  }
  else if(mainmode==1)
  {
    clock.hour = (clock.hour+1)%24;
    clock.setDateTime();
    lcdrefreshtime();
  }
  else if(mainmode==2)
  {
    clock.minute = (clock.minute+1)%60;
    clock.setDateTime();
    lcdrefreshtime();
  }
  else if(mainmode==3)
  {
    alarmhourstop = (alarmhourstop+1)%24;
    lcdrefreshinfo();
  }
  else if(mainmode==4)
  {
    alarmminutestop = (alarmminutestop+1)%60;
    lcdrefreshinfo();
  }
  else if(mainmode==5)
  {
    alarmduration = (alarmduration+1)%180;
    lcdrefreshinfo();
  }
  else if(mainmode==6)
  {
    alarmevents = (alarmevents+1)%255;
    lcdrefreshinfo();
  }
  else if(mainmode==7)
  {
    alarmthreshold = (alarmthreshold+1)%100;
    lcdrefreshinfo();
  }

  alarmlock = 0;
}

void eventB()
{
  if(!mytone.isPlaying()) mytone.playSync(NOTE_A4,10);
  mainmode = (mainmode+1)%8;      
  if(mainmode==0)
  {
    eeprom_write(alarmthreshold, alarmthreshold);
    eeprom_write(alarmevents, alarmevents);
    eeprom_write(alarmduration, alarmduration);
    eeprom_write(alarmhourstop, alarmhourstop);
    eeprom_write(alarmminutestop, alarmminutestop);
  }
  lcdrefreshinfo(); 
}





void inline touchsensorrefresh(void)
{
  
  //lcd.goTo(16-3);
  /*if(millis()<10000) 
  {
    calibratetouch();
  }/**/

  {
    byte buttonA = touchdetector(capA,calA,valCapA,last,600,10);
    byte buttonC = touchdetector(capC,calC,valCapC,last,600,10);  
    if(buttonA && !buttonC)
    {
      eventA();
      valCapA=0;
      valCapB=0;
      valCapC=0;
    }
        
    if(buttonC && !buttonA)
    {
      eventC();      
      valCapA=0;
      valCapB=0;
      valCapC=0;
    }
    
    if(buttonA && buttonC)
    {
      eventB();
      valCapA=0;
      valCapB=0;
      valCapC=0;
    }
    
    byte buttonB = touchdetector(capB,calB,valCapB,last,600,10);
    if(buttonB && !buttonA && !buttonC)
    {
      eventB();
      valCapA=0;
      valCapB=0;
      valCapC=0;
    }/**/
  }
    
 //serialrefresh();
}



void inline accelsensingrefresh(void)
{
  valAccel = analogRead(pinAccel);
  maxenergyAccel = max(maxenergyAccel,abs(valAccel-zeroAccel));
  if(maxenergyAccel<8)
  {
    zeroAccel = 0.999*zeroAccel + 0.001*valAccel;
  }
}

void inline accelrefresh(void)
{
  energyAccel = maxenergyAccel;
  maxenergyAccel=0;
}






boolean inline startalarmtime(void)
{
  int alarmminutestart = (alarmminutestop+60-alarmduration)%60;
  int alarmhourstart = ((int)((float)alarmhourstop+((float)alarmminutestop/60)+24-((float)alarmduration/60)))%24;
  
 return  ((int)clock.hour==alarmhourstart) 
      && ((int)clock.minute==alarmminutestart);
}


boolean inline stopalarmtime(void)
{
 return  ((int)clock.hour==alarmhourstop) 
      && ((int)clock.minute==alarmminutestop);
}
  

int alarmfreq = 5;
void inline alarm(void)
{
  if((startalarmtime() || true) && alarmlock==0)
  {
    alarmlock=1;
  }
  
  if((energyAccel>alarmthreshold || stopalarmtime()) && alarmlock==1)
  {
    alarmlock++;
    mytone.playSync(NOTE_B0,10);
  }  
  
  #define durationnote random(10,20)
  #define freqvariation random(0,500)
  if(alarmlock>=alarmevents)
  {
    mytone.playSync(NOTE_A5+freqvariation,durationnote);
    mytone.playSync(NOTE_B6+freqvariation,durationnote);
    mytone.playSync(NOTE_A6+freqvariation,durationnote);
    mytone.playSync(NOTE_B6+freqvariation,durationnote);
    mytone.playSync(NOTE_C7+freqvariation,durationnote);
    mytone.playSync(NOTE_B7+freqvariation,durationnote);
    mytone.playSync(NOTE_C7+freqvariation,durationnote);
    mytone.playSync(NOTE_A7+freqvariation,durationnote);
    mytone.playSync(NOTE_A7+freqvariation,durationnote);
    mytone.playSync(NOTE_B7+freqvariation,durationnote);
    mytone.playSync(NOTE_C8+freqvariation,durationnote);
    mytone.playSync(NOTE_B8+freqvariation,durationnote);
    mytone.playSync(NOTE_C8+freqvariation,durationnote);
    mytone.playSync(NOTE_A8+freqvariation,durationnote);
    mytone.playSync(NOTE_A8+freqvariation,durationnote);
    mytone.playSync(NOTE_B8+freqvariation,durationnote);
    mytone.playSync(NOTE_D8+freqvariation,durationnote);
    mytone.playSync(NOTE_B8+freqvariation,durationnote);
    mytone.playSync(NOTE_A8+freqvariation,durationnote);
    mytone.playSync(NOTE_D7+freqvariation,durationnote);
    mytone.playSync(NOTE_A8+freqvariation,durationnote);
    mytone.playSync(NOTE_B7+freqvariation,durationnote);
    mytone.playSync(NOTE_D8+freqvariation,durationnote);

  }  
}


void loop() 
{
  touchsensorrefresh();
  Timer.tick();
  counter++;
}

