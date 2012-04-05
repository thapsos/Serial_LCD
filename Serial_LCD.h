// 
// 4D Systems μLCD-μLED-μVGA Serial_LCD Library Suite
// Arduino 0023 chipKIT MPIDE 0023 Library
// ----------------------------------
//
// Mar 13, 2012 release 122
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
#define SERIAL_LCD_RELEASE 122

#ifndef Serial_LCD_h
#define Serial_LCD_h

#include "WProgram.h"
#include "proxySerial.h"

// Test release
#if PROXYSERIAL_RELEASE < 106
#error required PROXYSERIAL_RELEASE 106
#endif


// Colours                       Red  Green Blue
//                               4321054321043210
const uint16_t blackColour   = 0b0000000000000000;
const uint16_t whiteColour   = 0b1111111111111111;
const uint16_t redColour     = 0b1111100000000000;
const uint16_t greenColour   = 0b0000011111100000;
const uint16_t blueColour    = 0b0000000000011111;
const uint16_t yellowColour  = 0b1111111111100000;
const uint16_t cyanColour    = 0b0000011111111111;
const uint16_t orangeColour  = 0b1111101111100000;
const uint16_t magentaColour = 0b1111100000001111;
const uint16_t violetColour  = 0b0111100000011111;
const uint16_t grayColour    = 0b0111101111101111;


// Objects

class Serial_LCD {
public:
    Serial_LCD(ProxySerial * port0); // uint8_t receivePin, uint8_t transmitPin); // constructor
    
    // 2.1 General Commands
    void begin(int8_t resetPin0=-1); // AutoBaud – 55hex 
    uint8_t setSpeed(uint16_t speed); // Set new Baud-Rate - 51hex 
    String WhoAmI(); // Version-Device Info Request – 56hex 
    uint8_t replaceBackGroundColour(uint16_t colour);  // Replace Background Colour – 42hex 
    uint8_t clear(); // Clear Screen – 45hex
    uint8_t setBacklight(boolean  b);   // Display Control Functions – 59hex 
    uint8_t setDisplay(boolean  b);   // Display Control Functions – 59hex
    uint8_t setContrast(uint8_t b);   // Display Control Functions – 59hex
    uint8_t setOrientation(uint8_t b);   // Display Control Functions – 59hex
    uint8_t getOrientation();  // Display Control Functions – 59hex
    uint8_t setTouch(boolean  b);   // Display Control Functions – 59hex
    uint8_t setResolutionVGA(uint8_t b);   // Set VGA Resolution Control Functions – 59hex
    uint8_t setVolume(uint8_t percent); // Set Volume - 76hex 
    uint8_t protectFAT(boolean b);  // Display Control Functions – 59hex
    uint8_t checkScreenType();
    uint8_t checkHardwareVersion();
    uint8_t checkSoftwareVersion();
    
    // Sleep – 5Ahex 
    // Read GPIO Pin - 61hex
    // Write GPIO Pin - 69hex 
    // Read GPIO Bus - 79hex 
    // Write GPIO Bus – 57hex  
    void off(); // Highly recommended at the end
    
