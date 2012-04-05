// 
// 4D Systems μLCD-μLED-μVGA Serial_LCD Library Suite
// Arduino 1.0 Library
// ----------------------------------
// Developped with embedXcode
//
// Mar 19, 2012 release 209
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
#include "Graphics.h"


gClock::gClock() {
	;
}

void gClock::dDefine(Serial_LCD * lcd0, uint16_t x0, uint16_t y0, uint16_t dx, uint16_t dy, uint16_t backColour, uint16_t frontColour, uint16_t hourColour, uint16_t minuteColour, uint16_t secondColour) {
	uint16_t radius = min(dx, dy)/2;
	define(lcd0, x0+dx/2, y0+dy/2, radius-2, backColour, frontColour, hourColour, minuteColour, secondColour);
}

void gClock::define(Serial_LCD * lcd0, uint16_t x0, uint16_t y0, uint16_t radius, uint16_t backColour, uint16_t frontColour, uint16_t hourColour, uint16_t minuteColour, uint16_t secondColour) {
	
	_pscreen = lcd0;
	
	_x0 = x0; 
	_y0 = y0; 
	_radius = radius;
	_backColour = backColour;
	_frontColour = frontColour;
	_hourColour = hourColour; 
	_minuteColour = minuteColour; 
	_secondColour = secondColour;
	
	_oldHour = 0;
	_oldMinute = 0;
	_oldSecond = 0;
	
	_pscreen->setPenSolid(true);
	_pscreen->circle(_x0, _y0, _radius, _backColour); 
	
	_pscreen->setPenSolid(false);
	_pscreen->circle(_x0, _y0, _radius, _frontColour); 
	
	_pscreen->setPenSolid(true);
	for (int8_t i=0; i<12; i++) {
		_pscreen->circle(_x0+(_radius-2)*cos(i*PI/6.0), _y0+(_radius-2)*sin(i*PI/6.0), 2, _frontColour); 
	}
}

void gClock::draw(uint8_t hour, uint8_t minute, uint8_t second) {
	uint16_t hX, hY, mX, mY, sX, sY;
	uint16_t _hX, _hY, _mX, _mY, _sX, _sY;
	
	hX  = uint16_t( _x0+_radius*0.6*cos( (hour+minute/60.0         -3.0)*2.0*PI/12.0 ) );
	hY  = uint16_t( _y0+_radius*0.6*sin( (hour+minute/60.0         -3.0)*2.0*PI/12.0 ) );
	mX  = uint16_t( _x0+_radius*0.8*cos( (     minute+second/60.0 -15.0)*2.0*PI/60.0 ) );
	mY  = uint16_t( _y0+_radius*0.8*sin( (     minute+second/60.0 -15.0)*2.0*PI/60.0 ) );
	sX  = uint16_t( _x0+_radius*0.8*cos( (            second      -15.0)*2.0*PI/60.0 ) );
	sY  = uint16_t( _y0+_radius*0.8*sin( (            second      -15.0)*2.0*PI/60.0 ) );
	
	_hX  = uint16_t( _x0+_radius*0.6*cos( (_oldHour+_oldMinute/60.0             -3.0)*2.0*PI/12.0 ) );
	_hY  = uint16_t( _y0+_radius*0.6*sin( (_oldHour+_oldMinute/60.0             -3.0)*2.0*PI/12.0 ) );
	_mX  = uint16_t( _x0+_radius*0.8*cos( (         _oldMinute+_oldSecond/60.0 -15.0)*2.0*PI/60.0 ) );
	_mY  = uint16_t( _y0+_radius*0.8*sin( (         _oldMinute+_oldSecond/60.0 -15.0)*2.0*PI/60.0 ) );
	_sX  = uint16_t( _x0+_radius*0.8*cos( (                    _oldSecond      -15.0)*2.0*PI/60.0 ) );
	_sY  = uint16_t( _y0+_radius*0.8*sin( (                    _oldSecond      -15.0)*2.0*PI/60.0 ) );
	
	// old time with _backColour
	if ( (sX!=_sX) || (sY!=_sY) ) {
		_pscreen->line(_x0, _y0, _sX, _sY, _backColour);
		
		if ( (mX!=_mX) || (mY!=_mY) ) {   
			_pscreen->line(_x0,   _y0,   _mX,   _mY,   _backColour);
			_pscreen->line(_x0,   _y0+1, _mX,   _mY+1, _backColour);
			_pscreen->line(_x0+1, _y0,   _mX+1, _mY,   _backColour);
			_pscreen->line(_x0+1, _y0+1, _mX+1, _mY+1, _backColour);
			
			if ( (hX!=_hX) || (hY!=_hY) ) {
				_pscreen->line(_x0-1, _y0-1, _hX-1, _hY-1, _backColour);
				_pscreen->line(_x0-1, _y0,   _hX-1, _hY,   _backColour);
				_pscreen->line(_x0-1, _y0+1, _hX-1, _hY+1, _backColour);
				_pscreen->line(_x0,   _y0-1, _hX,   _hY-1, _backColour);
				_pscreen->line(_x0,   _y0,   _hX,   _hY,   _backColour);
				_pscreen->line(_x0,   _y0+1, _hX,   _hY+1, _backColour);
				_pscreen->line(_x0+1, _y0-1, _hX+1, _hY-1, _backColour);
				_pscreen->line(_x0+1, _y0,   _hX+1, _hY,   _backColour);
				_pscreen->line(_x0+1, _y0+1, _hX+1, _hY+1, _backColour);
				
			}
		}
		// new time with _hourColour, _minuteColour, _secondColour
		_pscreen->line(_x0-1, _y0-1, hX-1, hY-1, _hourColour);
		_pscreen->line(_x0-1, _y0,   hX-1, hY,   _hourColour);
		_pscreen->line(_x0-1, _y0+1, hX-1, hY+1, _hourColour);
		_pscreen->line(_x0,   _y0-1, hX,   hY-1, _hourColour);
		_pscreen->line(_x0,   _y0,   hX,   hY,   _hourColour);
		_pscreen->line(_x0,   _y0+1, hX,   hY+1, _hourColour);
		_pscreen->line(_x0+1, _y0-1, hX+1, hY-1, _hourColour);
		_pscreen->line(_x0+1, _y0,   hX+1, hY,   _hourColour);
		_pscreen->line(_x0+1, _y0+1, hX+1, hY+1, _hourColour);
		
		_pscreen->line(_x0,   _y0,   mX,   mY,   _minuteColour);
		_pscreen->line(_x0,   _y0+1, mX,   mY+1, _minuteColour);
		_pscreen->line(_x0+1, _y0,   mX+1, mY,   _minuteColour);
		_pscreen->line(_x0+1, _y0+1, mX+1, mY+1, _minuteColour);
		
		_pscreen->line(_x0, _y0, sX, sY, _secondColour);
		
		_pscreen->setPenSolid(true);
		_pscreen->circle(_x0, _y0, 3, _frontColour); 
	}
	_oldSecond = second;
	_oldMinute = minute;
	_oldHour   = hour;
}


