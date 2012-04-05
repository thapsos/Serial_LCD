// 
// 4D Systems μLCD-μLED-μVGA Serial_LCD Library Suite
// Arduino 1.0 Library
//
// Mar 19, 2012 release 223
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

#include "proxySerial.h"
#include "Serial_LCD.h"


// Constructor
Serial_LCD::Serial_LCD(ProxySerial * port0) {
    _port=port0;
};


// Interface
// 2.1 General Commands
// AutoBaud – 55hex 
void Serial_LCD::begin(int8_t resetPin0) {
    // default speed = 9600
    _resetPin = resetPin0;
    
    // reset is required for Goldelox-based screens
    if (_resetPin>-1) {
        delay(3000);
        pinMode(_resetPin, OUTPUT);     
        digitalWrite(_resetPin, LOW);
        delay(10);
        digitalWrite(_resetPin, HIGH);
        delay(10);
        _port->flush();
    }
    
    // LCD 500 ms power-up
    // SD card 3000 ms power-up
    delay(3000);
    _port->print('U');    // connect
    while (_port->read()!=0x06)  {     
        delay(100);  
    }
    
    _port->print('o');    // clear touch 
    _port->print((char)0x04);   // touch state
                                //  _port->flush();
    delay(100);
    while (_port->available()) _port->read();
    
    setDisplay(true);    // display on
    delay(100);
    
    // screen size based on orientation
    // screen type
    _checkedSD = false;  // SD not checked
    WhoAmI();
    
    // screen type required for setBacklight and following parameters 
    setBacklight(true);  // backlight on
    setOrientation(3);
    
    clear();
    setFont(1);
}


uint8_t Serial_LCD::replaceBackGroundColour(uint16_t colour) {   
    _port->print('B');
    _port->print((uint16_t)colour);
    
    return nacAck();
}


uint8_t Serial_LCD::setResolutionVGA(uint8_t b) {
    if (_checkedScreenType!=2) return 0x15;   // VGA only
    
    _port->print('Y');
    _port->print((uint8_t)0x0c);
    _port->print((uint8_t)b);
    return nacAck();
}   


uint8_t Serial_LCD::setSpeed(uint16_t speed) {
    uint8_t a=0x06;
    if      (speed==(uint16_t)  9600) a=0x06;
    else if (speed==(uint16_t) 19200) a=0x08;
    else if (speed==(uint16_t) 28800) a=0x09;
    else if (speed==(uint16_t) 38400) a=0x0a;  // max for Arduino
    else if (speed==(uint16_t) 57600) a=0x0c;
    else if (speed==(uint16_t)115200) a=0x0d;  // ok with chipKIT
    else return 0x15;
    
    if (a != 0x06) {
        _port->print('Q');
        _port->print((uint8_t)a); 
        while (!_port->available());
        a=_port->read();     
    }
    a=0x06;
    return a;
}


uint16_t _size(uint8_t ui, uint8_t answer) {
    switch (ui) {
        case 0x22 :         return 220;  // return doesn't need break
        case 0x28 :         return 128;
        case 0x60 :         return 160; 
        case 0x64 :         return 64; 
        case 0x76 :         return 176; 
        case 0x96 :         return 96; 
        case 0x32 :         return 320;
        case 0x24 :         return 240;
        case 0xff :     
            if (answer==3)  return 480;  // assumed, to be checked
            if (answer==4)  return 272;  // assumed, to be checked
        default   :         return 0;
    }
}

String Serial_LCD::WhoAmI() {  
    String s="Serial 4D Labs screen ";
    _port->print('V');
    _port->print((char)0x00);
    delay(10);
    
    uint8_t i=0;
    uint8_t c=0;
    while(_port->available()!=0) {
        c=_port->read();
        s += String(c, HEX);
        s += " ";
        
        if ( i==0 )  {
            _checkedScreenType = c; // 8-bits uLED=0, 16_bits uLCD=1, 16_bits uVGA=2
            _port->setXY16( _checkedScreenType>0 );
        }
        if ( i==1 )  _checkedHardwareVersion = c;
        if ( i==2 )  _checkedSoftwareVersion = c;
        if ( i==3 )  _maxX = _size(c, 3); // standard
        if ( i==4 )  _maxY = _size(c, 4); // standard
        
        i++;
    }
    return s;
}


