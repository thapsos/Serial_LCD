// 
// μLCD-32PT(SGC) 3.2” Serial LCD Display Module
// Arduino & chipKIT Library
//
// Example - see README.txt
// © Rei VILO, 2010-2012
// CC = BY NC SA
// http://sites.google.com/site/vilorei/
// http://github.com/rei-vilo/Serial_LCD
//
//
// Based on
// 4D LABS PICASO-SGC Command Set
// Software Interface Specification
// Document Date: 1st March 2011 
// Document Revision: 6.0
// http://www.4d-Labs.com
//
//

#include "Serial_LCD.h"
#include "proxySerial.h"
#include "GUI.h"


// test release
#if GUI_RELEASE < 23
#error required GUI_RELEASE 23
#endif

// --- HardwareSerial Case - Arduino + chipKIT
ProxySerial mySerial(&Serial1);
Serial_LCD myLCD( &mySerial); 
button b7;

uint16_t x, y;
uint32_t l;

void setup() {
  Serial.begin(19200);
  Serial.print("\n\n\n***\n");

  // --- HardwareSerial Case - Arduino + chipKIT
  Serial1.begin(9600);
  Serial.print("HardwareSerial 9600 \n");

  myLCD.begin(4); // Arduino pin 4 connected to screen RESET

  delay(10);
  // === Serial port speed up
  myLCD.setSpeed(115200);
  Serial1.begin(115200);
  Serial.print("Serial 115200 \n");


  myLCD.setOrientation(0x03);
  myLCD.setPenSolid(true);
  myLCD.setFontSolid(true);
  myLCD.setFont(0);
  myLCD.gText( 0, 210, myLCD.WhoAmI());
  myLCD.setTouch(true);
  l=millis();

  b7.define(&myLCD,  0, 0, 79, 59, setItem(1, "Stop"), myLCD.setColour(0xff, 0xff, 0xff), myLCD.setColour(0xff, 0x00, 0x00), myLCD.setColour(0x88, 0x00, 0x00), 9);
  b7.enable(true);
  b7.draw();

  uint16_t chrono0, chrono1;
  chrono0=millis();

  myLCD.setPenSolid(false);
  for (int i=0; i<10; i++) {
    for (int j=0; j<10; j++) {
      myLCD.circle(120+j*10, 30+i*10, 30, random(0, 0xffff));
    }
  }

  chrono1=millis();
  myLCD.gText( 0, 180, ftoa((chrono1-chrono0), 0, 10 ));

  chrono0=millis();

  myLCD.setPenSolid(true);
  for (int i=0; i<10; i++) {
    for (int j=0; j<10; j++) {
      myLCD.circle(120+j*10, 30+i*10, 30, random(0, 0xffff));
    }
  }

  chrono1=millis();
  myLCD.gText( 160, 180, ftoa((chrono1-chrono0), 0, 10 ));

}

uint8_t c;

void loop() {

  c=myLCD.getTouchActivity();
  Serial.print(". ");

  if (c>0) {
    Serial.print("getTouchActivity ");
    myLCD.getTouchXY(x, y);
    Serial.print("getTouchXY ");
    myLCD.setFont(0);
    Serial.print("setFont ");
    myLCD.gText(200, 0, ftoa(x, 0, 5)); 
    Serial.print("gText x ");
    myLCD.gText(200, 15, ftoa(y, 0, 5)); 
    Serial.print("gText y ");

    // quit
    if (b7.check()) {
      Serial.print("QUIT ");
      myLCD.off();
      while(true);
    }
    Serial.print("\n");
  }
  myLCD.setFont(0);
  myLCD.setFontSolid(true);
  myLCD.gText( 250, 225, ftoa(millis()-l, 0, 6));
  l=millis();
}





