gYaw::gYaw() {
	;
}

void gYaw::dDefine(Serial_LCD * lcd0, uint16_t x0, uint16_t y0, uint16_t dx, uint16_t dy, uint16_t backColour, uint16_t frontColour, uint16_t gridColour, uint16_t valueColour) {
	uint16_t radius = min(dx, dy)/2;
	define(lcd0, x0-radius, y0-radius, radius-2, backColour, frontColour, gridColour, valueColour);
}

void gYaw::define(Serial_LCD * lcd0, uint16_t x0, uint16_t y0, uint16_t radius, uint16_t backColour, uint16_t frontColour, uint16_t gridColour, uint16_t valueColour) {
	_pscreen = lcd0;
	
	_x0 = x0; 
	_y0 = y0; 
	_radius = radius;
	_backColour = backColour;
	_frontColour = frontColour;
	_gridColour = gridColour;
    _valueColour = valueColour;
	
	_pscreen->setPenSolid(true);
	_pscreen->circle(_x0, _y0, _radius, _backColour); 
	_pscreen->setPenSolid(false);
	_pscreen->circle(_x0, _y0, _radius, _gridColour); 
	
	_oldDegrees = 0.0;
}

void gYaw::draw(float degrees) {
	float radians, c, s;
	uint16_t x1, x2, x3, x4, y1, y2, y3, y4;
	
	if (degrees!=_oldDegrees) {
		
		// old value with _backColour
		radians = (-_oldDegrees / 180.0 * PI + PI);
		c = cos(radians) * 0.6 * _radius;
		s = sin(radians) * 0.6 * _radius;
		
		x1 = _x0 + (  c*(-0.5) + s*( 0.0) ) + 0.5;
		y1 = _y0 + ( -s*(-0.5) + c*( 0.0) ) + 0.5;
		x2 = _x0 + (  c*( 0.5) + s*( 0.0) ) + 0.5;
		y2 = _y0 + ( -s*( 0.5) + c*( 0.0) ) + 0.5;
		x3 = _x0 + (  c*(-0.5) + s*(-1.0) ) + 0.5;
		y3 = _y0 + ( -s*(-0.5) + c*(-1.0) ) + 0.5;
		x4 = _x0 + (  c*( 0.5) + s*(-1.0) ) + 0.5;
		y4 = _y0 + ( -s*( 0.5) + c*(-1.0) ) + 0.5;
		
		_pscreen->line(x1, y1, x3, y3, _backColour);
		_pscreen->line(x3, y3, x4, y4, _backColour);
		_pscreen->line(x2, y2, x4, y4, _backColour);
		
		x3 = _x0 + (  c*(-1.0) + s*( 0.0) ) + 0.5;
		y3 = _y0 + ( -s*(-1.0) + c*( 0.0) ) + 0.5;
		x4 = _x0 + (  c*( 1.0) + s*( 0.0) ) + 0.5;
		y4 = _y0 + ( -s*( 1.0) + c*( 0.0) ) + 0.5;
		_pscreen->line(x1, y1, x3, y3, _backColour);
		_pscreen->line(x2, y2, x4, y4, _backColour);
		
		x1 = _x0 + (  c*( 0.0) + s*( 1.0) ) + 0.5;
		y1 = _y0 + ( -s*( 0.0) + c*( 1.0) ) + 0.5;
		_pscreen->line(x1, y1, x3, y3, _backColour);
		_pscreen->line(x1, y1, x4, y4, _backColour);
		
        
		// new value with _frontColour
		radians = (-degrees / 180.0 * PI + PI);
		c = cos(radians) * 0.6 * _radius;
		s = sin(radians) * 0.6 * _radius;
		
		x1 = _x0 + (  c*(-0.5) + s*( 0.0) ) + 0.5;
		y1 = _y0 + ( -s*(-0.5) + c*( 0.0) ) + 0.5;
		x2 = _x0 + (  c*( 0.5) + s*( 0.0) ) + 0.5;
		y2 = _y0 + ( -s*( 0.5) + c*( 0.0) ) + 0.5;
		x3 = _x0 + (  c*(-0.5) + s*(-1.0) ) + 0.5;
		y3 = _y0 + ( -s*(-0.5) + c*(-1.0) ) + 0.5;
		x4 = _x0 + (  c*( 0.5) + s*(-1.0) ) + 0.5;
		y4 = _y0 + ( -s*( 0.5) + c*(-1.0) ) + 0.5;
		
		_pscreen->line(x1, y1, x3, y3, _valueColour);
		_pscreen->line(x3, y3, x4, y4, _valueColour);
		_pscreen->line(x2, y2, x4, y4, _valueColour);
		
		x3 = _x0 + (  c*(-1.0) + s*( 0.0) ) + 0.5;
		y3 = _y0 + ( -s*(-1.0) + c*( 0.0) ) + 0.5;
		x4 = _x0 + (  c*( 1.0) + s*( 0.0) ) + 0.5;
		y4 = _y0 + ( -s*( 1.0) + c*( 0.0) ) + 0.5;
		_pscreen->line(x1, y1, x3, y3, _valueColour);
		_pscreen->line(x2, y2, x4, y4, _valueColour);
		
		x1 = _x0 + (  c*( 0.0) + s*( 1.0) ) + 0.5;
		y1 = _y0 + ( -s*( 0.0) + c*( 1.0) ) + 0.5;
		_pscreen->line(x1, y1, x3, y3, _valueColour);
		_pscreen->line(x1, y1, x4, y4, _valueColour);
		
		_oldDegrees = degrees;
	}
	
	_pscreen->circle(_x0, _y0, 2, _frontColour); 
} 