uint8_t Serial_LCD::clear() {
    _port->print('E');
    return nacAck();
}

void Serial_LCD::off() {
    clear();
    //  _port->print('o');
    //  _port->print((char)0x04);   // state
    //  _port->flush();
    
    setBacklight(false); // backlight off
    setDisplay(false);   // display off
    
    while (_port->available()) _port->read();
    _port->print('Q');    // reset to default speed
    _port->print((char)0x06);    // 
    delay(10);
}


uint8_t Serial_LCD::setBacklight(boolean b) {
    if (_checkedScreenType!=1) return 0x15;   // only uLCD 
    
    _port->print('Y');
    _port->print((char)0x00);
    _port->print((b) ? (char)0x01 : (char)0x00);
    return nacAck();
}   

uint8_t Serial_LCD::setDisplay(boolean b) {
    if (_checkedScreenType==2) return 0x15;   // exclude VGA 
    
    _port->print('Y');
    _port->print((char)0x01);
    _port->print(b ? (char)0x01 : (char)0x00);
    return nacAck();
}

uint8_t Serial_LCD::setContrast(uint8_t b) {
    if (_checkedScreenType==2) return 0x15;   // exclude VGA 
    
    if (b<=0x0f) {
        _port->print('Y');
        _port->print((char)0x02);
        _port->print(b);
        
        return nacAck();
    } 
    else {
        return 0x15; 
    }
}


uint8_t Serial_LCD::setOrientation(uint8_t b) {   // Display Control Functions – 59hex
    if (_checkedScreenType!=1) return 0x15;   // exclude VGA 
    
    _orientation = b;
    _port->print('Y');
    _port->print((char)0x04);
    _port->print(b);  // 
    return nacAck();
}


uint8_t Serial_LCD::getOrientation() { 
    return _orientation; 
} 

uint8_t Serial_LCD::setTouch(boolean b) {
    if (_checkedScreenType!=1) return 0x15;   // only uLCD 
    
    if (b) {
        _port->print('Y');
        _port->print((char)0x05);
        _port->print((char)0x00);  // enable touch
        char c=nacAck();
        
        if (c=0x06) {
            _port->print('Y');
            _port->print((char)0x05);  // full screen active
            _port->print((char)0x00);
        }
    } 
    else {
        _port->print('Y');
        _port->print((char)0x05);  // disable touch
        _port->print((char)0x01);
    }
    return nacAck();
}

uint8_t Serial_LCD::setVolume(uint8_t percent) { // Set Volume - 76hex 
    uint8_t b=0x08 + percent;
    if (b>0x7f) b=0x7f;
    _port->print('v');
    _port->print(b);
    
    return nacAck();
}

// Graphics
uint8_t Serial_LCD::circle(uint16_t x1, uint16_t y1, uint16_t radius, uint16_t colour) {
    _port->print('C');
    
    _port->printXY(x1);
    _port->printXY(y1);
    _port->printXY(radius);
    _port->print(colour);
    
    return nacAck();
}


uint8_t Serial_LCD::dRectangle(uint16_t x0, uint16_t y0, uint16_t dx, uint16_t dy, uint16_t colour) {
    return rectangle(x0, y0, x0+dx-1, y0+dy-1, colour);
}

uint8_t Serial_LCD::rectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t colour) {
    _port->print('r');
    
    _port->printXY(x1);
    _port->printXY(y1);
    _port->printXY(x2);
    _port->printXY(y2);
    _port->print(colour);
    
    return nacAck();
}  

uint8_t Serial_LCD::ellipse(uint16_t x, uint16_t y, uint16_t rx, uint16_t ry, uint16_t colour) {
    _port->print('e');
    
    _port->printXY(x);
    _port->printXY(y);
    _port->printXY(rx);
    _port->printXY(ry);
    _port->print(colour);
    
    return nacAck();
}  

