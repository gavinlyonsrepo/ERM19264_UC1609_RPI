
/*
* Project Name: ERM19264_UC1609_RPI
* File:ERM19264_graphics.cpp
* Description: ERM19264 LCD driven by UC1609C controller header 
* file for the ERM19264 graphics functions 
* Author: Gavin Lyons.
* URL: https://github.com/gavinlyonsrepo/ERM19264_UC1609_RPI
*/

#include <stdbool.h>
#include "ERM19264_UC1609.hpp"
#include "ERM19264_graphics.hpp"
#include "ERM19264_font.hpp"


ERM19264_graphics::ERM19264_graphics(int16_t w, int16_t h):
  WIDTH(w), HEIGHT(h)
{
  _width    = WIDTH;
  _height   = HEIGHT;
  cursor_y  = cursor_x    = 0;
  textsize  = 1;
  textcolor = textbgcolor = 0xFF;
  wrap      = true;
  drawBitmapAddr = true; // True = vertical , false = horizontal
}

// Draw a circle outline
void ERM19264_graphics::drawCircle(int16_t x0, int16_t y0, int16_t r,
	uint8_t color) {
  int16_t f = 1 - r;
  int16_t ddF_x = 1;
  int16_t ddF_y = -2 * r;
  int16_t x = 0;
  int16_t y = r;

  drawPixel(x0  , y0+r, color);
  drawPixel(x0  , y0-r, color);
  drawPixel(x0+r, y0  , color);
  drawPixel(x0-r, y0  , color);

  while (x<y) {
	if (f >= 0) {
	  y--;
	  ddF_y += 2;
	  f += ddF_y;
	}
	x++;
	ddF_x += 2;
	f += ddF_x;

	drawPixel(x0 + x, y0 + y, color);
	drawPixel(x0 - x, y0 + y, color);
	drawPixel(x0 + x, y0 - y, color);
	drawPixel(x0 - x, y0 - y, color);
	drawPixel(x0 + y, y0 + x, color);
	drawPixel(x0 - y, y0 + x, color);
	drawPixel(x0 + y, y0 - x, color);
	drawPixel(x0 - y, y0 - x, color);
  }
}

void ERM19264_graphics::drawCircleHelper( int16_t x0, int16_t y0,
			   int16_t r, uint8_t cornername, uint8_t color) {
  int16_t f     = 1 - r;
  int16_t ddF_x = 1;
  int16_t ddF_y = -2 * r;
  int16_t x     = 0;
  int16_t y     = r;

  while (x<y) {
	if (f >= 0) {
	  y--;
	  ddF_y += 2;
	  f     += ddF_y;
	}
	x++;
	ddF_x += 2;
	f     += ddF_x;
	if (cornername & 0x4) {
	  drawPixel(x0 + x, y0 + y, color);
	  drawPixel(x0 + y, y0 + x, color);
	}
	if (cornername & 0x2) {
	  drawPixel(x0 + x, y0 - y, color);
	  drawPixel(x0 + y, y0 - x, color);
	}
	if (cornername & 0x8) {
	  drawPixel(x0 - y, y0 + x, color);
	  drawPixel(x0 - x, y0 + y, color);
	}
	if (cornername & 0x1) {
	  drawPixel(x0 - y, y0 - x, color);
	  drawPixel(x0 - x, y0 - y, color);
	}
  }
}

void ERM19264_graphics::fillCircle(int16_t x0, int16_t y0, int16_t r,
				  uint8_t color) {
  drawFastVLine(x0, y0-r, 2*r+1, color);
  fillCircleHelper(x0, y0, r, 3, 0, color);
}

// Used to do circles and roundrects
void ERM19264_graphics::fillCircleHelper(int16_t x0, int16_t y0, int16_t r,
	uint8_t cornername, int16_t delta, uint8_t color) {

  int16_t f     = 1 - r;
  int16_t ddF_x = 1;
  int16_t ddF_y = -2 * r;
  int16_t x     = 0;
  int16_t y     = r;

  while (x<y) {
	if (f >= 0) {
	  y--;
	  ddF_y += 2;
	  f     += ddF_y;
	}
	x++;
	ddF_x += 2;
	f     += ddF_x;

	if (cornername & 0x1) {
	  drawFastVLine(x0+x, y0-y, 2*y+1+delta, color);
	  drawFastVLine(x0+y, y0-x, 2*x+1+delta, color);
	}
	if (cornername & 0x2) {
	  drawFastVLine(x0-x, y0-y, 2*y+1+delta, color);
	  drawFastVLine(x0-y, y0-x, 2*x+1+delta, color);
	}
  }
}

