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

#include "WProgram.h"
#include "Serial_LCD.h"
#include "GUI.h"

item setItem(uint8_t index0, String text0) {
    item item0;
    item0.index = index0;
    text0.toCharArray(item0.label, 16);
    return item0;
}


button::button() {   
    ;
}

// string-based button
void button::dStringDefine(Serial_LCD * lcd0, uint16_t x0, uint16_t y0, uint16_t dx, uint16_t dy, String text0, uint16_t textColour1, uint16_t highColour1) {
    stringDefine( lcd0, x0, y0, x0+dx, y0+dy, text0, textColour1, highColour1, lcd0->halfColour(highColour1), 9);
}

void  button::stringDefine(Serial_LCD * lcd0, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, String text0, uint16_t textColour1, uint16_t highColour1) {
    stringDefine( lcd0, x1, y1, x2, y2, text0, textColour1, highColour1, lcd0->halfColour(highColour1), 9);
}

void button::dStringDefine(Serial_LCD * lcd0, uint16_t x0, uint16_t y0, uint16_t dx, uint16_t dy, String text0, uint16_t textColour1, uint16_t highColour1, uint16_t lowColour1, uint8_t size0) {
    stringDefine(lcd0, x0, y0, x0+dx, y0+dy, text0, textColour1, highColour1, lowColour1, size0);
}

void  button::stringDefine(Serial_LCD * lcd0, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, String text0, uint16_t textColour1, uint16_t highColour1, uint16_t lowColour1, uint8_t size0) {
    define(lcd0, x1, y1, x2, y2, setItem(0, text0), textColour1, highColour1, lowColour1, size0);
}

// item-based button
void button::define(Serial_LCD * lcd0, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, item item0, uint16_t textColour1, uint16_t highColour1) {
    define( lcd0, x1, y1, x2, y2, item0, textColour1, highColour1, _pscreen->halfColour(highColour1), 9);
}

void button::dDefine(Serial_LCD * lcd0, uint16_t x0, uint16_t y0, uint16_t dx, uint16_t dy, item item0, uint16_t textColour1, uint16_t highColour1) {
    dDefine( lcd0, x0, y0, dx, dy, item0, textColour1, highColour1, _pscreen->halfColour(highColour1), 9);
}

void button::dDefine(Serial_LCD  * lcd0, uint16_t x0, uint16_t y0, uint16_t dx, uint16_t dy, item item0, uint16_t textColour1, uint16_t highColour1, uint16_t lowColour1, uint8_t size0) {
    define(lcd0, x0, y0, x0+dx, y0+dy, item0, textColour1, highColour1, lowColour1, size0);
}

void button::define(Serial_LCD  * lcd0, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, item item0, uint16_t textColour1, uint16_t highColour1, uint16_t lowColour1, uint8_t size0) {
    _pscreen = lcd0;
    
    _x1 = x1;
    _y1 = y1;
    _x2 = x2;
    _y2 = y2;
    
    _text = item0.label;
    _index = item0.index;
    _textColour = textColour1;
    _highColour = highColour1;
    _lowColour  = lowColour1;
    _enable = false;  
    _size = size0;
}


boolean button::state() {     
    return _enable;   
}


void button::draw(boolean b1) {
    // 4 states
    // enable = true,  boolean b1 = true  : pressed button
    // enable = true,  boolean b1 = false : available button
    // enable = false, boolean b1 = true  : statement button
    // enable = false, boolean b1 = false : unavailable button
    if (_enable) {
        if (b1) {
            label(_pscreen, _x1, _y1, _x2, _y2, _text, _textColour, _highColour, 0, 0, _size);
            _pscreen->setPenSolid(false);
            _pscreen->rectangle(_x1+2, _y1+2, _x2-2, _y2-2, _lowColour); 
        } 
        else {
            label(_pscreen, _x1, _y1, _x2, _y2, _text, _textColour, _lowColour, 0, 0, _size);
            _pscreen->setPenSolid(false);
            _pscreen->rectangle(_x1+2, _y1+2, _x2-2, _y2-2, _highColour); 
        }
    }  
    else {
        if (b1) {
            label(_pscreen, _x1, _y1, _x2, _y2, _text, _textColour, _highColour, 0, 0, _size);
            _pscreen->setPenSolid(false);
            //      _pscreen->rectangle(_x1+2, _y1+2, _x2-2, _y2-2, _lowColour); 
        } 
        else {   
            label(_pscreen, _x1, _y1, _x2, _y2, _text, _pscreen->halfColour(_textColour), _pscreen->halfColour(_lowColour), 0, 0, _size);
            _pscreen->setPenSolid(false);      
            _pscreen->rectangle(_x1+2, _y1+2, _x2-2, _y2-2, _pscreen->halfColour(_highColour)); 
        }
    }
}