    // 2.2 Graphics Commands
    // Add User Bitmap Character – 41hex 
    // Draw User Bitmap Character – 44hex 
    uint8_t circle(uint16_t x1, uint16_t y1, uint16_t radius, uint16_t colour);  // Draw Circle – 43hex 
    uint8_t triangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3, uint16_t colour);  // Draw Triangle – 47hex 
                                                                                                                      // Draw Image-Icon – 49hex 
    uint8_t setBackGroundColour(uint16_t colour=blackColour);   // Set Background colour – 4Bhex 
    uint8_t dLine(uint16_t x0, uint16_t y0, uint16_t dx, uint16_t dy, uint16_t colour);  // Draw Line – 4Chex 
    uint8_t line(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t colour);  // Draw Line – 4Chex 
                                                                                        // Draw Polygon – 67hex 
    uint8_t rectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t colour);  // Draw Rectangle – 72hex 
    uint8_t dRectangle(uint16_t x0, uint16_t y0, uint16_t dx, uint16_t dy, uint16_t colour);  // Draw Rectangle – 72hex 
    uint8_t ellipse(uint16_t x, uint16_t y, uint16_t rx, uint16_t ry, uint16_t colour); // Draw Ellipse – 65hex 
    uint8_t point(uint16_t x1, uint16_t y1, uint16_t colour);   // Draw Pixel – 50hex 
    uint16_t readPixel(uint16_t x1, uint16_t y1); // Read Pixel – 52hex 
    uint8_t copyPaste(uint16_t xs, uint16_t ys, uint16_t xt, uint16_t yt, uint16_t dx, uint16_t dy); // Screen Copy-Paste – 63hex 
                                                                                                     // Replace colour – 6Bhex 
    uint8_t setPenSolid(boolean b=true);    // Set Pen Size 1=solid; 0=wire frame – 70hex
    
    // 2.3 Text Commands
    uint8_t setFont(uint8_t b=0);  // Set Font – 46hex 
    uint8_t setFontSolid(boolean b=true);  // Set 0=Transparent-1=Opaque Text – 4Fhex 
                                      // Draw ASCII Character (text format) – 54hex 
                                      // Draw ASCII Character (graphics format) – 74hex 
    uint8_t tText(uint8_t x, uint8_t y, String s, uint16_t colour=whiteColour);  // Draw “String” of ASCII Text (text format) – 73hex 
    uint8_t gText(uint16_t x, uint16_t y, String s, uint16_t colour=whiteColour);    // Draw “String” of ASCII Text (graphics format) – 53hex 
                                                                                     // Draw Text Button – 62hex
    
    // 2.4 Touch Screen Commands
    // Touch screen must be enabled to be able to use the touch commands. 
    uint8_t getTouchActivity();   // Get Touch Coordinates - 6Fhex - 0 : No Touch Activity 1 : Touch Press 2 : Touch Release 3 : Touch Moving
    uint8_t getTouchXY(uint16_t &x, uint16_t &y);   // Get Touch Coordinates - 6Fhex 
                                                    // Wait Until Touch - 77hex 
    uint8_t dDetectTouchRegion(uint16_t x0, uint16_t y0, uint16_t dx, uint16_t dy);
    uint8_t detectTouchRegion(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2); // Detect Touch Region - 75hex
    
    // 2.5 SD Memory Card Commands (Low-Level/RAW)
    // Initialise Memory Card - @69hex 
    // Set Address Pointer of Card (RAW) - @41hex 
    // Read uint8_t Data from Card (RAW) - @72hex 
    // Write uint8_t Data to Card (RAW) - @77hex 
    // Read Sector Block Data from Card (RAW) - @52hex 
    // Write Sector Block Data to Card (RAW) - @57hex 
    // Display Image-Icon from Card (RAW) - @49hex 
    // Display Object from Card (RAW) - @4Fhex 
    // sector should be 0:high:mid:low
    uint8_t saveScreenRAW(uint32_t sector);   // Screen Copy-Save to Card (RAW) - @43hex
    uint8_t dSaveScreenRAW(uint32_t sector, uint16_t x0, uint16_t y0, uint16_t dx, uint16_t dy);  // x0, y0 and width and height  
    uint8_t saveScreenRAW(uint32_t sector, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);  // x1, y1 x2, y2: same coordinates as rectangle
    uint8_t readScreenRAW(uint32_t sector, uint16_t x1=0, uint16_t y1=0); // x1, y1: left-top coordinates
    uint32_t getSectors(uint16_t x, uint16_t y, uint16_t sizeSector=512); // sector = 512
    
    // Display Video-Animation Clip from Card (RAW) - @56hex 
    // Run Script (4DSL) Program from Card (RAW) - @50hex
    
    // 2.6 SD Memory Card Commands (FAT16-Level/DOS)
    uint8_t initSD();   // Initialise Memory Card - @69hex 
    boolean checkSD();  // is SD card available?
    boolean checkRAW(); // is RAW partition available?
    boolean checkFAT(); // is FAT partition available?
    
    //    void setRAW(boolean b);
    // Read File from Card (FAT) - @61hex with open...next loop: caution, no screen releated function in the loop 
    uint8_t openTextFileDelimiter(String filename, char delimiter);
    boolean nextTextFileDelimiter(String * result); // false = end
    
    // Read File from Card (FAT) - @61hex with call-back
    uint8_t readTextFileDelimiter(String filename, char delimiter, void (*cbReadFile)(String text));
    uint8_t readTextFile(String filename, uint8_t bytes, void (*cbReadFile)(String text));

    // Write File to Card (FAT) - @74hex 
    uint8_t writeStringFile(String filename, String text, uint8_t option=0x00);   
    uint8_t appendStringFile(String filename, String text);
    uint8_t eraseFile(String filename);   // Erase file from Card (FAT) - @65hex 
    uint8_t findFile(String filename);    // List Directory from Card (FAT) - @64hex 
    uint8_t saveScreenFAT(String filename);   // Screen Copy-Save to Card (FAT) - @63hex 
    uint8_t dSaveScreenFAT(String filename, uint16_t x0, uint16_t y0, uint16_t dx, uint16_t dy);  // x0, y0 and width and height  
    uint8_t saveScreenFAT(String filename, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);  // x1, y1 x2, y2: same coordinates as rectangle
    uint8_t readScreenFAT(String filename, uint16_t x1=0, uint16_t y1=0); // x1, y1: left-top coordinates
    uint8_t readScreenGCI(String filename, uint16_t x1=0, uint16_t y1=0, uint16_t msb2=0, uint16_t lsb2=0); // x1, y1: left-top coordinates; msb2:lsb2 icon sector address
    uint8_t playSoundSD(String filename, uint8_t option0=0);  // Play Audio WAV file from Card (FAT) - @6Chex 
                                                              // Run Script (4DSL) Program from Card (FAT) - @70hex
    
    // Utilities
    uint16_t setColour(uint8_t red, uint8_t green, uint8_t blue);
    void splitColour(uint16_t rgb, uint8_t &red, uint8_t &green, uint8_t &blue);
    uint16_t halfColour(uint16_t rgb);
    uint16_t reverseColour(uint16_t rgb);
    
    uint8_t nacAck(); // 0x06=success, 0x15=error
    uint8_t fontX();     
    uint8_t fontY(); 
    uint16_t maxX();
    uint16_t maxY();
    
private:
    ProxySerial * _port;
    
    int8_t _resetPin;
    uint8_t _checkedScreenType;
    uint8_t _checkedHardwareVersion;
    uint8_t _checkedSoftwareVersion;
    boolean _checkedSD;
    boolean _checkedRAW;
    boolean _checkedFAT;
    uint8_t _font, _fontX, _fontY;
    uint8_t _touch_buffer[4];
    uint8_t _orientation;
    uint16_t _maxX, _maxY;
    uint32_t _fileSize;
    uint8_t _delimeter;
    
    void _swap(uint16_t &a, uint16_t &b);
};


#endif
