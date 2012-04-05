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
#include "proxySerial.h"

// test release
#if SERIAL_LCD_RELEASE < 223
#error required SERIAL_LCD_RELEASE 223
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


uint8_t aa;

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
  myLCD.begin();
  

delay(100);
  aa=myLCD.setOrientation(0x03);

delay(100);
  Serial.print("\n line \t");
  aa=myLCD.line(0,0,100,100, greenColour);
  Serial.print(aa, DEC);

delay(100);
  myLCD.setPenSolid(true);
  Serial.print("\n rectangle \t");
  aa=myLCD.rectangle(239,0,319,256, redColour);
  Serial.print(aa, DEC);

delay(100);
  myLCD.setPenSolid(false);
  Serial.print("\n circle \t");
  aa=myLCD.circle(200,100, 50, blueColour);
  aa=myLCD.circle(200,100, 49, blueColour);
  aa=myLCD.circle(200,100, 47, blueColour);
  Serial.print(aa, DEC);

delay(200);
  Serial.print("\n setFont \t");
  aa=myLCD.setFont(2);
  Serial.print(aa, DEC);

delay(100);
  Serial.print("\n gText \t");
  aa=myLCD.gText(25, 25, "String", whiteColour);
  Serial.print(aa, DEC);

delay(100);
//  Serial.print("\n tText \t");
//  aa=myLCD.tText(0, 19, 0xffff, "WhoAmI? "+ myLCD.WhoAmI());
//  Serial.print(aa, DEC);

//  for (uint8_t i=0; i<myLCD.WhoAmI().length(); i++)   
//    Serial.print(myLCD.WhoAmI().charAt(i));
//  Serial.print("\n");

  delay(100);

  Serial.print("\n triangle \t");
  aa=myLCD.triangle(160, 200, 80, 160, 60, 100, 0xffff);
  Serial.print(aa, DEC);

delay(100);
  Serial.print("\n setTouch \t");
  aa=myLCD.setTouch(true);
  Serial.print(aa, DEC);

  Serial.print("\n ");
}

uint16_t x, y;
uint8_t b;

void loop() {


  Serial.print("\n getTouchActivity \t");
  aa=myLCD.getTouchActivity();
  Serial.print(aa, DEC);


  if (aa) {
aa=0;
Serial.print(" * " );
    Serial.print("\t getTouchXY \t");
    aa=myLCD.getTouchXY(x, y);
delay(10);
    Serial.print(aa, DEC);

    Serial.print("\t");
    Serial.print(x, DEC );      
    Serial.print("\t");
    Serial.print(y, DEC );

    if (x>256) {
      myLCD.off();
      while(1);
    }
    Serial.println();
  }


  delay(200);

}












