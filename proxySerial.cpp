// 
// 4D Systems μLCD-μLED-μVGA Serial_LCD Library Suite
// Arduino 1.0 Library
//
// Mar 19, 2012 release 207
// see README.txt
//
// © Rei VILO, 2010-2012
// CC = BY NC SA
// http://sites.google.com/site/vilorei/
// https://sites.google.com/site/vilorei/arduino/13--serial-touch-320x240-lcd-screen
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

#include "Stream.h"
#include "proxySerial.h"

// Utilities

String ftoa(float number, uint8_t precision, uint8_t size) {
    // Based on mem,  16.07.2008
    // http://www.arduino.cc/cgi-bin/yabb2/YaBB.pl?num = 1207226548/6#6
    
    // prints val with number of decimal places determine by precision
    // precision is a number from 0 to 6 indicating the desired decimial places
    // example: printDouble(3.1415, 2); // prints 3.14 (two decimal places)
    
    // Added rounding, size and overflow #
    // ftoa(343.1453, 2, 10) -> "    343.15"
    // ftoa(343.1453, 4,  7) -> "#      "
    // avenue33, April 10th, 2010
    
    String s = "";
    
    // Negative 
    if (number < 0.0)  {
        s = "-";
        number = -number;
    }
    
    float rounding = 0.5;
    for (uint8_t i = 0; i < precision; ++i)    rounding /= 10.0;
    
    number += rounding;
    s += String(int(number));  // prints the integer part
    
    if(precision > 0) {
        s += ".";                // prints the decimal point
        uint32_t frac;
        uint32_t mult = 1;
        uint8_t padding = precision -1;
        while(precision--)     mult *= 10;
        
        frac = (number - (uint64_t)(number)) * mult;
        
        uint32_t frac1 = frac;
        while(frac1 /= 10)    padding--;
        while(padding--)      s += "0";
        
        s += String(frac,DEC) ;  // prints the fractional part
    }
    
    if ( (size>0) & (s.length()>size) )  return("#");
    while( s.length()<size ) s = " "+s;
    
    return s;
}


String ttoa(float number, uint8_t precision, uint8_t size) {
    // Automatic selection of the time unit: ms, s, mn, h
    if (number<1000) {
        // ms
        return ftoa(number, 0, max(0, size-2)) + "ms";
    }
    else if (number<60000.0) {
        // s = ms/1000
        return ftoa(number*0.001, precision, max(0, size-1)) + "s";
    }
    else if (number<3600000.0) {
        // mn = ms/1000/60
        return ftoa(number*0.001/60.0, precision, max(0, size-2)) + "mn";
    }
    else {
        // h = ms/1000/60/60
        return ftoa(number*0.001/60.0/60.0, precision, max(0, size-1)) + "h";
    }
}


String htoa(uint32_t number, uint8_t size) {
    String s = "";
    while ( number>0 ) {
        s = String("0123456789abcdef")[number%16] + s;
        number = number>>4;
    }
    if ( (size>0) && (s.length()>size) )  return("#");
    
    while ( s.length()<size )  s = "0"+s;
    return s;
}

// Object

ProxySerial::ProxySerial(Stream * port0) {
    _proxyPort = port0; 
}


void ProxySerial::_checkSpeed() {  
    //    while(!(millis()-_millis > 3));    // time in ms
    while((millis()-_millis < securityDelay));    // time in ms, the same !> = <
    _millis=millis();
}

void ProxySerial::begin(uint16_t b) {  // to be managed at serial port level
    _checkSpeed();  
    //  _proxyPort->begin(b); 
}

void ProxySerial::setXY16(boolean b) { 
    // true =  16-bits coordinates for Picaso
    // false =  8-bits coordinates for Goldelox
    _XY16 = b;
}

void ProxySerial::print(int8_t i) { 
    _checkSpeed();  
    _proxyPort->write(i); 
}

void ProxySerial::print(uint8_t ui) { 
    _checkSpeed();  
    _proxyPort->write(ui); 
};

void ProxySerial::print(int16_t i) { 
    _checkSpeed();  
    _proxyPort->write(highByte(i)); 
    _proxyPort->write(lowByte(i)); 
    
};

void ProxySerial::print(uint16_t ui) { 
    _checkSpeed();  
    _proxyPort->write(highByte(ui)); 
    _proxyPort->write(lowByte(ui)); 
};

void ProxySerial::printXY(int16_t i) { 
    _checkSpeed(); 
    if ( _XY16 )   _proxyPort->write(highByte(i)); 
    _proxyPort->write(lowByte(i)); 
};

void ProxySerial::printXY(uint16_t ui) { 
    _checkSpeed();  
    if ( _XY16 ) _proxyPort->write(highByte(ui)); 
    _proxyPort->write(lowByte(ui)); 
};

void ProxySerial::print(char c) { 
    _proxyPort->write((uint8_t)c); 
};
void ProxySerial::print(String s) { 
    for (uint8_t i=0; i<s.length(); i++)         {
        _checkSpeed();  
        _proxyPort->write(s.charAt(i));  }
}

uint8_t ProxySerial::read() { 
    _checkSpeed();  
    return _proxyPort->read(); 
}
int8_t ProxySerial::available() { 
    _checkSpeed();  
    return _proxyPort->available(); 
}
void ProxySerial::flush() {  
    _checkSpeed();  
    _proxyPort->flush(); 
}