gPitch::gPitch() {
	;
}

void gPitch::dDefine(Serial_LCD * lcd0, uint16_t x0, uint16_t y0, uint16_t dx, uint16_t dy, uint16_t backColour, uint16_t frontColour, uint16_t gridColour, uint16_t valueColour) {
	uint16_t radius = min(dx, dy)/2;
	define(lcd0, x0-radius, y0-radius, radius-2, backColour, frontColour, gridColour, valueColour);
}

void gPitch::define(Serial_LCD * lcd0, uint16_t x0, uint16_t y0, uint16_t radius, uint16_t backColour, uint16_t frontColour, uint16_t gridColour, uint16_t valueColour) {
	_pscreen = lcd0;
	
	_x0 = x0; 
	_y0 = y0; 
	_radius = radius;
	_backColour = backColour;
	_frontColour = frontColour;
    _gridColour = gridColour;
    _valueColour = valueColour;
	
	_pscreen->setPenSolid(true);
	_pscreen->circle(_x0, _y0, _radius, _backColour); 
	_pscreen->setPenSolid(false);
	_pscreen->circle(_x0, _y0, _radius, _gridColour); 
	
	_oldDegrees = 0.0;
}

void gPitch::draw(float degrees) {
	float value, radians, c, s;
	uint16_t x1, x2, y1, y2;
	
	if (degrees!=_oldDegrees) {
		// old value with _backColour
		value = _oldDegrees;		
		if (value > 180)  value -= 360;
		if (value < -180) value += 360;
		radians = (value / 180.0 * PI);
		c = cos(radians) * 0.6 * _radius;
		s = sin(radians) * 0.6 * _radius;
		
		x1 = _x0 + (  c*( 1.0) + s*( 0.0) ) + 0.5;
		y1 = _y0 + ( -s*( 1.0) + c*( 0.0) ) + 0.5;
		x2 = _x0 - (  c*( 1.0) + s*( 0.0) ) + 0.5;
		
		if (x1>x2) { 
			y2=x2; 
			x2=x1; 
			x1=y2; 
		}
		_pscreen->line(x1-_radius/10, y1, x2+_radius/10, y1, _backColour);
		
		x1 = _x0 + (  c*( 0.7) + s*( 0.0) ) + 0.5;
		x2 = _x0 - (  c*( 0.7) + s*( 0.0) ) + 0.5;
		
		if (x1>x2) { 
			y2=x2; 
			x2=x1; 
			x1=y2; 
		}
		
		x1 -= _radius/20;
		x2 += _radius/20;
		
		if ((degrees>=-90) && (degrees<=90)) {    
			y1 -= _radius/6;  
		} 
		else    { 
			y1 += _radius/6; 
		}
		
		_pscreen->line(x1, y1, x2, y1, _backColour);
		
		
		// new value with _frontColour
		value = degrees;
		if (value > 180)  value -= 360;
		if (value < -180) value += 360;
		radians = (value / 180.0 * PI);
		c = cos(radians) * 0.6 * _radius;
		s = sin(radians) * 0.6 * _radius;		
		
		x1 = _x0 + (  c*( 1.0) + s*( 0.0) ) + 0.5;
		y1 = _y0 + ( -s*( 1.0) + c*( 0.0) ) + 0.5;
		x2 = _x0 - (  c*( 1.0) + s*( 0.0) ) + 0.5;
		
		if (x1>x2) { 
			y2=x2; 
			x2=x1; 
			x1=y2; 
		}
		_pscreen->line(x1-_radius/10, y1, x2+_radius/10, y1, _valueColour);
		
		x1 = _x0 + (  c*( 0.7) + s*( 0.0) ) + 0.5;
		x2 = _x0 - (  c*( 0.7) + s*( 0.0) ) + 0.5;
		
		if (x1>x2) { 
			y2=x2; 
			x2=x1; 
			x1=y2; 
		}
		
		x1 -= _radius/20;
		x2 += _radius/20;
		
		if ((degrees>=-90) && (degrees<=90)) {    
			y1 -= _radius/6;  
		} 
		else    { 
			y1 += _radius/6; 
		}
		
		_pscreen->line(x1, y1, x2, y1, _frontColour);
		_pscreen->circle(_x0, _y0, 2, _frontColour); 		
		
		_oldDegrees = degrees;
	}
}



