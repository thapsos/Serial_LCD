// 
// 4D Systems μLCD-μLED-μVGA Serial_LCD Library Suite
// Arduino 1.0 Library
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

#include "Arduino.h"
#include "Wire.h"
#include "Serial_LCD.h"
#include "GUI.h"
#include "proxySerial.h"

// test release
#if GUI_RELEASE < 209
#error required GUI_RELEASE 209
#endif

// === Serial port choice ===
// --- Arduino Uno - software serial
#if defined (__AVR_ATmega328P__) 
#include "SoftwareSerial.h"
SoftwareSerial Serial3(2, 3); // RX, TX

// --- Arduino mega2560 - hardware serial
#elif defined (__AVR_ATmega2560__)
#endif
// === End of Serial port choice ===

ProxySerial myPort(&Serial3);
Serial_LCD myLCD(&myPort); 

uint16_t x, y;
uint32_t l;

button b7;



void setup() {
  Serial.begin(19200);
  Serial.print("\n\n\n***\n");

  // === Serial port initialisation ===
  // --- Arduino Uno - software serial
#if defined (__AVR_ATmega328P__) 
  Serial.print("software\n");
  // --- Arduino mega2560 - hardware serial
#elif defined (__AVR_ATmega2560__)
  Serial.print("hardware\n");
#endif
  // === End of Serial port initialisation ===
  
  Serial3.begin(9600);
  myLCD.begin(4);  // 9600 at start-up
  
  // === Serial port speed change ===
  myLCD.setSpeed(38400);
#if defined (__AVR_ATmega328P__) 
  Serial3.begin(38400);
#elif defined (__AVR_ATmega2560__)
  Serial3.begin(38400);
#endif
  // === End of Serial port speed change ===

  myLCD.setOrientation(0x03);
  myLCD.setPenSolid(true);
  myLCD.setFontSolid(true);

  myLCD.setFont(0);
  myLCD.gText( 0, 210, myLCD.WhoAmI(), 0xffff);

  myLCD.setTouch(true);

  l=millis();

  //    myLCD.setFont(3);
  //    myLCD.gText(0,  0, "         1         2    ");
  //    myLCD.gText(0, 20, "12345678901234567890123456"); 
  //    myLCD.gText(0, 60, ftoa(myLCD.fontX(), 0, 8)); 
  //
  //    myLCD.setFont(2);
  //    myLCD.gText(0,  80, "         1         2         3         4");
  //    myLCD.gText(0, 100, "1234567890123456789012345678901234567890"); 
  //    myLCD.gText(0, 120, ftoa(myLCD.fontX(), 0, 8)); 

  myLCD.setFont(1);
  myLCD.gText(0,  0, "         1         2         3         4");
  myLCD.gText(0, 20, "1234567890123456789012345678901234567890"); 
  myLCD.gText(0, 60, ftoa(myLCD.fontX(), 0, 8)); 

  myLCD.setFont(0);
  myLCD.gText(0,  80, "         1         2         3         4         5");
  myLCD.gText(0, 100, "12345678901234567890123456789012345678901234567890123"); 
  myLCD.gText(0, 120, ftoa(myLCD.fontX(), 0, 8)); 

  uint16_t i=9;
  b7.dDefine(&myLCD,  100, 100, 79, 59, setItem(1, "STOP"), myLCD.setColour(0xff, 0xff, 0xff), myLCD.setColour(0xff, 0x00, 0x00), myLCD.setColour(0x88, 0x00, 0x00), i);

  b7.enable(true);
  b7.draw();
}

uint8_t c;

void loop() {

  c=myLCD.getTouchActivity();

  if (c>0) {
    myLCD.getTouchXY(x, y);
    myLCD.setFont(0);
    myLCD.gText(200, 0, ftoa(x, 0, 5), 0xffff); 
    myLCD.gText(200, 15, ftoa(y, 0, 5)); // , 0xffff by default

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




















