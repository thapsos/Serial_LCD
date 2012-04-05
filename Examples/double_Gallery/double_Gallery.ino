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
SoftwareSerial mySerial1(2, 3); // RX, TX
SoftwareSerial mySerial2(5, 6); // RX, TX

// --- Arduino mega2560 - hardware serial
#elif defined (__AVR_ATmega2560__)
#endif
// === End of Serial port choice ===

ProxySerial myPort1(&Serial1);
ProxySerial myPort2(&Serial2);

// Define variables and constants
Serial_LCD myLCD1(&myPort1); 
Serial_LCD myLCD2(&myPort2); 
Gallery myGallery1;
Gallery myGallery2;
uint8_t a, b;


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
  
  Serial.print("1 \t");
  Serial1.begin(9600);
  myLCD1.begin();
  Serial.print(myLCD1.WhoAmI());
  Serial.print("\n");
  
  Serial.print("2 \t");
  Serial2.begin(9600);
  myLCD2.begin();
  Serial.print(myLCD2.WhoAmI());
  Serial.print("\n");

  a = myLCD1.initSD();
  a = myGallery1.begin(&myLCD1, "Images1");
  b = myLCD2.initSD();
  b = myGallery2.begin(&myLCD2, "Images2");

  if (a==0) {
    myLCD1.off();
    Serial.print("1: No image. STOP.\n");
    while (true);
  }
  if (b==0) {
    myLCD2.off();
    Serial.print("2: No image. STOP.\n");
    while (true);
  }

  myLCD1.setTouch(true);
  myLCD2.setTouch(true);
  myGallery1.showImage(0);
  myGallery2.showImage(0);
}

// Add loop code 
void loop() {

  Serial.print(myGallery1.index());
  Serial.print(".");
  Serial.print(myGallery2.index());
  Serial.print("\t");

  delay(3000);

  if (myLCD1.getTouchActivity() || myLCD2.getTouchActivity()) {
    myLCD1.off();
    myLCD2.off();
    while(true);
  }

  myGallery1.showNext();
  myGallery2.showNext();

  if ( myGallery1.index()==0 ) Serial.print("\n");
  if ( myGallery2.index()==0 ) Serial.print("\n");
}


