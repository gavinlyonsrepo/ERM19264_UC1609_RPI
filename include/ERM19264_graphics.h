/*
* Project Name: ERM19264_UC1609
* File: ERM19264_graphics.h
* Description: ERM19264 LCD driven by UC1609C controller header file for 
* the graphics functions
* Author: Gavin Lyons.
* URL: https://github.com/gavinlyonsrepo/ERM19264_UC1609
*/

#ifndef _ERM19264_GRAPHICS_H
#define _ERM19264_GRAPHICS_H

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "ERM19264_UC1609_Print.h"
#include "ERM19264_gfxfont.h"

#define swap(a, b) { int16_t t = a; a = b; b = t; }


class ERM19264_graphics : public Print {

 public:

	ERM19264_graphics(int16_t w, int16_t h); // Constructor

	// This MUST be defined by the subclass:
	virtual void drawPixel(int16_t x, int16_t y, uint8_t color) = 0;

	// These MAY be overridden by the subclass to provide device-specific
	// optimized code.  Otherwise 'generic' versions are used.
	virtual void
	drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint8_t color),
	drawFastVLine(int16_t x, int16_t y, int16_t h, uint8_t color),
	drawFastHLine(int16_t x, int16_t y, int16_t w, uint8_t color),
	drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint8_t color),
	fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint8_t color),
	fillScreen(uint8_t color);

	// no subclass overrides
	void
	drawCircle(int16_t x0, int16_t y0, int16_t r, uint8_t color),
	drawCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername,
		uint8_t color),
	fillCircle(int16_t x0, int16_t y0, int16_t r, uint8_t color),
	fillCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername,
		int16_t delta, uint8_t color),
	drawTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1,
		int16_t x2, int16_t y2, uint8_t color),
	fillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1,
		int16_t x2, int16_t y2, uint8_t color),
	drawRoundRect(int16_t x0, int16_t y0, int16_t w, int16_t h,
		int16_t radius, uint8_t color),
	fillRoundRect(int16_t x0, int16_t y0, int16_t w, int16_t h,
		int16_t radius, uint8_t color),
	drawChar(int16_t x, int16_t y, unsigned char c, uint8_t color,
		uint8_t bg, uint8_t size),
	setCursor(int16_t x, int16_t y),
	setTextColor(uint8_t c),
	setTextColor(uint8_t c, uint8_t bg),
	setTextSize(uint8_t s),
	setTextWrap(bool w),
	setFontNum(uint8_t FontNumber); 
	
void setFontGlyph(const GFXfont *f);
void drawCharGlyph(uint8_t x, uint8_t y, uint8_t c, uint8_t color,
				uint8_t size);
				
void drawTextGlyph(uint8_t x, uint8_t y, const char *_text,
					uint8_t color,uint8_t size); 
						 
	virtual size_t write(uint8_t);
	int16_t height(void) const;
	int16_t width(void) const;

 protected:
	GFXfont *_gfxFont= NULL;
	const int16_t
	WIDTH, HEIGHT;
	int16_t
	_width, _height,
	cursor_x, cursor_y;
	uint8_t
	textcolor, textbgcolor;
	uint8_t   textsize;

	bool wrap; // If set, 'wrap' text at right edge of display

	uint8_t _FontNumber = 1;
	uint8_t _CurrentFontWidth = 5;
	uint8_t _CurrentFontoffset = 0;
	uint8_t _CurrentFontheight = 8;
};


#endif