void button::enable(boolean b1) {
    _enable = b1;
}

boolean button::check(boolean instant) {
    if (!_enable) return false;
    
    uint16_t x0, y0, z0;
    
    // down
    if ((_pscreen->getTouchActivity() == 1) || (_pscreen->getTouchActivity() == 3)) {
        _pscreen->getTouchXY(x0, y0);
        
        // pressed
        if ((x0>=_x1) && (x0<=_x2) && (y0>=_y1) && (y0<=_y2)) {
            if (instant) return true;
          
            draw(true); 
            
            // debounce
            do {   
                z0 = _pscreen->getTouchActivity();
                delay(10); 
            }
            while (z0 != 2);
            
            // released
            delay(50);
            draw();        
            
            // up
            if (z0 == 2)  {
                _pscreen->getTouchXY(x0, y0);
                if ((x0>=_x1) && (x0<=_x2) && (y0>=_y1) && (y0<=_y2)) {
                    return true;
                } 
            } // if up
        } // if pressed
    } // if down
    return false; 
}


uint16_t button::getIndex() {
    return _index;
}


uint8_t dialog(Serial_LCD * lcd0, String text0, uint8_t kind0, uint16_t textColour0, uint16_t highColour0, uint16_t lowColour0, String text1, String button1="OK", uint16_t textColour1=0xffff, uint16_t highColour1=0x000f, uint16_t lowColour1=0x0008, String text2, String button2, uint16_t textColour2, uint16_t highColour2, uint16_t lowColour2, String text3, String button3, uint16_t textColour3, uint16_t highColour3, uint16_t lowColour3) {
    uint8_t a;
    Serial_LCD * _pscreen = lcd0;
    
    // Check SD
    if ( !_pscreen->checkSD() ) a = _pscreen->initSD();
    
    // landscape and portrait coordinates
    uint16_t _x0 = (_pscreen->maxX() - 200) /2;
    uint16_t _y0 = (_pscreen->maxY() - 200) /2;
    
    // Save initial screen
    if ( _pscreen->checkSD() ) {
        if ( _pscreen->checkRAW() ) {
            a = _pscreen->dSaveScreenRAW((uint32_t)0, _x0, _y0, 200, 200);
            //      Serial.print("\n dialog dSaveScreenRAW \t");
            //      Serial.print(a, HEX);
            //      Serial.print("\n");
        }
        else { 
            a = _pscreen->dSaveScreenFAT("dialog.tmp", _x0, _y0, 200, 200); 
            //      Serial.print("\n dialog dSaveScreenFAT \t");
            //      Serial.print(a, HEX);
            //      Serial.print("\n");
        }
    }
    
    // Draw window
    _pscreen->setPenSolid(true);
    _pscreen->dRectangle(_x0, _y0, 200, 200, lowColour0);
    delay(10);    // delay required by chipKIT
    _pscreen->setPenSolid(false);
    _pscreen->dRectangle(_x0+2, _y0+2, 200-2-2, 200-2-2, highColour0); 
    
    // String &result0, String text0, uint8_t kind0
    // uint16_t textColour0, uint16_t highColour0, uint16_t lowColour0, 
    dLabel(_pscreen, _x0+ 60, _y0+ 20, 200-60-10, 20, text0, textColour0, lowColour0, 1, 1, 3);
    dLabel(_pscreen, _x0+ 10, _y0+ 70, 200-10-10, 20, text1, textColour0, lowColour0, 1, 1, 1);
    dLabel(_pscreen, _x0+ 10, _y0+ 90, 200-10-10, 20, text2, textColour0, lowColour0, 1, 1, 1);
    dLabel(_pscreen, _x0+ 10, _y0+110, 200-10-10, 20, text3, textColour0, lowColour0, 1, 1, 1);
    
    // uint8_t kind0, 
    uint16_t colour;
    char label;
    switch (kind0) {
        case 1:   
            colour = _pscreen->setColour(0x00, 0x00, 0xff);  // blue !  
            label ='!';
            break;
        case 2:
            colour = _pscreen->setColour(0xff, 0xff, 0x00);  // yellow ? 
            label ='?';
            break;
        case 3:
            colour = _pscreen->setColour(0xff, 0x00, 0x00);  // red X 
            label ='X';
            break;
        default:    
            colour = _pscreen->setColour(0x00, 0xff, 0x00);  // green + 
            label ='+';
    } 
    
    _pscreen->setPenSolid(false);
    _pscreen->circle(_x0+30, _y0+30, 18, colour);  
    _pscreen->circle(_x0+30, _y0+30, 16, colour);  
    _pscreen->circle(_x0+30, _y0+30, 15, colour);  
    
    _pscreen->setFont(3);
    _pscreen->setFontSolid(false);
    _pscreen->gText(_x0+30- (_pscreen->fontX()>>1)   , _y0+30- (_pscreen->fontY()>>1)   , String(label), colour);          
    _pscreen->gText(_x0+30- (_pscreen->fontX()>>1) +1, _y0+30- (_pscreen->fontY()>>1)   , String(label), colour);          
    _pscreen->gText(_x0+30- (_pscreen->fontX()>>1)   , _y0+30- (_pscreen->fontY()>>1) +1, String(label), colour);          
    _pscreen->gText(_x0+30- (_pscreen->fontX()>>1) +1, _y0+30- (_pscreen->fontY()>>1) +1, String(label), colour);  
    
    button b1;
    button b2;
    button b3;
    
    b1.dDefine(_pscreen, _x0+130, _y0+150, 60, 40, setItem( 1, button1), textColour1, highColour1, lowColour1, 9);
    b2.dDefine(_pscreen, _x0+ 70, _y0+150, 60, 40, setItem( 2, button2), textColour2, highColour2, lowColour2, 9);
    b3.dDefine(_pscreen, _x0+ 10, _y0+150, 60, 40, setItem( 3, button3), textColour3, highColour3, lowColour3, 9);
    
    b1.enable(true);
    b1.draw();
    if (button2.length()>0) { 
        b2.enable(true); 
        b2.draw(); 
    }
    if (button3.length()>0) { 
        b3.enable(true); 
        b3.draw(); 
    }
    
    // dialog main loop
    uint8_t answer = 0;
    while ( answer==0 ) {
        if      (b1.check()) answer = b1.getIndex(); 
        else if (b2.check()) answer = b2.getIndex(); 
        else if (b3.check()) answer = b3.getIndex(); 
    }
    
    if ( _pscreen->checkSD() ) {
        if ( _pscreen->checkRAW() ) a=_pscreen->readScreenRAW((uint32_t)0, _x0, _y0);
        else a=_pscreen->readScreenFAT("dialog.tmp", _x0, _y0);
    }
    
    return answer;
}



