// 
// 4D Systems μLCD-μLED-μVGA Serial_LCD Library Suite
// Arduino 1.0 Library
//
// SC16IS750 I2C slave bridge to serial
//
// Mar 19, 2012 release 204
// see README.txt
//
// © Rei VILO, 2010-2012
// CC = BY NC SA
// http://sites.google.com/site/vilorei/
// https://sites.google.com/site/vilorei/arduino/13--serial-touch-320x240-lcd-screen
//


#define I2C_SERIAL_RELEASE 204

#ifndef I2C_Serial_h
#define I2C_Serial_h

#include "Arduino.h"
#include "Stream.h"
#include "Wire.h"

// I2C_Serial() or 
// I2C_Serial(0)  : _address=0x48; : a0/a1=+/+ : default I2C serial port
// I2C_Serial(1)  : _address=0x49; : a0/a1=-/+ : secondary I2C serial port
// I2C_Serial(2)  : _address=0x4c; : a0/a1=+/- : RFID ID-2 sensor board
// I2C_Serial(12) : _address=0x4d; : a0/a1=-/- : RFID ID-12 sensor board


class I2C_Serial : public Stream {
public:
  I2C_Serial(uint8_t b=0); // 2 for ID2 or 12 for ID12, default = 0, 1 otherwise
  String WhoAmI();
  void begin(long b=9600);
  boolean test();

  virtual size_t write(uint8_t byte);
  virtual int read();
  virtual int available();
  virtual void flush();

  virtual int peek(); // !
  int free();       // TX

private:
  int8_t _address;
};

#endif