uint8_t Serial_LCD::dLine(uint16_t x0, uint16_t y0, uint16_t dx, uint16_t dy, uint16_t colour) {
    return line(x0, y0, x0+dx-1, y0+dy-1, colour);
}

uint8_t Serial_LCD::line(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t colour) {
    _port->print('L');
    
    _port->printXY((uint16_t)x1);
    _port->printXY((uint16_t)y1);
    _port->printXY((uint16_t)x2);
    _port->printXY((uint16_t)y2);
    _port->print((uint16_t)colour);
    
    return nacAck();
}  

// 2011-06-24 release 3 - setBackGroundColour added
uint8_t Serial_LCD::setBackGroundColour(uint16_t colour) { 
    _port->print('K');
    
    _port->print(colour);
    
    return nacAck();
}


uint8_t Serial_LCD::point(uint16_t x1, uint16_t y1, uint16_t colour) {
    _port->print('P');
    
    _port->printXY(x1);
    _port->printXY(y1);
    _port->print(colour);
    
    return nacAck();
}  


uint16_t Serial_LCD::readPixel(uint16_t x1, uint16_t y1) {
    _port->print('R');
    
    _port->printXY(x1);
    _port->printXY(y1);
    
    while (_port->available()<2);
    uint16_t c=0;
    c = _port->read() << 8;
    c += _port->read();
    return c;
}


uint8_t Serial_LCD::copyPaste(uint16_t xs, uint16_t ys, uint16_t xt, uint16_t yt, uint16_t dx, uint16_t dy) {
    if ( (xt+dx>maxX()) || (yt+dy>maxY()) ) return 0x15;
    
    
    _port->print('c');
    
    _port->printXY(xs);
    _port->printXY(ys);
    _port->printXY(xt);
    _port->printXY(yt);
    _port->printXY(dx);
    _port->printXY(dy);
    
    return nacAck();
}


uint8_t Serial_LCD::triangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3, uint16_t colour) {
    boolean b=true;
    
    // Graham Scan + Andrew's Monotone Chain Algorithm
    // 1. Sort by ascending x
    
    while (b) {  // required x2 < x1 : x3 > x2 : y2 > y1 : y3 > y1
        b=false;
        if (!b && (x1>x2)) { 
            _swap(x1, x2);
            _swap(y1, y2);
            b=true; 
        }
        if (!b && (x2>x3)) { 
            _swap(x3, x2);
            _swap(y3, y2);
            b=true; 
        }
    }
    
    // Graham Scan + Andrew's Monotone Chain Algorithm
    // 2. Sort by ascending y
    while (b) {  // required x2 < x1 : x3 > x2 : y2 > y1 : y3 > y1
        if (!b && (y1>y2)) { 
            _swap(x1, x2);
            _swap(y1, y2);
            b=true; 
        }
        if (!b && (y3>y2)) { 
            _swap(x3, x2);
            _swap(y3, y2);
            b=true; 
        }
    }
    
    // Graham Scan + Andrew's Monotone Chain Algorithm
    // 3. check counter-clockwise, clockwise, collinear
    long l= (x2 - x1)*(y3 - y1) - (y2 - y1)*(x3 - x1);
    
    if (l==0)   return line(x1, y1, x3, y3, colour);
    
    if (l>0) {
        _swap(x1, x2);
        _swap(y1, y2);
    }
    l= (x2 - x1)*(y3 - y1) - (y2 - y1)*(x3 - x1);
    
    _port->print('G');
    
    _port->printXY(x1);
    _port->printXY(y1);
    _port->printXY(x2);
    _port->printXY(y2);
    _port->printXY(x3);
    _port->printXY(y3);
    _port->print(colour);
    
    return nacAck();
}  



uint8_t Serial_LCD::setPenSolid(boolean b) {
    // 00hex : All graphics objects are drawn solid 
    // 01hex : All graphics objects are drawn wire-frame
    _port->print('p');
    _port->print(b ? (char)0x00 : (char)0x01);
    return nacAck();
}  