void ERM19264_graphics::drawLine(int16_t x0, int16_t y0,
				int16_t x1, int16_t y1,
				uint8_t color) {
  int16_t steep = abs(y1 - y0) > abs(x1 - x0);
  if (steep) {
	swap(x0, y0);
	swap(x1, y1);
  }

  if (x0 > x1) {
	swap(x0, x1);
	swap(y0, y1);
  }

  int16_t dx, dy;
  dx = x1 - x0;
  dy = abs(y1 - y0);

  int16_t err = dx / 2;
  int16_t ystep;

  if (y0 < y1) {
	ystep = 1;
  } else {
	ystep = -1;
  }

  for (; x0<=x1; x0++) {
	if (steep) {
	  drawPixel(y0, x0, color);
	} else {
	  drawPixel(x0, y0, color);
	}
	err -= dy;
	if (err < 0) {
	  y0 += ystep;
	  err += dx;
	}
  }
}

// Draw a rectangle
void ERM19264_graphics::drawRect(int16_t x, int16_t y,
				int16_t w, int16_t h,
				uint8_t color) {
  drawFastHLine(x, y, w, color);
  drawFastHLine(x, y+h-1, w, color);
  drawFastVLine(x, y, h, color);
  drawFastVLine(x+w-1, y, h, color);
}

void ERM19264_graphics::drawFastVLine(int16_t x, int16_t y,
				 int16_t h, uint8_t color) {
  drawLine(x, y, x, y+h-1, color);
}

void ERM19264_graphics::drawFastHLine(int16_t x, int16_t y,
				 int16_t w, uint8_t color) {
  drawLine(x, y, x+w-1, y, color);
}

void ERM19264_graphics::fillRect(int16_t x, int16_t y, int16_t w, int16_t h,
				uint8_t color) {
  for (int16_t i=x; i<x+w; i++) {
	drawFastVLine(i, y, h, color);
  }
}

void ERM19264_graphics::fillScreen(uint8_t color) {
  fillRect(0, 0, _width, _height, color);
}

// Draw a rounded rectangle
void ERM19264_graphics::drawRoundRect(int16_t x, int16_t y, int16_t w,
  int16_t h, int16_t r, uint8_t color) {
  drawFastHLine(x+r  , y    , w-2*r, color); // Top
  drawFastHLine(x+r  , y+h-1, w-2*r, color); // Bottom
  drawFastVLine(x    , y+r  , h-2*r, color); // Left
  drawFastVLine(x+w-1, y+r  , h-2*r, color); // Right
  // draw four corners
  drawCircleHelper(x+r    , y+r    , r, 1, color);
  drawCircleHelper(x+w-r-1, y+r    , r, 2, color);
  drawCircleHelper(x+w-r-1, y+h-r-1, r, 4, color);
  drawCircleHelper(x+r    , y+h-r-1, r, 8, color);
}

// Fill a rounded rectangle
void ERM19264_graphics::fillRoundRect(int16_t x, int16_t y, int16_t w,
				 int16_t h, int16_t r, uint8_t color) {
  fillRect(x+r, y, w-2*r, h, color);

  fillCircleHelper(x+w-r-1, y+r, r, 1, h-2*r-1, color);
  fillCircleHelper(x+r    , y+r, r, 2, h-2*r-1, color);
}

// Draw a triangle
void ERM19264_graphics::drawTriangle(int16_t x0, int16_t y0,
				int16_t x1, int16_t y1,
				int16_t x2, int16_t y2, uint8_t color) {
  drawLine(x0, y0, x1, y1, color);
  drawLine(x1, y1, x2, y2, color);
  drawLine(x2, y2, x0, y0, color);
}

