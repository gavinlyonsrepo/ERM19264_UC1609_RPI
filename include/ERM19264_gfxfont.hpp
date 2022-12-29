// Font structures for glyph fonts
// Example fonts are included in include directory.
// To use a font in your program, #include the corresponding .h
// file and pass address of GFXfont struct to setFont().  

#ifndef _ERM_GFXFONT_H_
#define _ERM_GFXFONT_H_

///Font data stored per glyph
typedef struct {
  uint16_t bitmapOffset; // Pointer into GFXfont->bitmap
  uint8_t width;         // Bitmap dimensions in pixels
  uint8_t height;        // bitmap dimensions in pixels
  uint8_t xAdvance;      // Distance to advance cursor (x axis)
  int8_t xOffset;        // X dist from cursor pos to UL corner
  int8_t yOffset;        // Y dist from cursor pos to UL corner
} GFXglyph;

// Data stored for font as a whole
typedef struct {
  uint8_t *bitmap;  // Glyph bitmaps, concatenated
  GFXglyph *glyph;  // Glyph array
  uint16_t first;   // ASCII extents (first char)
  uint16_t last;    // ASCII extents (last char)
  uint8_t yAdvance; // Newline distance (y axis)
} GFXfont;

#endif // _ERM_GFXFONT_H_