// Text
uint8_t Serial_LCD::setFont(uint8_t b) {
    // 00hex : 6x8 (5x7 false) small size font set 
    // 01hex : 8x8 medium size font set 
    // 02hex : 8x12 large size font set
    // 03hex : 12x16 largest size font set - not on uLED
    
    if (b>3) b=3;
    if ( (b>2) && (_checkedScreenType==0) ) b=2;
    
    _port->print('F');
    _port->print(b);
    _font=b;
    switch (b) {
        case 0:
            _fontX=6; 
            _fontY=8;
            break;
        case 1:
            _fontX=8; 
            _fontY=8;
            break;
        case 2:
            _fontX=8; 
            _fontY=12;
            break;
        case 3:
            _fontX=12; 
            _fontY=16;
    } 
    return nacAck();
}  

uint8_t Serial_LCD::setFontSolid(boolean b) {
    // 00hex : Transparent, objects behind text are visible. 
    // 01hex : Opaque, objects behind text blocked by background.
    _port->print('O');
    _port->print(b ? (char)0x01 : (char)0x00);
    return nacAck();
}

uint8_t Serial_LCD::tText(uint8_t x, uint8_t y, String s, uint16_t colour) {
    _port->print('s');
    _port->print(x);     // in character units
    _port->print(y);
    _port->print(_font);
    _port->print(colour);
    _port->print(s);
    _port->print((char)0x00);
    return nacAck();
}


uint8_t Serial_LCD::gText(uint16_t x, uint16_t y, String s, uint16_t colour) {
    _port->print('S');
    _port->printXY(x);    // in graphic units
    _port->printXY(y);
    _port->print(_font);
    _port->print(colour);
    _port->print((char)0x01);   // multiplier
    _port->print((char)0x01);
    _port->print(s);
    _port->print((char)0x00);
    return nacAck();
}


// Touch
//   return value 
//     0 : No Touch Activity 
//     1 : Touch Press 
//     2 : Touch Release 
//     3 : Touch Moving
uint8_t Serial_LCD::getTouchActivity() {
    if (_checkedScreenType!=1) return 0x15;   // only uLCD 
    
    _port->print('o');
    _port->print((char)0x04);   // state
    
    uint8_t i=0; 
    delay(10);    // delay required by chipKIT
    while ((_port->available()) && (i<4)) {
        _touch_buffer[i]=_port->read();
        i++;
    }
    
    switch (_touch_buffer[1]) {
        case 1:
        case 2:
        case 3:
            return _touch_buffer[1];
        default:
            return false;
    }
}

uint8_t Serial_LCD::getTouchXY(uint16_t &x, uint16_t &y) {
    if (_checkedScreenType!=1) return 0x15;   // only uLCD 
    
    _port->print('o');
    _port->print((char)0x05);   // coordinates
    
    uint8_t i=0; 
    delay(10);    // delay required by chipKIT
    while (_port->available() && (i<4)) {
        _touch_buffer[i]=_port->read();
        i++;
    }
    
    if (_touch_buffer[0]!=0x15) {
        x = (uint16_t)(_touch_buffer[0] << 8) + _touch_buffer[1];
        y = (uint16_t)(_touch_buffer[2] << 8) + _touch_buffer[3];
        return 0x06;
    } 
    else {
        return 0x15;
    }
}


uint8_t Serial_LCD::dDetectTouchRegion(uint16_t x0, uint16_t y0, uint16_t dx, uint16_t dy) {
    return Serial_LCD::detectTouchRegion(x0, y0, x0+dx-1, y0+dy-1);
}

// filters only events 1=press and 3=move
uint8_t Serial_LCD::detectTouchRegion(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2) {
    if (_checkedScreenType!=1) return 0x15;   // only uLCD 
    
    _port->print('u');
    _port->printXY(x1);
    _port->printXY(y1);
    _port->printXY(x2);
    _port->printXY(y2);
    
    return nacAck();
}

