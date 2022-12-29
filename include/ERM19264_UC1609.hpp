/*
* Project Name: ERM19264_UC1609_RPI
* File: ERM19264_UC1609.h
* Description: ERM19264 LCD driven by UC1609C controller header file
* Author: Gavin Lyons.
* URL: https://github.com/gavinlyonsrepo/ERM19264_UC1609_RPI
*/

#ifndef ERM19264_UC1609_H
#define ERM19264_UC1609_H


// ** INCLUDES **
#include <bcm2835.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "ERM19264_graphics.hpp"

// Display Pixel colours   definition
// (1): white on blue,  FG = white BG = blue
// ERM19264SBS-4 LCD Display White on Blue
// (2): black on white, FG = black BG = white
// ERM19264FS-4 LCD Display Black on White
// (3):white on black,  FG = white, BG = black
// ERM19264DNS-4LCD Display White on Black
#define FOREGROUND  0
#define BACKGROUND 1
#define INVERSE 2

// UC1909 Write registers
#define UC1609_SYSTEM_RESET 0xE2

#define UC1609_POWER_CONTROL 0x28
#define UC1609_PC_SET 0x06 // PC[2:0] 110 Internal V LCD (7x charge pump) + 10b: 1.4mA

#define UC1609_ADDRESS_CONTROL 0x88 // set RAM address control
#define UC1609_ADDRESS_SET 0x02 // Set AC [2:0] Program registers  for RAM address control.

#define UC1609_SET_PAGEADD 0xB0 // Page address Set PA[3:0]
#define UC1609_SET_COLADD_LSB 0x00 // Column Address Set CA [3:0]
#define UC1609_SET_COLADD_MSB 0x10 // Column Address Set CA [7:4]

#define UC1609_TEMP_COMP_REG 0x27 // Temperature Compensation Register
#define UC1609_TEMP_COMP_SET 0x00 // TC[1:0] = 00b= -0.00%/ C

#define UC1609_FRAMERATE_REG 0xA0 // Frame rate
#define UC1609_FRAMERATE_SET 0x01  // Set Frame Rate LC [4:3] 01b: 95 fps

#define UC1609_BIAS_RATIO 0xE8 // Bias Ratio. The ratio between V-LCD and V-D .
#define UC1609_BIAS_RATIO_SET 0x03 //  Set BR[1:0] = 11 (set to 9 default)

#define UC1609_GN_PM 0x81 // Set V BIAS Potentiometer to fine tune V-D and V-LCD  (double-byte command)
#define UC1609_DEFAULT_GN_PM 0x49 // default only used if user does not specify Vbias

#define UC1609_LCD_CONTROL 0xC0 // Rotate map control
#define UC1609_DISPLAY_ON 0xAE // enables display
#define UC1609_ALL_PIXEL_ON 0xA4 // sets on all Pixels on
#define UC1609_INVERSE_DISPLAY 0xA6 // inverts display
#define UC1609_SCROLL 0x40 // scrolls , Set the scroll line number. 0-64

// Rotate
#define UC1609_ROTATION_FLIP_TWO 0x06
#define UC1609_ROTATION_NORMAL 0x04
#define UC1609_ROTATION_FLIP_ONE 0x02
#define UC1609_ROTATION_FLIP_THREE 0x00

// Delays
#define UC1609_RESET_DELAY 3 // ms Delay
#define UC1609_RESET_DELAY2   0 // mS delay datasheet says 5mS, does not work
#define UC1609_INIT_DELAY 100   //  mS delay ,after init
#define UC1609_INIT_DELAY2 3 // mS delay,  before reset called
#define UC1609_HIGHFREQ_DELAY 0 // uS used in software SPI , option.

// GPIO
#define UC1609_CS_SetHigh bcm2835_gpio_write(_LCD_CS, HIGH)
#define UC1609_CS_SetLow bcm2835_gpio_write(_LCD_CS, LOW)
#define UC1609_CD_SetHigh bcm2835_gpio_write(_LCD_CD, HIGH)
#define UC1609_CD_SetLow bcm2835_gpio_write(_LCD_CD, LOW)
#define UC1609_RST_SetHigh bcm2835_gpio_write(_LCD_RST, HIGH)
#define UC1609_RST_SetLow bcm2835_gpio_write(_LCD_RST, LOW)
#define UC1609_SCLK_SetHigh bcm2835_gpio_write(_LCD_SCLK, HIGH)
#define  UC1609_SCLK_SetLow bcm2835_gpio_write(_LCD_SCLK, LOW)
#define  UC1609_SDA_SetHigh bcm2835_gpio_write(_LCD_DIN, HIGH)
#define  UC1609_SDA_SetLow bcm2835_gpio_write(_LCD_DIN, LOW)


struct MultiBuffer
{
  uint8_t* screenbitmap; // pointer to buffer
  uint8_t width=0;  // bitmap x size
  uint8_t height=0 ; // bitmap y size
  int16_t xoffset = 0; // x offset
  int16_t yoffset = 0; // y offset
};

//class
class ERM19264_UC1609 : public ERM19264_graphics {
  public:
	 // Contructor 1 Software SPI with explicit SCLK and SDIN
	ERM19264_UC1609(int16_t lcdwidth, int16_t lcdheight , int8_t rst, int8_t cd, int8_t cs,  int8_t sclk, int8_t din);
	// Constructor 2 Hardware SPI
	ERM19264_UC1609(int16_t lcdwidth, int16_t lcdheight , int8_t rst, int8_t cd);

	~ERM19264_UC1609(){};

   MultiBuffer* ActiveBuffer= NULL;

	virtual void drawPixel(int16_t x, int16_t y, uint8_t colour) override;
	void LCDupdate(void);
	void LCDclearBuffer(void);
	void LCDBuffer(int16_t x, int16_t y, uint8_t w, uint8_t h, uint8_t* data);
	void LCDbegin(uint8_t VbiasPot = UC1609_DEFAULT_GN_PM );
	void LCDinit(void);
	void LCDEnable(uint8_t on);
	void LCDFillScreen(uint8_t pixel);
	void LCDFillPage(uint8_t page_num, uint8_t pixels);
	void LCDrotate(uint8_t rotatevalue);
	void LCDinvert(uint8_t on);
	void LCDallpixelsOn(uint8_t bits);
	void LCDscroll(uint8_t bits);
	void LCDReset(void);
	void LCDBitmap(int16_t x, int16_t y, uint8_t w, uint8_t h, const uint8_t* data);
	void LCDPowerDown(void);
	void LCDSPIon(void);
	void LCDSPIoff(void);
	bool LCDIssleeping(void);
	void LCDinitBufferStruct(MultiBuffer *p, uint8_t* mybuffer, 
								uint8_t w, uint8_t h, int16_t x, int16_t y); 

  private:

	void send_data(uint8_t data);
	void send_command(uint8_t command, uint8_t value);
	int8_t GetCommMode(void);
	void CustomshiftOut(uint8_t val);

	int8_t _LCD_CS;
	int8_t _LCD_CD;
	int8_t _LCD_RST;
	int8_t _LCD_SCLK; // Software SPI only
	int8_t _LCD_DIN;  // Software SPI only

	uint8_t _VbiasPOT; // Contrast default 0x49, range 00-FE
	int16_t _LCD_WIDTH =192;
	int16_t _LCD_HEIGHT=64;
	int8_t _LCD_PAGE_NUM = (_LCD_HEIGHT/8);
	int8_t _LCD_mode = 2; // 2 = HW SPI 3 = SW SPI, other numbers reserved for future use
	bool _sleep = true; // False = awake/ON , true = sleep/OFF
};

#endif
