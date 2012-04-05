// 
// 4D Systems μLCD-μLED-μVGA Serial_LCD Library Suite
// Arduino 1.0 Library
// ----------------------------------
// Developped with embedXcode
//
// Mar 19, 2012 release 202
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
#define GALLERY_RELEASE 202

#include "Arduino.h"
#include "Serial_LCD.h"
#include "vector.h"

// Test release
#if SERIAL_LCD_RELEASE < 223
#error required SERIAL_LCD_RELEASE 223
#endif


#ifndef Gallery_h
#define Gallery_h


class Gallery {
public:
    Gallery(); // constructor
    ~Gallery(); // destructor
    uint8_t begin(Serial_LCD * lcd0, String name);
    uint8_t number();
    uint8_t index();
    uint8_t showNext();
    uint8_t showPrevious();
    uint8_t showImage(uint8_t i=0);
    
private:
    struct image_t {
        //    String name;
        uint16_t msb, lsb, x, y;
    };
    
    Serial_LCD * _pscreen;
    String _name;
    uint8_t _index;
    Vector <image_t> _gallery;
};


#endif