// Initialise Memory Card - @69hex 
// valid for FAT and RAW
// 2.5 SD Memory Card Commands (Low-Level/RAW)
// 2.6 SD Memory Card Commands (FAT16-Level/DOS)
uint8_t Serial_LCD::initSD() {
    uint8_t a;
    _checkedSD = false; 
    _checkedRAW = false; 
    _checkedFAT = false; 
    
    // SD card 3000 ms power-up
    delay(3000);
    _port->print('@');
    _port->print('i');   
    
    // answer = 210 ms
    delay(210);
    a = nacAck();
    
    if (a==0x06) {
        //    _checkedSD = true;
        //
        // Method 1 - Look for RAW.INI file
        //    if ( findFile("RAW.INI")==0x06 ) { 
        //      _checkedRAW = true;
        //      delay(1000);
        //    }
        // Method 2 - Set Address Pointer of Card (RAW) - @41hex
        //    _port->print('@');
        //    _port->print('A');
        //    _port->print((uint8_t)0);
        //    _port->print((uint8_t)0);
        //    _port->print((uint8_t)0);
        //    _port->print((uint8_t)0);
        //      if ( nacAck()==0x06 ) {
        //          _checkedRAW = true;
        //      }
        //      _port->flush(); // if no RAW, 5 times error message 0x15, only one read
        // Method 3 - specific function
        _port->print('z');
        a = _port->read();
        if (bitRead(a, 7)) _checkedSD = true;  // uSD card present
        if (bitRead(a, 6)) _checkedFAT = true; // FAT partition present
        if (bitRead(a, 5)) _checkedRAW = true; // RAW partition present
        
        //      if (bitRead(a, 4)) Serial.print("FAT partition is protected");Serial.print("\n");
        //      if (bitRead(a, 3)) Serial.print("New Image format Enabled");Serial.print("\n");
        
        // Back to initial result
        a = 0x06;
    }
    
    return a;
}


boolean Serial_LCD::checkSD() { 
    return _checkedSD; 
}
boolean Serial_LCD::checkRAW() { 
    return _checkedRAW; 
}
boolean Serial_LCD::checkFAT() { 
    return _checkedFAT; 
}

uint8_t Serial_LCD::protectFAT(boolean b) {
    if (_checkedScreenType==0) return 0x15;   // exclude uOLED  
    
    _port->print('Y');
    _port->print((char)0x08);
    _port->print((b) ? (char)0x01 : (char)0x00);
    return nacAck();
}

uint8_t Serial_LCD::checkScreenType() {
    return _checkedScreenType;
}
uint8_t Serial_LCD::checkHardwareVersion() {
    return _checkedHardwareVersion;
}
uint8_t Serial_LCD::checkSoftwareVersion() {
    return _checkedSoftwareVersion;
}

// 2.5 SD Memory Card Commands (Low-Level/RAW)
// Screen Copy-Save to Card (RAW) - @43hex
uint8_t Serial_LCD::saveScreenRAW(uint32_t sector) {
    return dSaveScreenRAW(sector, 0, 0, maxX(), maxY());
}

// Screen Copy-Save to Card (RAW) - @43hex
// x1, y1 x2, y2: same coordinates as rectangle
uint8_t Serial_LCD::saveScreenRAW(uint32_t sector, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2) {
    return dSaveScreenRAW(sector, x1, y1, x2-x1+1, y2-y1+1);
}

// x0, y0 and width and height  
uint8_t Serial_LCD::dSaveScreenRAW(uint32_t sector, uint16_t x0, uint16_t y0, uint16_t dx, uint16_t dy) {
    if ( !_checkedSD ) return 0x15;
    _port->print('@');
    _port->print('C');
    _port->printXY((uint16_t)x0);
    _port->printXY((uint16_t)y0);
    _port->printXY((uint16_t)dx);
    _port->printXY((uint16_t)dy);
    _port->print((uint8_t)((sector >> 8) & 0x00ff));  // high:
    _port->print((uint16_t)(sector & 0xffff));        // mid:low
    
    delay(10);
    return nacAck();
}


// Display Object from Card (RAW) - @4Fhex     
// x1, y1: left-top coordinates
// no coordinates: 0, 0 for full screen
uint8_t Serial_LCD::readScreenRAW(uint32_t sector, uint16_t x1, uint16_t y1) {
    if (_checkedScreenType==0) return 0x15;   //  exclude uOLED
    if ( !_checkedSD ) return 0x15;
    
    // RAW image new format
    _port->print('Y');
    _port->print((char)0x06);   
    _port->print((char)0x00);   
    nacAck();
    
    _port->print('@');
    _port->print('I');
    _port->printXY((uint16_t)x1);
    _port->printXY((uint16_t)y1);
    _port->print((uint8_t)((sector >> 8) & 0x00ff));  // high:
    _port->print((uint16_t)(sector & 0xffff));        // mid:low
    
    delay(10);
    return nacAck();
}