gRoll::gRoll() {
	;
}

void gRoll::dDefine(Serial_LCD * lcd0, uint16_t x0, uint16_t y0, uint16_t dx, uint16_t dy, uint16_t backColour, uint16_t frontColour, uint16_t gridColour, uint16_t valueColour) {
	uint16_t radius = min(dx, dy)/2;
	define(lcd0, x0-radius, y0-radius, radius-2, backColour, frontColour, gridColour, valueColour);
}

void gRoll::define(Serial_LCD * lcd0, uint16_t x0, uint16_t y0, uint16_t radius, uint16_t backColour, uint16_t frontColour, uint16_t gridColour, uint16_t valueColour) {
	_pscreen = lcd0;
	
	_x0 = x0; 
	_y0 = y0; 
	_radius = radius;
	_backColour = backColour;
	_frontColour = frontColour;
	_gridColour = gridColour;
    _valueColour = valueColour;
	
	_pscreen->setPenSolid(true);
	_pscreen->circle(_x0, _y0, _radius, _backColour); 
	_pscreen->setPenSolid(false);
	_pscreen->circle(_x0, _y0, _radius, _gridColour); 
	
	_oldDegrees = 0.0;
}

void gRoll::draw(float degrees) {
	float radians, c, s;
	uint16_t x1, x2, y1, y2;
	
	if (degrees!=_oldDegrees) {
		// old value with _backColour
		radians = (-_oldDegrees / 180.0 * PI + PI);
		c = cos(radians) * 0.6 * _radius;
		s = sin(radians) * 0.6 * _radius;
		
		_pscreen->setPenSolid(true);
		
		x1 = _x0 + (  c*(-1.0) + s*( 0.0) ) + 0.5;
		y1 = _y0 + ( -s*(-1.0) + c*( 0.0) ) + 0.5;
		x2 = _x0 + (  c*( 1.0) + s*( 0.0) ) + 0.5;
		y2 = _y0 + ( -s*( 1.0) + c*( 0.0) ) + 0.5;
		_pscreen->line(x1, y1, x2, y2, _backColour);
		
		x1 = _x0 + (  c*(-0.7) + s*( 0.3) ) + 0.5;
		y1 = _y0 + ( -s*(-0.7) + c*( 0.3) ) + 0.5;
		x2 = _x0 + (  c*( 0.7) + s*( 0.3) ) + 0.5;
		y2 = _y0 + ( -s*( 0.7) + c*( 0.3) ) + 0.5;
		_pscreen->line(x1, y1, x2, y2, _backColour);
		
		
		// new value with _frontColour
		radians = (-degrees / 180.0 * PI + PI);
		c = cos(radians) * 0.6 * _radius;
		s = sin(radians) * 0.6 * _radius;
		
		x1 = _x0 + (  c*(-1.0) + s*( 0.0) ) + 0.5;
		y1 = _y0 + ( -s*(-1.0) + c*( 0.0) ) + 0.5;
		x2 = _x0 + (  c*( 1.0) + s*( 0.0) ) + 0.5;
		y2 = _y0 + ( -s*( 1.0) + c*( 0.0) ) + 0.5;
		_pscreen->line(x1, y1, x2, y2, _valueColour);
		
		x1 = _x0 + (  c*(-0.7) + s*( 0.3) ) + 0.5;
		y1 = _y0 + ( -s*(-0.7) + c*( 0.3) ) + 0.5;
		x2 = _x0 + (  c*( 0.7) + s*( 0.3) ) + 0.5;
		y2 = _y0 + ( -s*( 0.7) + c*( 0.3) ) + 0.5;
		_pscreen->line(x1, y1, x2, y2, _valueColour);
		
		_pscreen->circle(_x0, _y0, 2, _frontColour); 
		
		_oldDegrees = degrees;
	}
}		



