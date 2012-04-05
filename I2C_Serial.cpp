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
//

#include "Wire.h"
#include "I2C_Serial.h"
#include "Stream.h"

// ---------------- Functions

// Writes value to address register on device
static void _writeTo(uint8_t device, uint8_t address, uint8_t value) {
  Wire.beginTransmission(device); // start transmission to device 
  Wire.write(address);             // send register address
  Wire.write(value);               // send value to write
  Wire.endTransmission();         // end transmission
}


// Reads number uint8_ts starting from address register on device uint8_to buffer array
static void _readFrom(uint8_t device, uint8_t address, uint8_t number, uint8_t buffer[]) {
  Wire.beginTransmission(device); // start transmission to device 
  Wire.write(address);             // sends address to read from
  Wire.endTransmission();         // end transmission

  Wire.beginTransmission(device);    // start transmission to device
  Wire.requestFrom(device, number);  // request number uint8_ts from device

  uint8_t i = 0;
  while(Wire.available())       // device may send less than requested (abnormal)
  { 
    buffer[i] = Wire.read(); // receive a uint8_t
    i++;
  }
  Wire.endTransmission();       //end transmission
}



static uint8_t _readByteFrom(int8_t device, uint8_t address) {
  Wire.beginTransmission(device); // start transmission to device  
  Wire.write(address);             // send address to read from
  Wire.endTransmission();         // end transmission

  Wire.beginTransmission(device); // start transmission to device
  Wire.requestFrom(device, 1);    // request 1 uint8_t from device

  while(!Wire.available());      // device may send less than requested (abnormal)
  uint8_t b = Wire.read();    // receive a uint8_t
  Wire.endTransmission();        // end transmission
  return b;
}

// ---------------- Class

I2C_Serial::I2C_Serial(uint8_t n) // constructor
{
  _address=0x48;                  // 0x90 a0/a1=+/+ : default I2C serial port
  if (n==1) _address=0x49;        // 0x92 a0/a1=+/- : secondary I2C serial port
  else if (n==2) _address=0x4c;   // 0x98 a0/a1=-/+ : RFID ID-2 sensor board
  else if (n==12) _address=0x4d;  // 0x9a a0/a1=-/- : RFID ID-12 sensor board
}

String I2C_Serial::WhoAmI() {
  String s="";
  if (_address < 0x10) s="0";
  s = s + String(_address, 0x10) +"h ";
  s = s + "SC16IS750 I2C Serial (actuator/sensor)";
  return s;
}


void I2C_Serial::begin(long b) {

  _writeTo(_address, 0x0e << 3, 0x01 << 3); // software reset

  // See datasheet section 7.8 for configuring the
  // "Programmable baud rate generator"
  uint16_t divisor = (uint16_t)(14745600/b/16);

  _writeTo(_address, 0x03 << 3, 0b10000000); // Line Control Register - divisor latch enable
  _writeTo(_address, 0x00 << 3, lowByte(divisor));  // Divisor Latch LSB
  _writeTo(_address, 0x01 << 3, highByte(divisor)); // Divisor Latch MSB


  //  _writeTo(_address, 0x03 << 3, 0xbf); // Line Control Register - access EFR register
  //  _writeTo(_address, 0x02 << 3, 0x00); // enable enhanced registers
  _writeTo(_address, 0x03 << 3, 0b00000011); // Line Control Register - 8 data bit, 1 stop bits, no parity
  _writeTo(_address, 0x02 << 3, 0x06); // reset TXFIFO, reset RXFIFO, non FIFO mode
  _writeTo(_address, 0x02 << 3, 0x01); // enable FIFO mode   
}


size_t I2C_Serial::write(uint8_t byte) {
  //  //                                   start I2C    
  //  Wire.beginTransmission(_address); // talk to device at _address
  //  Wire.write(0x00);                   // command 
  //  Wire.write(i);                     // value  
  //  Wire.endTransmission();           // end I2C  
  _writeTo(_address, 0x00 << 3, byte);
}


int I2C_Serial::read() {
  //  Wire.beginTransmission(_address); //start transmission to ACC 
  //  Wire.write(0x00);                   // command 
  //  Wire.endTransmission(); //end transmission
  //
  //    Wire.beginTransmission(_address); //start transmission to ACC
  //  Wire.requestFrom(_address, 1);    // request 6 uint8_ts from ACC
  //
  //  while (!Wire.available())    //ACC may send less than requested (abnormal)
  //    return  Wire.read(); // receive a uint8_t
  //  Wire.endTransmission(); //end transmission

  return _readByteFrom(_address, 0x00 << 3);
}


boolean I2C_Serial::test() {
  char a = (char)random(0x00, 0xff);
  _writeTo(_address, 0x07 << 3, a); // Scratch Pad Register
  delay(3);
  char b=_readByteFrom(_address, 0x07 << 3); // Scratch Pad Register
  return (a==b);
}


int I2C_Serial::available() {
  return _readByteFrom(_address, 0x09 << 3); // Receiver FIFO Level register
}

int I2C_Serial::peek() {
  if (available()==0) return -1;
  else return 1; // ?
}

int I2C_Serial::free() {
  return _readByteFrom(_address, 0x08 << 3); // Transmitter FIFO Level register
}


void I2C_Serial::flush() {
  _writeTo(_address, 0x02 << 3, 0x06); // reset TXFIFO, reset RXFIFO, non FIFO mode
  _writeTo(_address, 0x02 << 3, 0x01); // enable FIFO mode   
}