// 2.6 SD Memory Card Commands (FAT16-Level/DOS)
// Write File to Card (FAT) - @74hex 
// default option = 0
uint8_t Serial_LCD::writeStringFile(String filename, String text, uint8_t option) { 
    String s;
    uint8_t a;
    uint8_t j;
    
    if ( !_checkedSD ) return 0x15;
    
    j=text.length() >>4;
    
    // 16-uint8_t blocks
    if (j>0) {
        _port->print('@');
        _port->print('t');
        _port->print((char)(0x10 + option)); // hand-shaking
        _port->print(filename);
        _port->print((char)0x00);
        _port->print((uint16_t)0x00);
        _port->print((uint16_t)(j <<4));
        a=nacAck();
        
        for (uint8_t i=0; i<j; i++) {
            s=text.substring(i <<4, (i+1) <<4);
            _port->print(s);
            a=nacAck();
        }
    }
    
    // remaining bytes
    j=text.length() % 0x10;
    
    if   (j > 0) {
        _port->print('@');
        _port->print('t');
        _port->print((uint8_t)(0x00 + option));   // no hand-shaking
        _port->print(filename);
        _port->print((char)0x00);
        _port->print((uint16_t)0x00);
        _port->print((uint16_t)(j));
        a=nacAck();
        
        s=text.substring(text.length()-j, text.length());
        _port->print(s);
        a=nacAck();
    }
    return a;
}

uint8_t Serial_LCD::appendStringFile(String filename, String text) { 
    return writeStringFile(filename, text, 0x80);  // append option
}


//  Read File from Card (FAT) - @61hex 
//  
//  filename   - The filename of the file to read
//  bytes      - The number of bytes to get each time (0=all, not a good idea when filesize > 512b)
//  cbReadFile - Callback function that is called every time data is received so a parser do its job.
uint8_t Serial_LCD::readTextFile(String filename, uint8_t bytes, void (*cbReadFile)(String text)) {  
    if ( !_checkedSD ) return 0x15;
    
    _port->print('@');
    _port->print('a');
    _port->print((uint8_t)bytes);
    _port->print(filename);
    _port->print((uint8_t)0x00);    
    
    uint8_t i = 0;
    char c = 0;
    String s = "";
    boolean done = false;
    uint32_t fileSize = 0x00;
    
    while(!_port->available());
    for (uint8_t j=0; j<4; j++) {
        fileSize <<= 4;
        fileSize |= _port->read();
    }
    
    do {
        _port->print((char) 0x06);     
        
        i = 0;
        s = "";
        
        if ( _port->available() ) {
            do {
                c = _port->read();  
                if ( (c==0x06) || (fileSize==0) ) {
                    done = true;
                } 
                else {
                    s += c;
                    i++;
                    fileSize--;
                } 
            } 
            while ( _port->available() && (i<bytes) && !done);
        }
        
        cbReadFile(s);
    } 
    while ( !done );
    
    while (_port->available()) { 
        _port->read(); 
    };
    
    return c;
}


uint8_t Serial_LCD::readTextFileDelimiter(String filename, char delimiter, void (*cbReadFile)(String text)) {
    if ( !_checkedSD ) return 0x15;
    
    _port->print('@');
    _port->print('a');
    _port->print((uint8_t)0x10);
    _port->print(filename);
    _port->print((uint8_t) 0x00);    
    
    uint8_t i = 0;
    char c = 0;
    String s = "";
    boolean done = false;
    boolean up = false;
    uint32_t fileSize = 0x00;
    
    while(!_port->available());
    for (uint8_t j=0; j<4; j++) {
        fileSize <<= 4;
        fileSize |= _port->read();
    }
    
    do {
        _port->print((char) 0x06);     
        
        if (up) {
            i = 0;
            s = "";
            up = false;
        }
        if ( _port->available() ) {
            do {
                c = _port->read();  
                if ( (c==0x06) || (fileSize==0) ) {
                    done = true;
                } 
                else if ( c==delimiter ) {
                    up = true;
                } 
                else {
                    s += c;
                    i++;
                    fileSize--;
                }
            } 
            while ( _port->available() && (i<128) && !up && !done);
        }
        if (up) cbReadFile(s);
    } 
    while ( !done );
    
    while (_port->available()) { 
        _port->read(); 
    };
    
    return c;
}


