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

// test release
#if SERIAL_LCD_RELEASE < 23
#error required SERIAL_LCD_RELEASE 23
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
uint8_t a;

#include "Gallery.h"
Gallery myGallery;


void setup() {
  Serial.begin(19200);
  Serial.print("\n\n\n***\n");

  // === Serial port initialisation ===
#if defined(__I2C_Serial__)
  Serial.print("i2c\n");
  Wire.begin();
  mySerial.begin(9600);

#elif defined(__AVR__)  || defined (__AVR_ATmega328P__) || defined (__AVR_ATmega328P__)
  Serial.print("avr\n");
  mySerial.begin(9600);

#elif defined(__PIC32MX__) 
  Serial.print("chipKIT\n");
  Serial1.begin(9600);

#endif 
  // === End of Serial port initialisation ===

  myLCD.begin(4);

  Serial.print("begin\n");

  //  // === Serial port speed change ===
  //  myLCD.setSpeed(38400);
  //#if defined(__I2C_Serial__)
  //  mySerial.begin(38400);
  //
  //#elif defined(__AVR__)  || defined (__AVR_ATmega328P__) || defined (__AVR_ATmega328P__)
  //  mySerial.begin(38400);
  //
  //#elif defined(__PIC32MX__) 
  //  Serial1.begin(38400);
  //
  //#endif 
  //  // === End of Serial port speed change ===
  //
  Serial.print("readScreenGCI initSD");
  Serial.print("\t");
  a =   myLCD.initSD();
  Serial.print(a, HEX);
  Serial.print("\n");
  
  Serial.print("readScreenGCI checkSD");
  Serial.print("\t");
  a =   myLCD.checkSD();
  Serial.print(a, HEX);
  Serial.print("\n");

  myLCD.setOrientation(0x03);

  //  myLCD.setPenSolid(true);
  //  myLCD.setFontSolid(true);
  //
  //  myLCD.setFont(0);
  //  myLCD.gText( 0, 210, 0xffff, myLCD.WhoAmI());
  //  delay(500);

//  Serial.print("readScreenGCI images.Gci");
//  Serial.print("\t");
//  a = myLCD.findFile("images.gci");
//  Serial.print(a, HEX);
//  Serial.print("\n");
//
//
//  Serial.print("readScreenGCI images.Dat");
//  Serial.print("\t");
//  a = myLCD.findFile("images.dat");
//  Serial.print(a, HEX);
//  Serial.print("\n");

  Serial.print("myGallery.begin(images)");
  Serial.print("\t");
  a = myGallery.begin(&myLCD, "images");
  Serial.print(a, HEX);
  Serial.print("\n");

myLCD.setTouch(true);

//  Serial.print("* readTextFile");
//  Serial.print("\n");

//  a = myLCD.readTextFile("images.dat", 7, callbackReadFile);
//  Serial.print("* ");
//  Serial.print(a, HEX);
//  Serial.print("\n");

//  a = myLCD.openTextFileDelimiter("images.dat", '\n', callbackReadFile);
//  Serial.print("* ");
//  Serial.print(a, HEX);
//  Serial.print("\n");


//  a = myLCD.readScreenFAT("160.gci");
//  Serial.print(a, HEX);
//  Serial.print("\n");
//  delay(10000);

}

uint8_t c;

void loop() {

  Serial.print("... ");
    if (myLCD.getTouchActivity()>0) {
      myLCD.off();
      while(true);
    }

  a = myGallery.showNext();
  
 a = myGallery.showImage(1);
  
  Serial.print("\nImage ");
  Serial.print(myGallery.index(), DEC);
  Serial.print("\t");
  Serial.print(a, HEX);
  Serial.print("\n");
  
  //
  //
  //
  //  myLCD.setFont(0);
  //  myLCD.setFontSolid(true);
  //  myLCD.gText( 250, 225, 0xffff, ftoa(millis()-l, 0, 6));
  //  l=millis();
  delay(3000);
}























