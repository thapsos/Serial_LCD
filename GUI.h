// 
// 4D Systems μLCD-μLED-μVGA Serial_LCD Library Suite
// Arduino 0023 chipKIT MPIDE 0023 Library
// ----------------------------------
//
// Jan 28, 2012 release 108
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
#define GUI_RELEASE 108

#include "WProgram.h"
#include "Serial_LCD.h"

// Test release
#if SERIAL_LCD_RELEASE < 118
#error required SERIAL_LCD_RELEASE 118
#endif

#ifndef GUI_h
#define GUI_h


struct item {
  uint16_t index;
  char label[16];
};

item setItem(uint8_t index0, String text0);

class button {
public:
  button();

  // item-based button
  void  define(Serial_LCD * lcd0, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, item item0, uint16_t textColour1, uint16_t highColour1);
  void dDefine(Serial_LCD * lcd0, uint16_t x0, uint16_t y0, uint16_t dx, uint16_t dy, item item0, uint16_t textColour1, uint16_t highColour1);
  void  define(Serial_LCD * lcd0, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, item item0, uint16_t textColour1, uint16_t highColour1, uint16_t lowColour1, uint8_t size0);
  void dDefine(Serial_LCD * lcd0, uint16_t x0, uint16_t y0, uint16_t dx, uint16_t dy, item item0, uint16_t textColour1, uint16_t highColour1, uint16_t lowColour1, uint8_t size0);

  // string-based button
  void  stringDefine(Serial_LCD * lcd0, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, String text0, uint16_t textColour1, uint16_t highColour1);
  void dStringDefine(Serial_LCD * lcd0, uint16_t x0, uint16_t y0, uint16_t dx, uint16_t dy, String text0, uint16_t textColour1, uint16_t highColour1);
  void  stringDefine(Serial_LCD * lcd0, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, String text0, uint16_t textColour1, uint16_t highColour1, uint16_t lowColour1, uint8_t size0);
  void dStringDefine(Serial_LCD * lcd0, uint16_t x0, uint16_t y0, uint16_t dx, uint16_t dy, String text0, uint16_t textColour1, uint16_t highColour1, uint16_t lowColour1, uint8_t size0);

  boolean state();
  void draw(boolean b1=false);
  void enable(boolean b1); 
  boolean check(boolean instant=false);
  uint16_t getIndex();

private:
  uint16_t _x1, _y1, _x2, _y2, _xt, _yt;
  uint16_t _textColour, _highColour, _lowColour;
  boolean _enable;  
  String _text;
  uint16_t _index;
  uint8_t _size;
  Serial_LCD * _pscreen;
};

// kind0 = icon
// default = green + ; 1 = blue ! ; 2 = yellow ? ; 3 = red X
// option: button2
// option: button2, button3
uint8_t dialog(Serial_LCD * lcd0, String text0, uint8_t kind0, uint16_t textColour0, uint16_t highColour0, uint16_t lowColour0, String text1, String button1, uint16_t textColour1, uint16_t highColour1, uint16_t lowColour1, String text2="\0", String button2="\0", uint16_t textColour2=0, uint16_t highColour2=0, uint16_t lowColour2=0, String text3="\0", String button3="\0", uint16_t textColour3=0, uint16_t highColour3=0, uint16_t lowColour3=0);

// menu
uint16_t menu(Serial_LCD * lcd0, item menuItem0[], uint8_t nItems0, uint16_t textColour0, uint16_t highColourMain0, uint16_t highColourSub0);

// label
void dLabel(Serial_LCD * lcd0, uint16_t x0, uint16_t y0, uint16_t dx, uint16_t dy, String text0, uint16_t textColour0, uint16_t backColour0, uint8_t horizontal0, uint8_t vertical0, uint8_t size0);
void  label(Serial_LCD * lcd0, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, String text0, uint16_t textColour0, uint16_t backColour0, uint8_t horizontal0, uint8_t vertical0, uint8_t size0);

#endif