uint8_t Serial_LCD::openTextFileDelimiter(String filename, char delimeter) {
    if ( !_checkedSD ) return 0x15;
    
    _delimeter = delimeter;
    
    _port->print('@');
    _port->print('a');
    _port->print((uint8_t)0x10);
    _port->print(filename);
    _port->print((uint8_t) 0x00);    
    
    while(!_port->available());
    for (uint8_t j=0; j<4; j++) {
        _fileSize <<= 4;
        _fileSize |= _port->read();
    }
    
    return 0x06;
}

boolean Serial_LCD::nextTextFileDelimiter(String * result) { // false = end
    uint8_t i = 0;
    char c = 0;
    String s = "";
    boolean done = false;
    boolean up = false;
    
    delay(10);
    
    do {
        if ( _port->available()==0 ) {
            _port->print((char) 0x06);     
        } 
        
        while (!_port->available());
        do {
            c = _port->read();  
            if ( (c==0x06) || (_fileSize==0) ) {
                done = true;
            } 
            else if ( c==_delimeter ) {
                up = true;
            } 
            else {
                if (c>0x1f) s += c;
                i++;
                _fileSize--;
            }
        } 
        while ( _port->available() && (i<128) && !up && !done);
    }
    while ( (i<128) && !up && !done);
    if (up || done) { 
        *result = s; 
    }
    if ( done) while (_port->available()) { 
        _port->read(); 
    };
    
    return !done; // return true to continue, false if EOF
}


// Erase file from Card (FAT) - @65hex 
uint8_t Serial_LCD::eraseFile(String filename) {  
    uint8_t a;
    if ( !_checkedSD ) return 0x15;
    
    _port->print('@');
    _port->print('e');
    _port->print(filename);
    _port->print((char)0x00);
    
    return nacAck();
}


// List Directory from Card (FAT) - @64hex
uint8_t Serial_LCD::findFile(String filename) {  
    if ( !_checkedSD ) return 0x15;
    
    _port->print('@');
    _port->print('d');
    _port->print(filename);
    _port->print((char)0x00);
    
    while (!_port->available());
    
    char c;
    String s="";  
    boolean flag = false;
    
    while (!flag) {
        if (_port->available()) {
            delay(2);
            c = _port->read();  
            flag = ( (c==0x06) || (c==0x15) || (c==0x0a) );
            if (!flag) s = s + String(c);
        }
    }
    while ( (c!=0x06) && (c!=0x15) ) c = _port->read();  
    
    if (s.length()==0) return 0x15;
    if (filename.equalsIgnoreCase(s.substring(0, (s.indexOf(c))))) return 0x06 ;
    return 0x15;
}


uint8_t Serial_LCD::saveScreenFAT(String filename) {    
    return dSaveScreenFAT(filename, 0, 0, maxX(), maxY());
}

// Screen Copy-Save to Card (FAT) - @63hex 
// x1, y1 x2, y2: same coordinates as rectangle
uint8_t Serial_LCD::saveScreenFAT(String filename, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2) {    
    return dSaveScreenFAT(filename, x1, y1, x2-x1+1, y2-y1+1);
}

uint8_t Serial_LCD::dSaveScreenFAT(String filename, uint16_t x0, uint16_t y0, uint16_t dx, uint16_t dy) {    
    uint8_t a;
    if ( !_checkedSD ) return 0x15;
    _port->print('@');
    _port->print('c');
    _port->printXY((uint16_t)x0);
    _port->printXY((uint16_t)y0);
    _port->printXY((uint16_t)dx);
    _port->printXY((uint16_t)dy);
    _port->print(filename);
    _port->print((char)0x00);
    
    return nacAck();
}