gHistogram::gHistogram() {
	;
}

void gHistogram::define(Serial_LCD * lcd0, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, float valueMin, float valueMax, uint16_t lapse, uint16_t memory, uint16_t gridX, uint16_t gridY, boolean continous, uint16_t backColour, uint16_t frontColour, uint16_t gridColour, uint16_t valueColour, uint16_t minColour, uint16_t maxColour) {
    dDefine(lcd0, x1, y1, x2-x1, y2-y1, valueMin, valueMax, lapse, memory, gridX, gridY, continous, backColour, frontColour, gridColour, valueColour, minColour, maxColour);
}


void gHistogram::dDefine(Serial_LCD * lcd0, uint16_t x0, uint16_t y0, uint16_t dx, uint16_t dy, float valueMin, float valueMax, uint16_t lapse, uint16_t memory, uint16_t gridX, uint16_t gridY, boolean continous, uint16_t backColour, uint16_t frontColour, uint16_t gridColour, uint16_t valueColour, uint16_t minColour, uint16_t maxColour) {
	_pscreen = lcd0;
	
	_x0 = x0; 
	_y0 = y0; 
	_dx = dx;
	_dy = dy;
	
	_backColour  = backColour;
	_frontColour = frontColour;
	_gridColour  = gridColour;
	_valueColour = valueColour;
	_minColour   = minColour;
	_maxColour   = maxColour;
	
	_valueMin  = valueMin;
	_valueMax  = valueMax;
	_continous = continous;
	_lapse     = lapse;
	_memory    = memory; 
	_gridX     = gridX;
	_gridY     = gridY;
	_gridC     = 0;
	_chrono    = millis();
	
	_pscreen->setPenSolid(true);
	_pscreen->dRectangle(_x0, _y0, _dx, _dy, _backColour);
	_pscreen->setPenSolid(false);
	_pscreen->dRectangle(_x0, _y0, _dx, _dy, _gridColour);
	
	_n   = 0; // number of values shown
	_min = 0; // y coordinate of min for memory
	_max = 0; // y coordinate of max for memory
	_amnesiaMin=0;
	_amnesiaMax=0;
	
	if (_gridX>0) for (uint16_t i=1; i<_gridX; i++) {
		_pscreen->dLine(_x0+1, _y0+map(i, 0, _gridX, 0, _dy), _dx-2, 1, _pscreen->halfColour(_gridColour));
	}
}



