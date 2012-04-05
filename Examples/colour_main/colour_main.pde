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
#if SERIAL_LCD_RELEASE < 119
#error required SERIAL_LCD_RELEASE 119
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



  int8_t i;


  myLCD.setPenSolid(true);

  i=0;  
  myLCD.dRectangle(20*i, 0, 17, 100, blackColour);
  i++;  
  myLCD.dRectangle(20*i, 0, 17, 100, whiteColour);
  i++;  
  myLCD.dRectangle(20*i, 0, 17, 100, redColour);
  i++;  
  myLCD.dRectangle(20*i, 0, 17, 100, greenColour);
  i++;  
  myLCD.dRectangle(20*i, 0, 17, 100, blueColour);
  i++;  
  myLCD.dRectangle(20*i, 0, 17, 100, yellowColour);
  i++;  
  myLCD.dRectangle(20*i, 0, 17, 100, cyanColour);
  i++;  
  myLCD.dRectangle(20*i, 0, 17, 100, orangeColour);
  i++;  
  myLCD.dRectangle(20*i, 0, 17, 100, magentaColour);
  i++;
  myLCD.dRectangle(20*i, 0, 17, 100, violetColour);
  i++;
  myLCD.dRectangle(20*i, 0, 17, 100, grayColour);


  i=0;  
  myLCD.dRectangle(20*i, 100, 17, 100, myLCD.reverseColour(blackColour));
  i++;  
  myLCD.dRectangle(20*i, 100, 17, 100, myLCD.reverseColour(whiteColour));
  i++;  
  myLCD.dRectangle(20*i, 100, 17, 100, myLCD.reverseColour(redColour));
  i++;  
  myLCD.dRectangle(20*i, 100, 17, 100, myLCD.reverseColour(greenColour));
  i++;  
  myLCD.dRectangle(20*i, 100, 17, 100, myLCD.reverseColour(blueColour));
  i++;  
  myLCD.dRectangle(20*i, 100, 17, 100, myLCD.reverseColour(yellowColour));
  i++;  
  myLCD.dRectangle(20*i, 100, 17, 100, myLCD.reverseColour(cyanColour));
  i++;  
  myLCD.dRectangle(20*i, 100, 17, 100, myLCD.reverseColour(orangeColour));
  i++;  
  myLCD.dRectangle(20*i, 100, 17, 100, myLCD.reverseColour(magentaColour));
  i++;
  myLCD.dRectangle(20*i, 100, 17, 100, myLCD.reverseColour(violetColour));
  i++;
  myLCD.dRectangle(20*i, 100, 17, 100, myLCD.reverseColour(grayColour));


}


void loop() {
}