// Fill a triangle
void ERM19264_graphics::fillTriangle ( int16_t x0, int16_t y0,
				  int16_t x1, int16_t y1,
				  int16_t x2, int16_t y2, uint8_t color) {

  int16_t a, b, y, last;

  if (y0 > y1) {
	swap(y0, y1); swap(x0, x1);
  }
  if (y1 > y2) {
	swap(y2, y1); swap(x2, x1);
  }
  if (y0 > y1) {
	swap(y0, y1); swap(x0, x1);
  }

  if(y0 == y2) {
	a = b = x0;
	if(x1 < a)      a = x1;
	else if(x1 > b) b = x1;
	if(x2 < a)      a = x2;
	else if(x2 > b) b = x2;
	drawFastHLine(a, y0, b-a+1, color);
	return;
  }

  int16_t
	dx01 = x1 - x0,
	dy01 = y1 - y0,
	dx02 = x2 - x0,
	dy02 = y2 - y0,
	dx12 = x2 - x1,
	dy12 = y2 - y1;
  int32_t
	sa   = 0,
	sb   = 0;

  if(y1 == y2) last = y1;   // Include y1 scanline
  else         last = y1-1; // Skip it

  for(y=y0; y<=last; y++) {
	a   = x0 + sa / dy01;
	b   = x0 + sb / dy02;
	sa += dx01;
	sb += dx02;
	/* longhand:
	a = x0 + (x1 - x0) * (y - y0) / (y1 - y0);
	b = x0 + (x2 - x0) * (y - y0) / (y2 - y0);
	*/
	if(a > b) swap(a,b);
	drawFastHLine(a, y, b-a+1, color);
  }

  sa = dx12 * (y - y1);
  sb = dx02 * (y - y0);
  for(; y<=y2; y++) {
	a   = x1 + sa / dy12;
	b   = x0 + sb / dy02;
	sa += dx12;
	sb += dx02;
	if(a > b) swap(a,b);
	drawFastHLine(a, y, b-a+1, color);
  }
}


size_t ERM19264_graphics::write(uint8_t c) {
  
  if (c == '\n') {
	cursor_y += textsize*_CurrentFontheight;
	cursor_x  = 0;
  } else if (c == '\r') {
	// skip em
  } else {
	drawChar(cursor_x, cursor_y, c, textcolor, textbgcolor, textsize);
	cursor_x += textsize*(_CurrentFontWidth+1);
	if (wrap && (cursor_x > (_width - textsize*(_CurrentFontWidth+1)))) {
	  cursor_y += textsize*_CurrentFontheight;
	  cursor_x = 0;
	}
  }
  return 1;
}

// Draw a character
void ERM19264_graphics::drawChar(int16_t x, int16_t y, unsigned char c,
				uint8_t color, uint8_t bg, uint8_t size) {

  if((x >= _width)            || // Clip right
	 (y >= _height)           || // Clip bottom
	 ((x + (_CurrentFontWidth+1) * size - 1) < 0) || // Clip left
	 ((y + _CurrentFontheight  * size - 1) < 0))   // Clip top
	return;

  for (int8_t i=0; i<(_CurrentFontWidth+1); i++ ) {
	uint8_t line;
	if (i == _CurrentFontWidth)
	{
	  line = 0x0;
	}
	else
	{
	switch (_FontNumber) 
	{
		case UC1609Font_Default:
			line = pFontDefaultptr[(c - _CurrentFontoffset) * _CurrentFontWidth + i];
		break;
		case UC1609Font_Thick:
			line = pFontThickptr[(c - _CurrentFontoffset) * _CurrentFontWidth + i];
		break;
		case UC1609Font_Seven_Seg:
			line = pFontSevenSegptr[(c - _CurrentFontoffset) * _CurrentFontWidth + i];
		break;
		case UC1609Font_Wide:
			line = pFontWideptr[(c - _CurrentFontoffset) * _CurrentFontWidth + i];
		break;
		case UC1609Font_Tiny:
			line =pFontTinyptr[(c - _CurrentFontoffset) * _CurrentFontWidth + i];
		break;
		case UC1609Font_Homespun:
			line = pFontHomeSpunptr[(c - _CurrentFontoffset) * _CurrentFontWidth + i];
		break;
		default:
			printf("Error: Wrong font number ,must be 1-4\n");
			return;
		break;
	} //switch font linenumber
	}
	for (int8_t j = 0; j<_CurrentFontheight; j++) {
	  if (line & 0x1) {
		if (size == 1) // default size
		  drawPixel(x+i, y+j, color);
		else {  // big size
		  fillRect(x+(i*size), y+(j*size), size, size, color);
		}
	  } else if (bg != color) {
		if (size == 1) // default size
		  drawPixel(x+i, y+j, bg);
		else {  // big size
		  fillRect(x+i*size, y+j*size, size, size, bg);
		}
	  }
	  line >>= 1;
	}
  }

}