void gHistogram::draw(float value) {
	uint32_t l = millis()-_chrono;
	if ( (_lapse>0) && (l<_lapse) ) return;
	_chrono = millis();
	
	uint16_t y = _y0+1+_dy-2 - (uint16_t)( (value-_valueMin)*(_dy-2)/(_valueMax-_valueMin) );
	if (y<_y0+2)     y = _y0+2;
	if (y>_y0+_dy-4) y = _y0+_dy-4;
	
	// continuity management
	if (_continous) {
		// right-most side
		if (_n>_dx-4) {
			// move
			_pscreen->copyPaste(_x0+2, _y0, _x0+1, _y0, _dx-3, _dy);
			// new 
			_pscreen->line(_x0+_n+1, _y0+1, _x0+_n+1, _y0+_dy-2, _backColour);
			
			// gridY
			if (_gridY>0) { 
				if (_gridC==0) _pscreen->line(_x0+_n+1, _y0+1, _x0+_n+1, _y0+_dy-2, _pscreen->halfColour(_gridColour));
				_gridC++;
				_gridC %= _gridY;
			} // gridY
			
			// gridX
			if (_gridX>0) for (uint16_t i=1; i<_gridX; i++) 
				_pscreen->point(_x0+_n+1, _y0+map(i, 0, _gridX, 0, _dy), _pscreen->halfColour(_gridColour));
		} 
		else {
			_n++;
		} // right-most side
	} 
	else {
		// right 
		if (_n>_dx-4) {
			_n=1;
		} 
		else {
			_n++;
		} // right 
		
		_pscreen->line(_x0+_n+1, _y0+1, _x0+_n+1, _y0+_dy-2, _backColour);
		_pscreen->line(_x0+_n+2, _y0+1, _x0+_n+2, _y0+_dy-2, _gridColour);
		
		// gridY
		if (_gridY>0) { 
			if (_gridC==0) _pscreen->line(_x0+_n+1, _y0+1, _x0+_n+1, _y0+_dy-2, _pscreen->halfColour(_gridColour));
			_gridC++;
			_gridC %= _gridY;
		} // gridY
		
		// gridX
		if (_gridX>0) for (uint16_t i=1; i<_gridX; i++) 
			_pscreen->point(_x0+_n+1, _y0+map(i, 0, _gridX, 0, _dy), _pscreen->halfColour(_gridColour));
		
	} // continuity management
	
	// value
	_pscreen->dRectangle(_x0+_n, y, 2, 2, _valueColour);
	
	// min and max memory
	if (_memory>0) {
		
		// first time
		if (_n==0) {
			_max = y;
			_min = y;
		}
		
		// max - coordinates in reverse scale
		if (y<=_max) {
			_max = y;
			_amnesiaMax = _memory;
		} 
		else if (_amnesiaMax>0) {
			_pscreen->dRectangle(_x0+_n, _max, 2, 2, _maxColour);
			_amnesiaMax--;
		} 
		else {
			_max = y;
		} // max
		
		// min - coordinates in reverse scale
		if (y>=_min) {
			_min = y;
			_amnesiaMin = _memory;
		} 
		else if (_amnesiaMin>0) {  
			_pscreen->dRectangle(_x0+_n, _min, 2, 2, _minColour);
			_amnesiaMin--;
		} 
		else {
			_min = y;
		} // min
	} // min and max memory
	
	;
	_pscreen->setFont(0);
	_pscreen->setFontSolid(true);
	_pscreen->gText(_x0+3, _y0+2, ftoa(_valueMax, 1, 0), _frontColour);
	_pscreen->gText(_x0+3, _y0+_dy-2-_pscreen->fontY(), ftoa(_valueMin, 1, 0), _frontColour);
	
	// lapse error
	if (_lapse>0) {
		if (l>_lapse) {
			if (_gridY>0) {
				_pscreen->setBackGroundColour(_frontColour);
				_pscreen->gText(_x0+_dx-2-7*_pscreen->fontX(), _y0+_dy-2-_pscreen->fontY(), ttoa(l*_gridY/1000.0, 1, 7), _backColour);
				_pscreen->setBackGroundColour(_backColour);
			} 
			else {
				_pscreen->setBackGroundColour(_frontColour);
				_pscreen->gText(_x0+_dx-2-7*_pscreen->fontX(), _y0+_dy-2-_pscreen->fontY(), ttoa(l, 0, 7), _backColour);
				_pscreen->setBackGroundColour(_backColour);
			} // _gridY
			
		} 
		else {
			if (_gridY>0) {
				_pscreen->gText(_x0+_dx-2-7*_pscreen->fontX(), _y0+_dy-2-_pscreen->fontY(), ttoa(l*_gridY/1000.0, 1, 7), _frontColour);
			} 
			else {
				_pscreen->gText(_x0+_dx-2-7*_pscreen->fontX(), _y0+_dy-2-_pscreen->fontY(), ttoa(l, 0, 7), _frontColour);
			} // _gridY
		} // lapse error
	}
	else {
		_pscreen->setBackGroundColour(_frontColour);
		_pscreen->gText(_x0+_dx-2-7*_pscreen->fontX(), _y0+_dy-2-_pscreen->fontY(), ttoa(l, 0, 7), _backColour);
		_pscreen->setBackGroundColour(_backColour);
	} // end _lapse>0
	
}
// end gHistogram



