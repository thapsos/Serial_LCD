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

button myButton;

// menu declaration and size
item myMenuItems[] = { 
  {     0x0000, "Menu 0"        }  ,    
  {     0x1000, "Item 1"        }  ,
  {     0x1100, "Item 11"       }  ,
  {     0x1200, "Item 12"       }  ,
  {     0x2000, "Item 2"        }  ,
  {     0x2100, "Item 21"       }  ,
  {     0x2110, "Item 211"      }  ,
  {     0x2120, "Item 212"      }  ,
  {     0x2121, "Item 2121"     }  ,
  {     0x2122, "Item 2122"     }  ,
  {     0x2200, "Item 22"       }  ,
  {     0x2300, "Item 23"       }  ,
  {     0x3000, "Item 3"        } 
};

uint8_t nItems = sizeof(myMenuItems) / sizeof(myMenuItems[0]);


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

  myLCD.begin();  // 9600 at start-up

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

  myLCD.initSD();
  myLCD.setRAW(true);

  if (myLCD.checkSD()) {
    if (myLCD.checkRAW()) dLabel(&myLCD, 260, 30, 60, 40, "RAW",   myLCD.setColour(0x00, 0xff, 0x00), 0x0000, 0, 0, 9);
    else dLabel(&myLCD, 260, 30, 60, 40, "FAT",  myLCD.setColour(0x00, 0x00, 0xff), 0x0000, 0, 0, 9);
  } else {
    dLabel(&myLCD, 260, 30, 60, 40, "no SD",   myLCD.setColour(0xff, 0x00, 0x00), 0x0000, 0, 0, 9);
  }
  
//  myLCD.setRAW(false);
//
//  Serial.print("\n myLCD.setRAW(false);\t");
//  myLCD.appendStringFile("test.txt", "myLCD.setRAW(false);\t");
//  l=millis();
//  for (int i=0; i<10; i++) {
//    myLCD.dSaveScreenFAT("essai.tmp", 40, 40, 200, 200);
//    myLCD.line(10, 20*i, myLCD.maxX()-20, 20*i, 0xffff);
//    myLCD.readScreenFAT("essai.tmp", 40, 40);
//    myLCD.line(10, 20*i+6, myLCD.maxX()-20, 20*i+6, 0xffff);
//  }
//  myLCD.appendStringFile("test.txt", ftoa((millis()-l)/10, 1, 6) + "\n");
//  Serial.print(ftoa((millis()-l)/10, 1, 6));
//
//  myLCD.clear();
//
//  myLCD.setRAW(true);
//  Serial.print("\n myLCD.setRAW(true);\t");
//  myLCD.appendStringFile("test.txt", "myLCD.setRAW(true);\t");
//  l=millis();
//  for (int i=0; i<10; i++) {
//    myLCD.dSaveScreenRAW((uint32_t)0, 40, 40, 200, 200);
//    myLCD.line(10, 20*i, myLCD.maxX()-20, 20*i, 0xffff);
//    myLCD.readScreenRAW((uint32_t)0, 40, 40);
//    myLCD.line(10, 20*i+6, myLCD.maxX()-20, 20*i+6, 0xffff);
//  }
//  myLCD.appendStringFile("test.txt", ftoa((millis()-l)/10, 1, 6) + "\n");
//  Serial.print(ftoa((millis()-l)/10, 1, 6));
//  Serial.print("\n");

  myLCD.setOrientation(0x03);
  myLCD.setPenSolid(true);
  myLCD.setFontSolid(true);

  myLCD.setFont(0);
  myLCD.gText( 0, 210, 0xffff, myLCD.WhoAmI());

  myLCD.setTouch(true);

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

   uint32_t sizeRAW = myLCD.getSectors(80, 30);
//   Serial.print(sizeRAW, DEC);
   Serial.print("\n");
   for (int i=0; i<8; i++)       myLCD.dSaveScreenRAW( sizeRAW*(i+0),  0, i*30, 80, 30 ); 
   
   myLCD.line(20, 0, 20, 239, 0xffff);
//   delay(500);
   
   Serial.print("\n");
   for (int i=0; i<8; i++)       myLCD.readScreenRAW( sizeRAW*(i+0),  0, i*30 ); 
   Serial.print("\n");

//delay(1000);


  dLabel(&myLCD, 20, 30, 220, 25, "label left",   myLCD.setColour(0xff, 0x00, 0x00), myLCD.setColour(0x00, 0x2f, 0x2f), 1, 0, 9);
  dLabel(&myLCD, 20, 60, 220, 25, "label center", myLCD.setColour(0x00, 0xff, 0x00), myLCD.setColour(0x2f, 0x00, 0x2f), 0, 0, 9);
  dLabel(&myLCD, 20, 90, 220, 25, "label right",  myLCD.setColour(0x00, 0x00, 0xff), myLCD.setColour(0x2f, 0x2f, 0x00), 2, 0, 9);
}

uint8_t c;
uint16_t result;
boolean b=true;

void loop() {

  if (b) {
    myButton.dStringDefine(&myLCD, 160, 120, 79, 59, String(result, HEX), myLCD.setColour(0xff, 0xff, 0xff), myLCD.setColour(0xff, 0x00, 0x00), myLCD.setColour(0x88, 0x00, 0x00), 9);
    myButton.enable(false);
    myButton.draw();

    result = menu(&myLCD, myMenuItems, nItems, (uint16_t)0xffff, myLCD.setColour(0x00, 0x00, 0xff), myLCD.setColour(0x1f, 0x1f, 0x8f));

    myButton.dStringDefine(&myLCD, 160, 120, 79, 59, String(result, HEX), myLCD.setColour(0xff, 0xff, 0xff), myLCD.setColour(0xff, 0x00, 0x00), myLCD.setColour(0x88, 0x00, 0x00), 9);
    myButton.enable(true);
    myButton.draw();

    b=false;
  }

  c=myLCD.getTouchActivity();

  if (c>0) {
    myLCD.getTouchXY(x, y);
    myLCD.setFont(0);
    myLCD.gText(200, 0, 0xffff, ftoa(x, 0, 5)); 
    myLCD.gText(200, 15, 0xffff, ftoa(y, 0, 5)); 

    // quit
    if (myButton.check()) {
      String s;
      if ( 1 == dialog(&myLCD, "Quit?", 3, myLCD.setColour(0xff,0xff,0xff), myLCD.setColour(0x20,0x20,0x20), myLCD.setColour(0x80,0x80,0x80), String("Are you sure to quit?"), "Yes", myLCD.setColour(0xff,0xff,0xff), myLCD.setColour(0x00,0xff,0x00), myLCD.setColour(0x00,0x80,0x00), "Please confirm.","No",myLCD.setColour(0xff,0xff,0xff), myLCD.setColour(0xff,0x00,0x00), myLCD.setColour(0x80,0x00,0x00)) ) {
//        delay(2000);
        myLCD.off();
        while(true);
      } 
      else {
        b=true;
      }
    }
  }

  myLCD.setFont(0);
  myLCD.setFontSolid(true);
  myLCD.gText( 250, 225, 0xffff, String(millis()-l));
  l=millis();

}