void ERM19264_graphics::setCursor(int16_t x, int16_t y) {
  cursor_x = x;
  cursor_y = y;
}

void ERM19264_graphics::setTextSize(uint8_t s) {
  textsize = (s > 0) ? s : 1;
}

void ERM19264_graphics::setTextColor(uint8_t c) {
  textcolor = textbgcolor = c;
}

void ERM19264_graphics::setTextColor(uint8_t c, uint8_t b) {
  textcolor   = c;
  textbgcolor = b;
}

void ERM19264_graphics::setTextWrap(bool w) {
  wrap = w;
}

// Return the size of the display
int16_t ERM19264_graphics::width(void) const {
  return _width;
}

int16_t ERM19264_graphics::height(void) const {
  return _height;
}


void ERM19264_graphics::setFontGlyph(const GFXfont *f) {
  _gfxFont = (GFXfont *)f;
}

// Draw text character array to screen
void ERM19264_graphics::drawTextGlyph(uint8_t x, uint8_t y, const char *_text,
						 uint8_t color, uint8_t size) {
  uint8_t cursor_x, cursor_y, first_char, last_char;
  uint16_t textlen, i;

  cursor_x = x, cursor_y = y;
  first_char = _gfxFont->first;
  last_char  = _gfxFont->last;
  textlen    = strlen(_text);

  for(i = 0; i < textlen; i++){
	uint8_t c = _text[i];
	if (c<first_char || c>last_char) {
	  continue;
	}

	GFXglyph *glyph = &(_gfxFont->glyph[c-first_char]);
	uint8_t w = glyph->width, h = glyph->height;

	if((w > 0) && (h > 0)) { // bitmap available
	  int16_t xo = glyph->xOffset;
	  if(wrap && ((cursor_x + size * (xo + w)) > _width)) {
		cursor_x = 0;
		cursor_y += (int16_t)size * _gfxFont->yAdvance;
	  }
	  drawCharGlyph(cursor_x,cursor_y,c,color,size);
	  
	}
	cursor_x += glyph->xAdvance * (int16_t)size;
  }
}

void ERM19264_graphics::drawCharGlyph(uint8_t x, uint8_t y, uint8_t c, uint8_t color,
			  uint8_t size) {
  c -= (uint8_t) (_gfxFont->first);
  GFXglyph *glyph = _gfxFont->glyph + c;
  uint8_t *bitmap = _gfxFont->bitmap;

  uint16_t bo = glyph->bitmapOffset;
  uint8_t w   = glyph->width, h = glyph->height;
  int8_t xo   = glyph->xOffset, yo = glyph->yOffset;
  uint8_t xx, yy, bits = 0, abit = 0;
  int16_t xo16 = 0, yo16 = 0;

  if (size > 1) {
	xo16 = xo;
	yo16 = yo;
  }

  for (yy = 0; yy < h; yy++) {
	for (xx = 0; xx < w; xx++) {
	  if (!(abit++ & 7)) {
		bits = bitmap[bo++];
	  }
	  if (bits & 0x80) {
		if (size == 1) {
		  drawPixel(x+xo+xx, y+yo+yy,color);
		} else {
		  fillRect(x+(xo16+xx)*size,y+(yo16+yy)*size,size,size,color);
		}
	  }
	  bits <<= 1;
	}
  }
}

// Desc :  Set the font number
// Param1: fontnumber 1-6
// 1=default 2=thick 3=seven segment 4=wide 5=tiny 6=homespun