uint16_t menu(Serial_LCD * lcd0, item menuItem0[], uint8_t nItems0, uint16_t textColour0, uint16_t highColourMain0, uint16_t highColourSub0) {
    uint8_t i, j, k, level, nMain, nSub, nItems;
    uint16_t current, mask;
    button main[8];
    button sub[8];
    boolean mainFlag, subFlag;
    boolean oldState0, oldState7;
    uint8_t a;
    uint32_t sizeRAW;
    Serial_LCD * _pscreen = lcd0;
    
    // Check SD
    nItems = nItems0;
    if ( !_pscreen->checkSD() ) {
        _pscreen->initSD();
    } 
    
    // landscape and portrait coordinates
    
    // Save initial screen
    if ( _pscreen->checkSD() ) {
        if ( _pscreen->checkRAW() ) {
            sizeRAW = _pscreen->getSectors(80, 30);
            for (i=0; i<8; i++) {
                _pscreen->saveScreenRAW( sizeRAW*(i+0),  0, i*30,  0+79, i*30+29 ); // main
                _pscreen->saveScreenRAW( sizeRAW*(i+8), 80, i*30, 80+79, i*30+29 ); // sub
            }
        } 
        else {
            for (i=0; i<8; i++) {
                _pscreen->saveScreenFAT( "main" + String(i, HEX) + ".tmp",  0, i*30,  0+79, i*30+29 );
                _pscreen->saveScreenFAT( "sub"  + String(i, HEX) + ".tmp", 80, i*30, 80+79, i*30+29 );
            }
        }
    }
    
//    Serial.print("\n ! end of saveScreen... \n");
    
    main[0].dDefine(      _pscreen, 0, 0*30, 79, 29, menuItem0[0], 0xffff, highColourMain0, _pscreen->halfColour(highColourMain0), 9);
    main[6].dStringDefine(_pscreen, 0, 6*30, 79, 29, "Cancel",     0xffff, _pscreen->setColour(0xff, 0xff, 0x00), _pscreen->setColour(0x7f, 0x7f, 0x00), 9);
    main[7].dStringDefine(_pscreen, 0, 7*30, 79, 29, "OK",         0xffff, _pscreen->setColour(0x00, 0xff, 0x00), _pscreen->setColour(0x00, 0x7f, 0x00), 9);
    
    current = 0x0000;
    nMain=0;
    
    mainFlag = false;
    
    // persistent buttons
    main[0].enable(false);
    oldState0 = false;    
    main[6].enable(true);
    main[7].enable(false);
    oldState7 = false;
    
    main[0].draw();
    main[6].draw();
    main[7].draw();
    
    // main loop
    while ( !mainFlag ) {
        for (i=0; i<8; i++)       sub[i].enable(false);
        for (i=0; i<6; i++)       main[i].enable(false);
        
        // main menu column 
        // menu title
        nMain = 0;
        main[nMain].dDefine(_pscreen,  0, nMain*30,  79, 29, menuItem0[nMain], 0xffff, highColourMain0, _pscreen->halfColour(highColourMain0), 9);
        nMain++;
        
        // other menus
        for (k=0; k<4; k++) {    
            mask = (current >> (4*(3-k)));
            if ( (mask & 0xf) >0 ) {
                for (i=1; i<nItems; i++) {
                    if ( menuItem0[i].index == (mask << (4*(3-k))) ) {
                        main[nMain].dDefine(_pscreen,  0, nMain*30,  79, 29, menuItem0[i], 0xffff, highColourMain0, _pscreen->halfColour(highColourMain0), 9);
                        main[nMain].enable(true);
                        main[nMain].draw();
                        nMain++;
                    }
                }
            }
        }  
        
        if ( _pscreen->checkRAW() ) 
            for (i=1; i<6; i++) {
                if (i>=nMain) _pscreen->readScreenRAW( sizeRAW*(i+0),  0, i*30 ); // main
            }
        else for (i=1; i<6; i++) {
            if (i>=nMain) _pscreen->readScreenFAT( "main"  + String(i, HEX) + ".tmp", 0, i*30 );
        } 
        
        // sub-menu column
        mask = 0x0000;
        for (uint8_t k=0; k<4; k++) {    
            if ( (current & (0xf << (4*k))) > 0 ) {
                mask |= ((uint16_t)0xf << (4*k));
            }
        }
        mask ^= 0xffff;
        
        nSub = 0;
        for (i=0; i<nItems; i++) {
            if (nSub<8) {
                
                if ( ((menuItem0[i].index & (mask >> 4)) == 0) && (menuItem0[i].index > current) && (menuItem0[i].index < current+mask) ) {
                    sub[nSub].dDefine(_pscreen, 80, nSub*30, 79, 29, menuItem0[i], 0xffff, highColourSub0, _pscreen->halfColour(highColourSub0), 9);
                    sub[nSub].enable(true);
                    sub[nSub].draw();
                    nSub++;
                }
            }
        }
        
        for (i=0; i<8; i++) {
            if (i>=nSub) {
                if (_pscreen->checkRAW()) _pscreen->readScreenRAW( sizeRAW*(i+8), 80, i*30 ); // sub
                else _pscreen->readScreenFAT( "sub"  + String(i, HEX) + ".tmp", 80, i*30 );
            }
        }
        
        // final preparation
        // hide / show menu Cancel OK buttons
        if (nMain>1) {
            main[nMain-1].enable(false);
            main[nMain-1].draw(true);
        }
        // menu title
        main[0].enable(nMain>1);     
        if ( oldState0 != main[0].state() ) {
            main[0].draw();         
            oldState0 = main[0].state();
        }
        
        // OK button
        main[7].enable(nSub==0);   
        if ( oldState7 != main[7].state() ) {
            main[7].draw();         
            oldState7 = main[7].state();
        }
        
        // touch loop
        subFlag=false;
        
        do {
            for (i=0; i<nSub; i++) {
                if (sub[i].check()) { 
                    current=sub[i].getIndex(); 
                    subFlag = true;
                };
            };
            
            for (i=0; i<nMain; i++) {
                if (main[i].check()) { 
                    current = main[i].getIndex(); 
                    subFlag = true;
                }
            }
            
            if (main[6].check()) {          // Cancel
                mainFlag = true;
                current = 0x0000;
            } 
            else if (main[7].check()) {     // OK
                mainFlag = true;
            }
        } 
        while ( !(subFlag || mainFlag) );
    }
    
    // Restore initial screen
    if ( _pscreen->checkSD() ) {
        if ( _pscreen->checkRAW() ) for (i=0; i<8; i++) {
            _pscreen->readScreenRAW( sizeRAW*(i+0),  0, i*30 ); // main
            _pscreen->readScreenRAW( sizeRAW*(i+8), 80, i*30 ); // sub
        } 
        else for (i=0; i<8; i++) {
            _pscreen->readScreenFAT( "main" + String(i, HEX) + ".tmp",  0, i*30 );
            _pscreen->readScreenFAT( "sub"  + String(i, HEX) + ".tmp", 80, i*30 );
        }
    }
    return current;
}


