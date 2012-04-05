// 
// 4D Systems μLCD-μLED-μVGA Serial_LCD Library Suite
// Arduino 1.0 Library
//
//  Serial_LCD1_main.pde
//  Sketch 
//  ----------------------------------
//  Developed with embedXcode
//
//  Project Serial_LCD1_main
//  Created by Rei VILO on 16/03/12
//  Copyright (c) 2012 http://sites.google.com/site/vilorei
//

// Core library
#include "Arduino.h"	

// Include application, user and local libraries
#include "Wire.h"
#include "proxySerial.h"
#include "Serial_LCD.h"
#include "Gallery.h"

// test release
#if GALLERY_RELEASE < 202
#error required GALLERY_RELEASE 202
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

// Define variables and constants
Serial_LCD myLCD( &myPort); 
Gallery myGallery;
uint8_t a;


// Add setup code 
void setup() {                
  Serial.begin(19200);
  Serial.print("\n\n\n***\n");
  Serial.print("\n");

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
  myLCD.begin(4);

  Serial.print(myLCD.WhoAmI());
  Serial.print("\n");

  a = myLCD.initSD();
  a = myGallery.begin(&myLCD, "Images");

  if ( a==0) {
    myLCD.off();
    Serial.print("No image. STOP.\n");
    while (true);
  }

  myLCD.setTouch(true);
  myGallery.showImage(0);
  Serial.print(myGallery.index());
  Serial.print("\t");
}

// Add loop code 
void loop() {

  delay(3000);

  if (myLCD.getTouchActivity()>0) {
    myLCD.off();
    while(true);
  }

  myGallery.showNext();

  if ( myGallery.index()==0 ) Serial.print("\n");
  Serial.print(myGallery.index());
  Serial.print("\t");
}


