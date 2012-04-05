// 
// μLCD-32PT(SGC) 3.2” Serial LCD Display Module
// Arduino & chipKIT Library
//
// Example - see README.txt
// Jan 25, 2012 release 29 - see README.txt
//
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

#include "WProgram.h"
#include <Wire.h>

#include "Serial_LCD.h"
#include "GUI.h"

// test release
#if GUI_RELEASE < 29
#error required GUI_RELEASE 29
#endif

// Arduino Case : uncomment #include
// #if defined(__AVR__) || defined (__AVR_ATmega328P__) works!
// ---
//#include "NewSoftSerial.h"
// ===

#include "proxySerial.h"

#if defined(__AVR__)  || defined (__AVR_ATmega328P__)  | defined (__AVR_ATmega328P__)
// Arduino Case ---
#include "NewSoftSerial.h"
NewSoftSerial nss(2, 3); // RX, TX
ProxySerial mySerial(&nss);

#elif defined(__PIC32MX__) 
// chipKIT Case ---
ProxySerial mySerial(&Serial1);

#else
#error Non defined board
#endif 


Serial_LCD myLCD( &mySerial); 



uint16_t x, y;
uint32_t l;

button b1, b2, b7;



void setup() {
  Serial.begin(19200);
  Serial.print("\n\n\n***\n");

#if defined(__AVR__)  || defined (__AVR_ATmega328P__) | defined (__AVR_ATmega328P__)
  Serial.print("avr\t");
  Serial.print(__AVR__);
  Serial.print("\n");
  nss.begin(9600);
#elif defined(__PIC32MX__) 
  Serial.print("chipKIT\t");
  Serial.print(__PIC32MX__);
  Serial.print("\n");
  Serial1.begin(9600);
#endif 

  myLCD.begin();
  myLCD.setOrientation(0x03);

  Wire.begin();

  myLCD.setPenSolid(true);
  myLCD.setFontSolid(true);

  myLCD.setFont(0);
  myLCD.gText( 0, 210, 0xffff, myLCD.WhoAmI());

  myLCD.setTouch(true);

  l=millis();

  uint16_t i=9;
  b1.dDefine(&myLCD,  0, myLCD.maxY()-60, 79, 59, setItem(1, "Normal"), myLCD.setColour(0xff, 0xff, 0xff), myLCD.setColour(0x00, 0xff, 0x00), myLCD.setColour(0x00, 0x88, 0x00), i);
  b2.dDefine(&myLCD,  80, myLCD.maxY()-60, 79, 59, setItem(2, "Instant"), myLCD.setColour(0xff, 0xff, 0xff), myLCD.setColour(0x00, 0x00, 0xff), myLCD.setColour(0x00, 0x00, 0x88), i);
  b7.dDefine(&myLCD,  myLCD.maxX()-80, myLCD.maxY()-60, 79, 59, setItem(0, "Stop"), myLCD.setColour(0xff, 0xff, 0xff), myLCD.setColour(0xff, 0x00, 0x00), myLCD.setColour(0x88, 0x00, 0x00), i);

  b1.enable(true);
  b2.enable(true);
  b7.enable(true);
  b1.draw();
  b2.draw();
  b7.draw();

  //    myLCD.setFont(3);
  //    myLCD.gText(0,  0, 0xffff, "         1         2    ");
  //    myLCD.gText(0, 20, 0xffff, "12345678901234567890123456"); 
  //    myLCD.gText(0, 60, 0xffff, ftoa(myLCD.fontX(), 0, 8)); 
  //
  //    myLCD.setFont(2);
  //    myLCD.gText(0,  80, 0xffff, "         1         2         3         4");
  //    myLCD.gText(0, 100, 0xffff, "1234567890123456789012345678901234567890"); 
  //    myLCD.gText(0, 120, 0xffff, ftoa(myLCD.fontX(), 0, 8)); 

  myLCD.setFont(1);
  myLCD.gText(0,  0, 0xffff, "         1         2         3         4");
  myLCD.gText(0, 20, 0xffff, "1234567890123456789012345678901234567890"); 
  myLCD.gText(0, 60, 0xffff, ftoa(myLCD.fontX(), 0, 8)); 

  myLCD.setFont(0);
  myLCD.gText(0,  80, 0xffff, "         1         2         3         4         5");
  myLCD.gText(0, 100, 0xffff, "12345678901234567890123456789012345678901234567890123"); 
  myLCD.gText(0, 120, 0xffff, ftoa(myLCD.fontX(), 0, 8)); 
}

uint8_t c;

void loop() {

  if (myLCD.getTouchActivity()) {

    myLCD.getTouchXY(x, y);
    myLCD.setFont(0);
    myLCD.gText(0,  0, 0xffff, ftoa(x, 0, 5)); 
    myLCD.gText(0, 15, 0xffff, ftoa(y, 0, 5)); 

    // quit
    if (b1.check()) {
  myLCD.setPenSolid(true);
  myLCD.dRectangle(40, 40, 100, 100, myLCD.setColour(random(0, 0xff), random(0, 0xff), random(0, 0xff)));       
    } 
    if (b2.check(true)) {
  myLCD.setPenSolid(true);
  myLCD.dRectangle(180, 40, 100, 100, myLCD.setColour(random(0, 0xff), random(0, 0xff), random(0, 0xff)));       
      
    }
    
    if (b7.check()) {
      myLCD.off();
      while(true);
    }



  }
  myLCD.setFont(0);
  myLCD.setFontSolid(true);
  myLCD.gText( 0, 225, 0xffff, String(millis()-l));
  l=millis();
}



















