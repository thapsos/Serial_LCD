// 
// 4D Systems μLCD-μLED-μVGA Serial_LCD Library Suite
// Arduino 0023 chipKIT MPIDE 0023 Library
// ----------------------------------
// Developped with embedXcode
//
// Mar 13, 2012 release 101
// see README.txt
//
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
#define GALLERY_RELEASE 101

#include "WProgram.h"
#include "Serial_LCD.h"
#include "vector.h"

// Test release
#if SERIAL_LCD_RELEASE < 122
#error required SERIAL_LCD_RELEASE 122
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
