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

// === Serial port choice ===

// uncomment for I2C serial interface
#define __I2C_Serial__

// --- I2C Case -
#if defined(__I2C_Serial__)
#include "Wire.h"
#include "I2C_Serial.h"
I2C_Serial mySerial(0);
ProxySerial myPort(&mySerial);

// --- Arduino SoftwareSerial Case - Arduino only
#elif defined(__AVR__)  || defined (__AVR_ATmega328P__) | defined (__AVR_ATmega328P__)
#include "NewSoftSerial.h"
NewSoftSerial mySerial(2, 3); // RX, TX
ProxySerial myPort(&mySerial);

// --- chipKIT HardwareSerial Case - chipKIT
#elif defined(__PIC32MX__) 
ProxySerial myPort(&Serial1);

#else
#error Non defined board
#endif 

// === End of Serial port choice ===

Serial_LCD myLCD( &myPort); 

uint16_t x, y;
uint32_t l;

button b7;



void setup() {
  Serial.begin(19200);
  Serial.print("\n\n\n***\n");

  // === Serial port initialisation ===
#if defined(__I2C_Serial__)
  Serial.print("i2c\n");
  Wire.begin();
  mySerial.begin(9600);

#elif defined(__AVR__)  || defined (__AVR_ATmega328P__) | defined (__AVR_ATmega328P__)
  Serial.print("avr\n");
  mySerial.begin(9600);

#elif defined(__PIC32MX__) 
  Serial.print("chipKIT\n");
  Serial1.begin(9600);

#endif 
  // === End of Serial port initialisation ===

  myLCD.begin();

  Serial.print("begin\n");

  // === Serial port speed change ===
  myLCD.setSpeed(38400);
#if defined(__I2C_Serial__)
  mySerial.begin(38400);

#elif defined(__AVR__)  || defined (__AVR_ATmega328P__) || defined (__AVR_ATmega328P__)
  mySerial.begin(38400);

#elif defined(__PIC32MX__) 
  Serial1.begin(38400);

#endif 
  // === End of Serial port speed change ===


  myLCD.setOrientation(0x03);

  myLCD.setPenSolid(true);
  myLCD.setFontSolid(true);

  myLCD.setFont(0);
  myLCD.gText( 0, 210, 0xffff, myLCD.WhoAmI());

  myLCD.setTouch(true);

  l=millis();

  b7.define(&myLCD, 0, 0, 79, 59, setItem(0, "Stop"), myLCD.setColour(0xff, 0xff, 0xff), myLCD.setColour(0xff, 0x00, 0x00), myLCD.setColour(0x88, 0x00, 0x00), 9);

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
  myLCD.gText( 0, 180, 0xffff, ftoa((chrono1-chrono0), 0, 10 ));

  chrono0=millis();

  myLCD.setPenSolid(true);
  for (int i=0; i<10; i++) {
    for (int j=0; j<10; j++) {
      myLCD.circle(120+j*10, 30+i*10, 30, random(0, 0xffff));
    }
  }

  chrono1=millis();
  myLCD.gText( 160, 180, 0xffff, ftoa((chrono1-chrono0), 0, 10 ));

}

uint8_t c;

void loop() {

  c=myLCD.getTouchActivity();

  if (c>0) {
    myLCD.getTouchXY(x, y);
    myLCD.setFont(0);
    myLCD.gText(200, 0, 0xffff, ftoa(x, 0, 5)); 
    myLCD.gText(200, 15, 0xffff, ftoa(y, 0, 5)); 

    // quit
    if (b7.check()) {
      myLCD.off();
      while(true);
    }



  }
  myLCD.setFont(0);
  myLCD.setFontSolid(true);
  myLCD.gText( 250, 225, 0xffff, ftoa(millis()-l, 0, 6));
  l=millis();
}





