gGauge::gGauge() {
	;
}

void gGauge::dDefine(Serial_LCD * lcd0, uint16_t x0, uint16_t y0, uint16_t dx, uint16_t dy, float valueMin, float valueMax, uint16_t memory, uint16_t grid, uint16_t backColour, uint16_t frontColour, uint16_t gridColour, uint16_t valueColour, uint16_t minColour, uint16_t maxColour) {
	uint16_t radius = min(dx, dy)/2;
	define(lcd0, x0+dx/2, y0+dy/2, radius-2, valueMin, valueMax, memory, grid, backColour, frontColour, gridColour, valueColour, minColour, maxColour);
}

void gGauge::define(Serial_LCD * lcd0, uint16_t x0, uint16_t y0, uint16_t radius, float valueMin, float valueMax, uint16_t memory, uint16_t grid, uint16_t backColour, uint16_t frontColour, uint16_t gridColour, uint16_t valueColour, uint16_t minColour, uint16_t maxColour) {
	_pscreen = lcd0;
	
	_x0 = x0; 
	_y0 = y0; 
	_radius = radius;
	_oldZ = 0.0;
	
	_backColour  = backColour;
	_frontColour = frontColour;
	_gridColour  = gridColour;
	_valueColour = valueColour;
	_minColour   = minColour;
	_maxColour   = maxColour;
	
	_valueMin  = valueMin;
	_valueMax  = valueMax;
	_memory    = memory; 
	_grid      = grid;
	
	_pscreen->setPenSolid(true);
	_pscreen->circle(_x0, _y0, _radius, _backColour); 
	
	_pscreen->setPenSolid(false);
	_pscreen->circle(_x0, _y0, _radius, _gridColour); 
	
	_n   = 0; // number of values shown
	_min = 0; // y coordinate of min for memory
	_max = 0; // y coordinate of max for memory
	_amnesiaMin=0;
	_amnesiaMax=0;
	
	_pscreen->setPenSolid(false);
	if (_grid>0) for (uint16_t i=0; i<=_grid; i++) {
		_pscreen->circle(_x0-(_radius-2)*cos( (7.0+6.0*i/_grid)*PI/4.0 ), _y0-(_radius-2)*sin( (7.0+6.0*i/_grid)*PI/4.0 ), 2, _gridColour); 
	}
}


