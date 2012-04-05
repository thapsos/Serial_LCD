// 
// μLCD-32PT(SGC) 3.2” Serial LCD Display Module
// Arduino & chipKIT Library
//
// histogram_gauge Example - see README.txt
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
#include "Graphics.h"

// test release
#if GUI_RELEASE < 108
#error required GUI_RELEASE 108
#endif

#if GRAPHICS_RELEASE < 108
#error required GRAPHICS_RELEASE 108
#endif

// Uncomment for I2C serial
#define __I2C__

// === Serial port choice ===
// --- Arduino Uno - software serial
// --- I2C serial
#if defined (__I2C__)
#include "Wire.h"
#include "I2C_Serial.h"
I2C_Serial myI2CSerial;
ProxySerial myPort(&myI2CSerial);

#elif defined (__AVR_ATmega328P__) 
#include "NewSoftSerial.h"
NewSoftSerial mySerial(2, 3); // RX, TX
ProxySerial myPort(&mySerial);

// --- Arduino mega2560 - hardware serial
#elif defined (__AVR_ATmega2560__)
ProxySerial myPort(&Serial3);

// --- chipKIT UNO32 - hardware serial
#elif defined (__PIC32MX__)
ProxySerial myPort(&Serial1);

#else
#error Non defined board
#endif
// === End of Serial port choice ===

Serial_LCD myLCD(&myPort); 


uint16_t x, y;
uint32_t l;

button b7;
uint8_t a;
gGauge myGauge;
gHistogram myHistogram;


void setup() {
  Serial.begin(19200);
  Serial.print("\n\n\n***\n");

  // === Serial port initialisation ===
  // --- Arduino Uno - software serial
#if defined (__I2C__)
  Wire.begin();
  Serial.print("I2C\n");
  myI2CSerial.begin(9600);
  Serial.print("I2C\n");
#elif defined (__AVR_ATmega328P__) 
  Serial.print("software\n");
  mySerial.begin(9600);
  // --- Arduino mega2560 - hardware serial
#elif defined (__AVR_ATmega2560__)
  Serial.print("hardware\n");
  Serial3.begin(9600);
#elif defined (__PIC32MX__)
  Serial.print("hardware\n");
  Serial1.begin(9600);
// --- I2C serial
#endif
  // === End of Serial port initialisation ===

  Serial.print("I2C test: ");
  Serial.print(myI2CSerial.test(), DEC);
  Serial.print("\n");

  myLCD.begin(4);

  Serial.print("begin\n");

  // === Serial port speed change ===
  myLCD.setSpeed(38400);
#if defined (__I2C__)
  myI2CSerial.begin(38400);
#elif defined (__AVR_ATmega328P__) 
  mySerial.begin(38400);
#elif defined (__AVR_ATmega2560__)
  Serial3.begin(38400);
#elif defined (__PIC32MX__)
  Serial3.begin(38400);
#endif
  // === End of Serial port speed change ===

  myLCD.setOrientation(0x03);
  myLCD.setPenSolid(true);
  myLCD.setFontSolid(true);

  myLCD.setFont(0);
  myLCD.gText( 0, myLCD.maxY()-20, myLCD.WhoAmI());
  myLCD.setTouch(true);

  l=millis();

  b7.dDefine(&myLCD, myLCD.maxX()-70, myLCD.maxY()-40, 70, 40, setItem(0, "Stop"), myLCD.setColour(0xff, 0xff, 0xff), myLCD.setColour(0xff, 0x00, 0x00), myLCD.setColour(0x88, 0x00, 0x00), 9);
  b7.enable(true);
  b7.draw();

// void dDefine(Serial_LCD * lcd0, 
//    uint16_t x0, uint16_t y0, uint16_t dx, uint16_t dy, 
//    float valueMin, float valueMax, 
//    uint16_t lapse=0, uint16_t memory=8, uint16_t gridX=4, uint16_t gridY=0, 
//    boolean continous=false, 
//    uint16_t backColour=blackColour, uint16_t frontColour=whiteColour, 
//    uint16_t gridColour=yellowColour, uint16_t valueColour=blueColour, 
//    uint16_t minColour=greenColour, uint16_t maxColour=redColour);

  myHistogram.dDefine(&myLCD, 20, 40, myLCD.maxX()/2-40, myLCD.maxY()-40-80, -2.0, 2.0, 250, 8, 4, 10, false);
  myGauge.dDefine(&myLCD, myLCD.maxX()/2+20, 20, myLCD.maxX()/2-40, myLCD.maxY()-40-40, -2.0, 2.0, 8, 8);
}

uint32_t ll = 0;
uint16_t i = 0;
float v;
uint8_t c;


void loop() {
  Serial.print(".");
  
  //  // Solution 1
  //  while (millis()-l<100) {
  //    v = cos(i*PI/64.0) + 0.3*cos(i*PI/16.0+PI/8.0);
  //    i++;
  //    i %= 256;
  //  }

  // Solution 2
  v = cos(i*PI/64.0) + 0.3*cos(i*PI/16.0+PI/8.0);
  i++;
  i %= 256;
  while (millis()-ll<250);

  ll=millis();
  myHistogram.draw( v );
  myGauge.draw( v, ftoa(v, 2, 7) );

//  myLCD.setFont(3);
//  myLCD.setFontSolid(true);
//  myLCD.gText( 160, 180, ftoa(v, 2, 10 ));

  c = myLCD.getTouchActivity();
  
  if ( c>0 ) {
    
    myLCD.getTouchXY(x, y);
    myLCD.setFont(0);
    myLCD.gText(200, 0, ftoa(x, 0, 5)); 
    myLCD.gText(200, 15, ftoa(y, 0, 5)); 

    // quit
    if (b7.check()) {
      myLCD.off();
      while(true);
    } // quit
  } // getTouchActivity

  myLCD.setFont(0);
  myLCD.setFontSolid(true);
  myLCD.gText( 250, myLCD.maxY()-20, ttoa(millis()-l, 0, 6));
  l=millis();

  //  delay(100);
}







