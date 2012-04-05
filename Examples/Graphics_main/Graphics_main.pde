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
#include "proxySerial.h"
#include "Graphics.h"

// test release
#if GRAPHICS_RELEASE < 103
#error required GRAPHICS_RELEASE 103
#endif

// === Serial port choice ===

// uncomment for I2C serial interface
//#define __I2C_Serial__

// --- I2C Case -
#if defined(__I2C_Serial__)
#include "Wire.h"
#include "I2C_Serial.h"
  // Test release
  #if I2C_SERIAL_RELEASE < 103
  #error required I2C_SERIAL_RELEASE 103
  #endif

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
gClock myClock;
gDirection myDirection;

uint16_t x, y;
uint32_t l;


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
    
    myLCD.begin();  // 9600 at start-up
    
    // === Serial port speed change ===
    myLCD.setSpeed(38400);
#if defined(__I2C_Serial__)
    mySerial.begin(38400);
    
#elif defined(__AVR__)  || defined (__AVR_ATmega328P__) | defined (__AVR_ATmega328P__)
    mySerial.begin(38400);
    
#elif defined(__PIC32MX__) 
    Serial1.begin(38400);
    
#endif 
    // === End of Serial port speed change ===
    
    myLCD.setOrientation(0x03);
    myLCD.setTouch(true);
    
    myClock.define(&myLCD, myLCD.maxX()/4, myLCD.maxY()/2, 60, myLCD.setColour(0x00, 0xff, 0x00), myLCD.setColour(0x00, 0x00, 0xff), myLCD.setColour(0xff, 0x00, 0x00), myLCD.setColour(0x00, 0x00, 0x00), myLCD.setColour(0xff, 0xff, 0xff));
        
    myDirection.define(&myLCD, myLCD.maxX()*3/4, myLCD.maxY()/2, 60, myLCD.setColour(0x00, 0x00, 0xff), myLCD.setColour(0x00, 0xff, 0x00));
}

uint8_t g;

void loop() {
    
    
    if (myLCD.getTouchActivity()) {
        myLCD.off();
        while(true);
    } 
    
    l=millis();
    myClock.draw((uint8_t)(g/5), 10, g);
    myLCD.setFont(0);
    myLCD.setFontSolid(true);
    myLCD.gText( myLCD.maxX()/4-20, 225, 0xffff, ftoa(millis()-l, 0, 5));
    
    l=millis();
    myDirection.draw(g*2.0);
    myLCD.setFont(0);
    myLCD.setFontSolid(true);
    myLCD.gText( myLCD.maxX()*3/4-20, 225, 0xffff, ftoa(millis()-l, 0, 5));
    
    g++;
    g %= 60;
    
    delay(333);
}