void gGauge::draw(float value, String sValue) {
	
	float z = ( 7.0+6.0*(value-_valueMin)/(_valueMax-_valueMin) )*PI/4.0;
	if ( z<PI* 7.0/4.0 )   z = PI* 7.0/4.0;
	if ( z>PI*13.0/4.0 )   z = PI*13.0/4.0;
	float k = min(_radius*0.8, _radius-8-3);
	float l = 0;
	uint16_t vX  = _x0-k*cos(z);
	uint16_t vY  = _y0-k*sin(z);
	uint16_t _vX = _x0-k*cos(_oldZ);
	uint16_t _vY = _y0-k*sin(_oldZ);
	
	_oldZ = z;
	if ( (vX==_vX) && (vY==_vY) ) return;
	
	// value
	_pscreen->line(_x0-1, _y0-1, vX-1, vY-1, _valueColour);
	_pscreen->line(_x0-1, _y0,   vX-1, vY,   _valueColour);
	_pscreen->line(_x0-1, _y0+1, vX-1, vY+1, _valueColour);
	//    _pscreen->line(_x0,   _y0-1, vX,   vY-1, _valueColour);
	//    _pscreen->line(_x0,   _y0,   vX,   vY,   _valueColour);
	//    _pscreen->line(_x0,   _y0+1, vX,   vY+1, _valueColour);
	_pscreen->line(_x0+1, _y0-1, vX+1, vY-1, _valueColour);
	_pscreen->line(_x0+1, _y0,   vX+1, vY,   _valueColour);
	_pscreen->line(_x0+1, _y0+1, vX+1, vY+1, _valueColour);
	
	_pscreen->line(_x0-1, _y0-1, _vX-1, _vY-1, _backColour);
	_pscreen->line(_x0-1, _y0,   _vX-1, _vY,   _backColour);
	_pscreen->line(_x0-1, _y0+1, _vX-1, _vY+1, _backColour);
	//    _pscreen->line(_x0,   _y0-1, _vX,   _vY-1, _backColour);
	//    _pscreen->line(_x0,   _y0,   _vX,   _vY,   _backColour);
	//    _pscreen->line(_x0,   _y0+1, _vX,   _vY+1, _backColour);
	_pscreen->line(_x0+1, _y0-1, _vX+1, _vY-1, _backColour);
	_pscreen->line(_x0+1, _y0,   _vX+1, _vY,   _backColour);
	_pscreen->line(_x0+1, _y0+1, _vX+1, _vY+1, _backColour);
	
	_pscreen->line(_x0-1, _y0-1, vX-1, vY-1, _valueColour);
	_pscreen->line(_x0-1, _y0,   vX-1, vY,   _valueColour);
	_pscreen->line(_x0-1, _y0+1, vX-1, vY+1, _valueColour);
	//    _pscreen->line(_x0,   _y0-1, vX,   vY-1, _valueColour);
	//    _pscreen->line(_x0,   _y0,   vX,   vY,   _valueColour);
	//    _pscreen->line(_x0,   _y0+1, vX,   vY+1, _valueColour);
	_pscreen->line(_x0+1, _y0-1, vX+1, vY-1, _valueColour);
	_pscreen->line(_x0+1, _y0,   vX+1, vY,   _valueColour);
	_pscreen->line(_x0+1, _y0+1, vX+1, vY+1, _valueColour);
	
	// min and max memory
	if (_memory>0) {
		_pscreen->setPenSolid(true);
		// first time
		if (_n==0) {
			_max = z;
			_min = z;
			_n++;
		}
		
		boolean fMax = false;
		boolean fMin = false;
		
		// max - coordinates in normal scale
		if (z>=_max) {
			_pscreen->circle(_x0-(_radius-8)*cos(_max), _y0-(_radius-8)*sin(_max), 2, _backColour);
			_max = z;
			_amnesiaMax = _memory;
		} 
		else if (_amnesiaMax>1) {
			fMax = true;
			_amnesiaMax--;
		} 
		else {
			_pscreen->circle(_x0-(_radius-8)*cos(_max), _y0-(_radius-8)*sin(_max), 2, _backColour);
			_max = z;
		} // max
		
		// min - coordinates in normal scale
		if (z<=_min) {
			_pscreen->circle(_x0-(_radius-8)*cos(_min), _y0-(_radius-8)*sin(_min), 2, _backColour);
			_min = z;
			_amnesiaMin = _memory;
		} 
		else if (_amnesiaMin>0) {
			fMin = true;
			_amnesiaMin--;
		} 
		else {
			_pscreen->circle(_x0-(_radius-8)*cos(_min), _y0-(_radius-8)*sin(_min), 2, _backColour);
			_min = z;
		} // min
		
		if (fMin)       _pscreen->circle(_x0-(_radius-8)*cos(_min), _y0-(_radius-8)*sin(_min), 2, _minColour);
		if (fMax)       _pscreen->circle(_x0-(_radius-8)*cos(_max), _y0-(_radius-8)*sin(_max), 2, _maxColour);
	} // min and max memory
	
	// min and max values
	_pscreen->setFont(0);
	_pscreen->setFontSolid(true);
	_pscreen->gText(_x0-_radius, _y0+_radius-_pscreen->fontY(), ftoa(_valueMin, 1, 0), _frontColour);
	_pscreen->gText(_x0+_radius-_pscreen->fontX()*ftoa(_valueMax, 1, 0).length(), _y0+_radius-_pscreen->fontY(), ftoa(_valueMax, 1, 0), _frontColour);
	
	// string display
	if (sValue!="") {
		_pscreen->setPenSolid(false);
		_pscreen->setFontSolid(true);
		
		uint8_t i=4;
		do {
			i--;
			_pscreen->setFont(i);
		} 
		while ( (sValue.length()*_pscreen->fontX() > (2*_radius*0.80)) && (i>0) );
		
		String _s = sValue.substring(0, min(sValue.length(), (2*_radius*0.80) / _pscreen->fontX()));
		k = sValue.length()*_pscreen->fontX()/2.0;
		l = _radius*0.90-_pscreen->fontY();
		
		_pscreen->dRectangle(_x0-k-1, _y0+l-2, 2*k+3, _pscreen->fontY()+3, _gridColour);
		_pscreen->gText(_x0-k, _y0+l, _s, _frontColour);
	} // string display
	
	_pscreen->setPenSolid(true);
	_pscreen->circle(_x0, _y0, 3, _valueColour); 
	
	//// debug
	//  Serial.print(value, 2);
	//  Serial.print("\t>");
	//  Serial.print(z, 2);
	//  Serial.print("\t min:");
	//  Serial.print(_min, 2);
	//  Serial.print("\t(");
	//  Serial.print(_amnesiaMin, DEC);
	//  Serial.print(")\t max:");
	//  Serial.print(_max, 2);
	//  Serial.print("\t(");
	//  Serial.print(_amnesiaMax, DEC);
	//  Serial.print(")\n");
}












