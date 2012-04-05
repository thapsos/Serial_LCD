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

#include "Serial_LCD.h"
#include "Gallery.h"
#include "vector.h"

// Utilities
//
uint16_t stoh(String in) {
    char c;
    uint16_t w = 0;
    in.trim(); // in-place string function
    
    for (uint8_t i=0; i<in.length(); i++) {
        c = in.charAt(i);
        w <<= 4; 
        if ((c>='0') && (c<='9')) w |= (c-'0');
        else if ((c>='A') && (c<='F')) w |= (c-'A'+0x0a);
        else if ((c>='a') && (c<='f')) w |= (c-'a'+0x0a);
        else;
    }
    return w;
}

uint16_t stod(String in) {
    char c;
    uint16_t w = 0;
    in.trim(); // in-place string function
    
    for (uint8_t i=0; i<in.length(); i++) {
        c = in.charAt(i);
        w *= 10; 
        if ((c>='0') && (c<='9')) w |= (c-'0');
        else;
    }
    return w;
}

uint8_t splitString(String in, char delimiter, String s[], uint8_t max) {
    uint8_t j=0;
    char c;
    s[0]='\0';
    
    for (uint8_t i=0; i<in.length(); i++) {
        c = in.charAt(i);
        if ( c=='\0' ) return j;
        
        else if ( c==delimiter ) {
            j++;
            if (j+1>max) return j; 
            s[j]='\0';
        } 
        else s[j] += c;
    }
} 

// Class
//
Gallery::Gallery() { // constructor
}

Gallery::~Gallery() { // destructor
}

uint8_t Gallery::begin(Serial_LCD * lcd0, String name) {
    uint8_t a;
    _pscreen = lcd0;
    String text;
    String s[5];
    _name = name;
    
    // check SD-card
    if ( _pscreen->checkSD()==false ) {
        a = _pscreen->initSD();
        if ( _pscreen->checkSD()==false ) return 0;        
    }
    
    // check .GCI and .DAT files
    a = _pscreen->findFile(_name + ".gci");
    if (a==0x15) return 0;
    
    a = _pscreen->findFile(_name + ".dat");
    if (a==0x15) return 0;
    
    a = _pscreen->openTextFileDelimiter(_name + ".dat", '\n');
    if (a==0x15) return 0;
    
    while (_pscreen->nextTextFileDelimiter(&text)) {
        // clean and split
        text.trim(); // in-place string function
        splitString(text, ' ', s, 5);
        
        // store
        image_t _image;
        _gallery.push_back(_image);
        _gallery[_gallery.size()-1].msb = stoh(s[2]);
        _gallery[_gallery.size()-1].lsb = stoh(s[1]);
        _gallery[_gallery.size()-1].x = stoh(s[3]);
        _gallery[_gallery.size()-1].y = stoh(s[4]);
    }    
    _index = 0;
    return _gallery.size();
}

uint8_t Gallery::number() {
    return _gallery.size();
}

uint8_t Gallery::index() {
    return _index;
}

uint8_t Gallery::showNext() {
    _index ++;
    _index %= _gallery.size();
    
    return showImage(_index);
}

uint8_t Gallery::showPrevious() {
    if ( _index=0 ) _index=_gallery.size();
    else _index--;
    
    return showImage(_index);
}

uint8_t Gallery::showImage(uint8_t i) {
    _index %= _gallery.size();
    if ( (i<_gallery.size()) ) {
        return _pscreen->readScreenGCI(Gallery::_name+".gci", _gallery[i].x, _gallery[i].y, _gallery[i].msb, _gallery[i].lsb); 
    }
    else return 0x15;
}