void dLabel(Serial_LCD  * lcd0, uint16_t x0, uint16_t y0, uint16_t dx, uint16_t dy, String text0, uint16_t textColour0, uint16_t backColour0, uint8_t horizontal0, uint8_t vertical0, uint8_t size0) {
    label(lcd0, x0, y0, x0+dx, y0+dy, text0, textColour0, backColour0, horizontal0, vertical0, size0);
}

void label(Serial_LCD  * lcd0, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, String text0, uint16_t textColour0, uint16_t backColour0, uint8_t horizontal0, uint8_t vertical0, uint8_t size0) {
    uint8_t _size;
    uint16_t _xt, _yt;
    String _text;
    Serial_LCD * _pscreen = lcd0;
    
    if (size0==9) {
        uint16_t i=4;
        do {
            i--;
            _pscreen->setFont(i);
        } 
        while ( (text0.length() * _pscreen->fontX() > (x2-x1) ) && (i>0) );
        _size = i;
    } 
    else {
        _size = size0;
    }
    _pscreen->setFont(_size);
    _text = text0.substring(0, min(text0.length(), (x2-x1) / _pscreen->fontX()));
    _text = _text.trim();
    
    // horizontal 
    // default = 0 = center, 1 = left, 2 = right
    if (horizontal0==1)   _xt = x1;
    else if (horizontal0==2)   _xt = x2 - _text.length() * _pscreen->fontX();
    else   _xt = x1 + ( (x2-x1) - (_text.length() * _pscreen->fontX()) ) /2;
    
    // vertical 
    // default = 0 = center, 1 = left, 2 = right
    if (vertical0==1)   _yt = y1;
    else if (vertical0==2)   _yt = y2 - _pscreen->fontY();
    else   _yt = y1 + ( (y2-y1) - _pscreen->fontY() ) /2;
    
    _pscreen->setPenSolid(true);
    _pscreen->rectangle(x1, y1, x2, y2, backColour0);
    delay(10);    // delay required by chipKIT
    
    _pscreen->setFont(_size);
    _pscreen->setFontSolid(false);
    _pscreen->gText(_xt, _yt, _text, textColour0);
}
