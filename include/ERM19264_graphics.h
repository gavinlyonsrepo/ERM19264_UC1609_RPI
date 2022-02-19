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

typedef enum 
{
	UC1609Font_Default = 1,
	UC1609Font_Thick = 2,
	UC1609Font_Seven_Seg = 3,
	UC1609Font_Wide = 4,
}LCD_FONT_TYPE_e;

typedef enum 
{
	FONT_W_5 = 5, FONT_W_7 = 7, FONT_W_4 = 4, FONT_W_8 = 8,FONT_W_16= 16
}LCD_Font_width_e; // width of the font in bytes cols.

typedef enum 
{
	FONT_O_EXTEND = 0x00, //   extends ASCII
	FONT_O_SP = 0x20,  // Starts at Space
	FONT_O_NUM = 0x30,  // Starts at number '0'
}LCD_Font_offset_e; // font offset in the ASCII table

typedef enum 
{
	FONT_H_8 = 8, FONT_H_16 = 16, FONT_H_32 = 32
}LCD_Font_height_e; // width of the font in bits

class ERM19264_graphics : public Print {

 public:

	ERM19264_graphics(int16_t w, int16_t h); // Constructor

	// This MUST be defined by the subclass:
	virtual void drawPixel(int16_t x, int16_t y, uint8_t color) = 0;

	void drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint8_t color);
	void drawFastVLine(int16_t x, int16_t y, int16_t h, uint8_t color);
	void drawFastHLine(int16_t x, int16_t y, int16_t w, uint8_t color);
	void drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint8_t color);
	void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint8_t color);
	void fillScreen(uint8_t color);
	
	void drawCircle(int16_t x0, int16_t y0, int16_t r, uint8_t color);
	void drawCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername,
		uint8_t color);
	void fillCircle(int16_t x0, int16_t y0, int16_t r, uint8_t color);
	void fillCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername,
		int16_t delta, uint8_t color);
	void drawTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1,
		int16_t x2, int16_t y2, uint8_t color);
	void fillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1,
		int16_t x2, int16_t y2, uint8_t color);
	void drawRoundRect(int16_t x0, int16_t y0, int16_t w, int16_t h,
		int16_t radius, uint8_t color);
	void fillRoundRect(int16_t x0, int16_t y0, int16_t w, int16_t h,
		int16_t radius, uint8_t color);
	void drawChar(int16_t x, int16_t y, unsigned char c, uint8_t color,
		uint8_t bg, uint8_t size);
	void setCursor(int16_t x, int16_t y);
	void setTextColor(uint8_t c);
	void setTextColor(uint8_t c, uint8_t bg);
	void setTextSize(uint8_t s);
	void setTextWrap(bool w);
	void setFontNum(LCD_FONT_TYPE_e FontNumber); 
	void setDrawBitmapAddr(bool mode);
	void drawBitmap(int16_t x, int16_t y, const uint8_t *bitmap,
			int16_t w, int16_t h, uint8_t color, uint8_t bg);
	
	void setFontGlyph(const GFXfont *f);
	void drawCharGlyph(uint8_t x, uint8_t y, uint8_t c, uint8_t color,
				uint8_t size);
				
	void drawTextGlyph(uint8_t x, uint8_t y, const char *_text,
					uint8_t color,uint8_t size); 
						 
	virtual size_t write(uint8_t);
	int16_t height(void) const;
	int16_t width(void) const;
	LCD_FONT_TYPE_e FontNum;

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
	bool drawBitmapAddr; // True = vertical , false = horizontal
	
	uint8_t _FontNumber = UC1609Font_Default;
	uint8_t _CurrentFontWidth = FONT_W_5;
	uint8_t _CurrentFontoffset = FONT_O_EXTEND ;
	uint8_t _CurrentFontheight = FONT_H_8;
};


#endif
