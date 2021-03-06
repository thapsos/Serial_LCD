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

#include "WProgram.h"
#include "Wire.h"
#include "Serial_LCD.h"
#include "GUI.h"
#include "proxySerial.h"

// test release
#if GUI_RELEASE < 23
#error required GUI_RELEASE 23
#endif

// === Serial port choice ===

// uncomment for I2C serial interface
//#define __I2C_Serial__

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

button b7; // ( &myLCD);
//dialog d1; // ( &myLCD);


void setup() {
  Serial.begin(19200);
  Serial.print("\n\n\n***\n");

  // === Serial port initialisation ===
#if defined(__I2C_Serial__)
  Serial.print("i2c\t");
  Wire.begin();
  mySerial.begin(9600);

#elif defined(__AVR__)  || defined (__AVR_ATmega328P__) || defined (__AVR_ATmega328P__)
  Serial.print("avr\t");
  mySerial.begin(9600);

#elif defined(__PIC32MX__) 
  Serial.print("chipKIT\t");
  Serial1.begin(9600);

#endif 
  // === End of Serial port initialisation ===

  Serial.print("s");

  myLCD.begin(4);  // 9600 at start-up
  Serial.print("*\n");

  //  // === Serial port speed change ===
  //  myLCD.setSpeed(38400);
  //#if defined(__I2C_Serial__)
  //  mySerial.begin(38400);
  //
  //#elif defined(__AVR__)  || defined (__AVR_ATmega328P__) | defined (__AVR_ATmega328P__)
  //  mySerial.begin(38400);
  //
  //#elif defined(__PIC32MX__) 
  //  Serial1.begin(38400);
  //
  //#endif 
  //  // === End of Serial port speed change ===

  myLCD.setOrientation(0x03);

  Serial.print("\n SD \t");
  Serial.print(myLCD.initSD(), HEX);
  Serial.print("\n RAW \t");
  Serial.print(myLCD.checkRAW(), HEX);
  Serial.print("\n write \t");
  Serial.print(myLCD.appendStringFile("test.txt", "this is a test\n"), HEX);

  myLCD.setPenSolid(true);
  myLCD.setFontSolid(true);

  myLCD.setFont(0);
  myLCD.gText( 0, 210, 0xffff, myLCD.WhoAmI());

  myLCD.setTouch(true);

  l=millis();

  uint16_t u;
  Serial.println("\n\nprompt 1");
  u=dialog(&myLCD, "Dialog 1", uint8_t(1), myLCD.setColour(0xff,0xff,0xff), myLCD.setColour(0x20,0x20,0x20), myLCD.setColour(0x80,0x80,0x80), String("First line"), String("OK"), myLCD.setColour(0xff,0xff,0xff), myLCD.setColour(0x00,0xff,0x00), myLCD.setColour(0x00,0x80,0x00));
  Serial.print(u, DEC);
  Serial.print("\n");

  myLCD.setFontSolid(true);
  myLCD.setFont(2);
  myLCD.gText(0, 160, 0xffff, u); 


  myLCD.setFont(3);
  myLCD.gText(0,  0, 0xffff, "         1         2    ");
  myLCD.gText(0, 20, 0xffff, "12345678901234567890123456"); 
  myLCD.gText(0, 40, 0xffff, ftoa(myLCD.fontX(), 0, 8)); 

  myLCD.setFont(2);
  myLCD.gText(0,  60, 0xffff, "         1         2         3         4");
  myLCD.gText(0, 80, 0xffff, "1234567890123456789012345678901234567890"); 
  myLCD.gText(0, 100, 0xffff, ftoa(myLCD.fontX(), 0, 8)); 

  myLCD.setFont(1);
  myLCD.gText(0,  120, 0xffff, "         1         2         3         4");
  myLCD.gText(0, 140, 0xffff, "1234567890123456789012345678901234567890"); 
  myLCD.gText(0, 160, 0xffff, ftoa(myLCD.fontX(), 0, 8)); 

  myLCD.setFont(0);
  myLCD.gText(0,  180, 0xffff, "         1         2         3         4         5");
  myLCD.gText(0, 200, 0xffff, "12345678901234567890123456789012345678901234567890123"); 
  myLCD.gText(0, 220, 0xffff, ftoa(myLCD.fontX(), 0, 8)); 

  Serial.println("\n\nprompt 2");
  u=dialog(&myLCD, String("Dialog 2"), uint8_t(2), myLCD.setColour(0xff,0xff,0xff), myLCD.setColour(0x20,0x20,0x20), myLCD.setColour(0x80,0x80,0x80), String("First line"), String("Yes"), myLCD.setColour(0xff,0xff,0xff), myLCD.setColour(0x00,0xff,0x00), myLCD.setColour(0x00,0x80,0x00), "Second line","No",myLCD.setColour(0xff,0xff,0xff), myLCD.setColour(0xff,0x00,0x00), myLCD.setColour(0x80,0x00,0x00),"Third line","Cancel",myLCD.setColour(0xff,0xff,0xff), myLCD.setColour(0xff,0xff,0x00), myLCD.setColour(0x80,0x80,0x00));
  Serial.print(u, DEC);
  Serial.print("\n");

  myLCD.setFontSolid(true);
  myLCD.setFont(2);
  myLCD.gText(0, 160, 0xffff, u); 

  uint16_t i=9;
  b7.dStringDefine(&myLCD, 160, 120, 79, 59, "STOP",        myLCD.setColour(0xff, 0xff, 0xff), myLCD.setColour(0xff, 0x00, 0x00), myLCD.setColour(0x88, 0x00, 0x00), i);

  b7.enable(true);
  b7.draw();

}


void loop() {

  if (myLCD.getTouchActivity()) {
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
  myLCD.gText( 250, 225, 0xffff, String(millis()-l));
  l=millis();
}



