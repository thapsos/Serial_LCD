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


// Needs to be defined in both proxySerial and main program
#define __i2cSerialPort__ 

#include "Serial_LCD.h"
#include "proxySerial.h"
#include "GUI.h"


// test release
#if GUI_RELEASE < 23
#error required GUI_RELEASE 23
#endif

#define __i2cSerialPort__


// I2C case
#if defined(__i2cSerialPort__) 
#include "Wire.h"
#include "I2C_Serial.h"
I2C_Serial myI2CSerial;
ProxySerial mySerial(&myI2CSerial);

// Arduino Case
#elif defined(__AVR__)  || defined (__AVR_ATmega328P__)  
#include "NewSoftSerial.h"
NewSoftSerial nss(2, 3); // RX, TX
ProxySerial mySerial(&nss);

// chipKIT Case
#elif defined(__PIC32MX__) 
ProxySerial Serial1;

#else
#error Non defined board
#endif


Serial_LCD myLCD( &mySerial); 

uint16_t x, y;
uint32_t l;

button b7;



void setup() {
  Serial.begin(19200);
  Serial.print("\n\n\n***\n");

  //#if defined(__AVR__)  || defined (__AVR_ATmega328P__) | defined (__AVR_ATmega328P__)
  //  Serial.print("avr\t");
  //  Serial.print(__AVR__);
  //  Serial.print("\n");
  //#elif defined(__PIC32MX__) 
  //  Serial.print("chipKIT\t");
  //  Serial.print(__PIC32MX__);
  //  Serial.print("\n");
  //#endif 

#if defined(__i2cSerialPort__) 
  Wire.begin();
#endif 

  myI2CSerial.begin(9600);
  Serial.print("I2C 9600 \n");


  myLCD.begin();
  Serial.print("LCD begin \n");
  myLCD.setOrientation(0x03);

//  myLCD.setSpeed(19200);
//  myI2CSerial.begin(19200);
//  Serial.print("I2C 19200 \n");

  myLCD.setPenSolid(true);
  myLCD.setFontSolid(true);

  myLCD.setFont(0);
  myLCD.gText( 0, 210, myLCD.WhoAmI());

  myLCD.setTouch(true);

  l=millis();

  myLCD.ellipse(100, 100, 50, 20, myLCD.setColour(0xff,0x00,0x00));

  b7.define(&myLCD, 0, 0, 79, 59, setItem(0, "Stop"), myLCD.setColour(0xff, 0xff, 0xff), myLCD.setColour(0xff, 0x00, 0x00), myLCD.setColour(0x88, 0x00, 0x00), 9);
  b7.enable(true);
  b7.draw();

  myLCD.setPenSolid(false);
  myLCD.rectangle(40, 40, 200, 200, myLCD.setColour(0x00, 0x00, 0xff));

  myLCD.detectTouchRegion(40, 40, 200, 200);
}

uint8_t c;

void loop() {
  c=myLCD.getTouchActivity();
  myLCD.setFont(3);
  myLCD.gText(140, 10, ftoa(c, 0, 5)); 

  if ((c==1) || (c==3)) {
    myLCD.getTouchXY(x, y);

    myLCD.setFont(1);
    myLCD.gText(200, 0, ftoa(x, 0, 5)); 
    myLCD.gText(200, 20, ftoa(y, 0, 5)); 

    myLCD.point(x, y, 0xffff);

    // quit
    if (b7.check()) {
      myLCD.off();
      while(true);
    }
  }

  myLCD.setFont(0);
  myLCD.setFontSolid(true);
  myLCD.gText( 250, 225, String(millis()-l));
  l=millis();
}