void ERM19264_graphics::setFontNum(LCDFontType_e FontNumber) 
{
	_FontNumber = FontNumber;
		
	switch (_FontNumber) 
	{
		case UC1609Font_Default:  // Norm default 5 by 8
			_CurrentFontWidth = UC1609FontWidth_5;
			_CurrentFontoffset =  UC1609FontOffset_Extend;
			_CurrentFontheight = UC1609FontHeight_8;
		break; 
		case UC1609Font_Thick: // Thick 7 by 8 (NO LOWERCASE LETTERS)
			_CurrentFontWidth = UC1609FontWidth_7;
			_CurrentFontoffset = UC1609FontOffset_Space;
			_CurrentFontheight = UC1609FontHeight_8;
		break; 
		case UC1609Font_Seven_Seg:  // Seven segment 4 by 8
			_CurrentFontWidth = UC1609FontWidth_4;
			_CurrentFontoffset = UC1609FontOffset_Space;
			_CurrentFontheight = UC1609FontHeight_8;
		break;
		case UC1609Font_Wide : // Wide  8 by 8 (NO LOWERCASE LETTERS)
			_CurrentFontWidth = UC1609FontWidth_8;
			_CurrentFontoffset = UC1609FontOffset_Space;
			_CurrentFontheight = UC1609FontHeight_8;
		break; 
		case UC1609Font_Tiny:  // tiny 3 by 8
			_CurrentFontWidth = UC1609FontWidth_3;
			_CurrentFontoffset =  UC1609FontOffset_Space;
			_CurrentFontheight = UC1609FontHeight_8;
		break;
		case UC1609Font_Homespun: // homespun 7 by 8 
			_CurrentFontWidth = UC1609FontWidth_7;
			_CurrentFontoffset = UC1609FontOffset_Space;
			_CurrentFontheight = UC1609FontHeight_8;
		break;
		default: // if wrong font num passed in,  set to default
			printf("Error: Wrong font number ,must be 1-6\n");
		break;
	}
}

// Draw a 1-bit color bitmap at the specified x, y position from the
// provided bitmap buffer using colour as the
// foreground colour and bg as the background colour.
// Note: Variable drawBitmapAddr controls data addressing
// drawBitmapAddr  = true Vertical  data addressing
// drawBitmapAddr  = false Horizontal data addressing
void ERM19264_graphics::drawBitmap(int16_t x, int16_t y,
						const uint8_t *bitmap, int16_t w, int16_t h,
						uint8_t color, uint8_t bg) {
							
if (drawBitmapAddr== true)
{
// Vertical byte bitmaps mode 
	uint8_t vline;
	int16_t i, j, r = 0, yin = y;
	
	for (i=0; i<(w+1); i++ ) {
		if (r == (h+7)/8 * w) break;
		vline = bitmap [ r] ;
		r++;
		if (i == w) {
			y = y+8;
			i = 0;
		}
		
		for (j=0; j<8; j++ ) {
			if (y+j-yin == h) break;
			if (vline & 0x1) {
				drawPixel(x+i, y+j, color);
			}
			else {
				drawPixel(x+i, y+j, bg);
			}	
			vline >>= 1;
		}
	}
} else if (drawBitmapAddr == false) {
// Horizontal byte bitmaps mode 
	int16_t byteWidth = (w + 7) / 8;
	uint8_t byte = 0;
	for (int16_t j = 0; j < h; j++, y++) 
	{
		for (int16_t i = 0; i < w; i++) 
		{
			if (i & 7)
				byte <<= 1;
			else
				byte = bitmap[j * byteWidth + i / 8];
			drawPixel(x+i, y, (byte & 0x80) ? color : bg);
		}
	}

} // end of elseif
} // end of function

//Func Desc : sets the data addressing mode in drawBitmap function.
//Param 1 boolean mode  , true default
// True =  bitmap data vertically addressed 
// False = bitmap data horizontally addressed 
void ERM19264_graphics::setDrawBitmapAddr(bool mode) {
	drawBitmapAddr = mode;
}

// Desc: Writes text string (*ptext) on the LCD 
// Param 1 , 2 : coordinates (x, y).
// Param 3: pointer to string 
// Param 4: color 
// Param 5: background color
// Notes for font 1- 6 only
void ERM19264_graphics::drawText(uint8_t x, uint8_t y, char *pText, uint8_t color, uint8_t bg, uint8_t size) {
	
	uint8_t cursor_x, cursor_y;
	cursor_x = x, cursor_y = y;
	
	while (*pText != '\0') 
	{
		if (wrap && ((cursor_x + size * _CurrentFontWidth) > _width)) 
		{
			cursor_x = 0;
			cursor_y = cursor_y + size * 7 + 3;
			if (cursor_y > _height) cursor_y = _height;
		}
		drawChar(cursor_x, cursor_y, *pText, color, bg, size);
		cursor_x = cursor_x + size * (_CurrentFontWidth + 1);
		if (cursor_x > _width) cursor_x = _width;
		pText++;
	}
}

//*********************** EOF ********************