// Display Image-Icon from Card (FAT) - @6Dhex 
// x1, y1: left-top coordinates
// no coordinates: 0, 0 for full screen
uint8_t Serial_LCD::readScreenFAT(String filename, uint16_t x1, uint16_t y1) {   
    if ( !_checkedSD ) return 0x15;
    _port->print('@');
    _port->print('m');
    _port->print(filename);
    _port->print((char)0x00);
    _port->printXY((uint16_t)x1);
    _port->printXY((uint16_t)y1);
    _port->printXY((uint16_t)0x00);
    _port->printXY((uint16_t)0x00);
    
    return nacAck();
}


// Play Audio WAV file from Card (FAT) - @6Chex 
uint8_t Serial_LCD::playSoundSD(String filename, uint8_t option0) {   
    if ( !_checkedSD ) return 0x15;
    
    _port->print((uint8_t)0x6c);
    _port->print((uint8_t)option0);
    _port->print(filename);
    _port->print((char)0x00);
    
    return nacAck();
}

// Run Script (4DSL) Program from Card (FAT) - @70hex


// Utilities
uint8_t Serial_LCD::fontX() {     
    return _fontX;   
}	
uint8_t Serial_LCD::fontY() {     
    return _fontY;   
}	


uint16_t Serial_LCD::maxX() {
    if ((_orientation==1) || (_orientation==2)) return _maxY; // rotated
    else return _maxX;                  // _orientation==3 or 4, standard
}

uint16_t Serial_LCD::maxY() {
    if ((_orientation==1) || (_orientation==2)) return _maxX; // rotated
    else return _maxY;                  // _orientation==3 or 4, standard
}


uint32_t Serial_LCD::getSectors(uint16_t x, uint16_t y, uint16_t sizeSector) {
    uint32_t n = (uint32_t)(x*y*2+6);
    if (n % sizeSector > 0) n += sizeSector;
    n /= sizeSector;
    return n;
}

uint16_t Serial_LCD::setColour(uint8_t red8, uint8_t green8, uint8_t blue8) {
    // rgb16 = red5 green6 blue5
    return (red8 >> 3) << 11 | (green8 >> 2) << 5 | (blue8 >> 3);
}

void Serial_LCD::splitColour(uint16_t rgb, uint8_t &red, uint8_t &green, uint8_t &blue) {
    // rgb16 = red5 green6 blue5
    red   = (rgb & 0b1111100000000000) >> 11 << 3;
    green = (rgb & 0b0000011111100000) >>  5 << 2;
    blue  = (rgb & 0b0000000000011111)       << 3;
}

uint16_t Serial_LCD::halfColour(uint16_t rgb) {
    // rgb16 = red5 green6 blue5 
    return (rgb & 0b1111100000000000) >> 12 << 11 | \
    (rgb & 0b0000011111100000) >> 6 << 5 | \
    (rgb & 0b0000000000011111) >> 1;    
}

uint16_t Serial_LCD::reverseColour(uint16_t rgb) {
    // rgb16 = red5 green6 blue5 
    return (uint16_t)(rgb ^ 0xffff);
}



uint8_t Serial_LCD::nacAck() {
    uint8_t b=0x00; // 0x06;
    while (!(_port->available())) {     
        delay(2);   
    }
    b = _port->read();
    return b;
}


void Serial_LCD::_swap(uint16_t &a, uint16_t &b) {
    uint16_t w=a;
    a=b;
    b=w;
}


uint8_t Serial_LCD::readScreenGCI(String filename, uint16_t x1, uint16_t y1, uint16_t msb2, uint16_t lsb2) { // x1, y1: left-top coordinates; msb2:lsb2 icon sector address
    if ( !_checkedSD ) return 0x15;
    _port->print('@');
    _port->print('m');
    _port->print(filename);
    _port->print((char)0x00);
    _port->printXY((uint16_t)x1);
    _port->printXY((uint16_t)y1);
    _port->print((uint16_t)msb2);
    _port->print((uint16_t)lsb2);
    
    return nacAck();
}













