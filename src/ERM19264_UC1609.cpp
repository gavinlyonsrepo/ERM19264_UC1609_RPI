/*
* Project Name: ERM19264_UC1609_RPI
* File: ERM19264_UC1609.cpp
* Description: ERM19264 LCD driven by UC1609C controller soucre file
* Author: Gavin Lyons.
* URL: https://github.com/gavinlyonsrepo/ERM19264_UC1609_RPI
*/

#include "ERM19264_UC1609.h"

// Class Constructors 4 off:
// (1) HWSPI no buffer
// (2) SWSPI no buffer
// (3) HWSPI buffer
// (4) SWSPI buffer

#ifdef NO_BUFFER
// Hardware SPI mode 2
ERM19264_UC1609 :: ERM19264_UC1609(int16_t lcdwidth, int16_t lcdheight , int8_t rst, int8_t cd)
{
	_LCD_HEIGHT = lcdheight;
	_LCD_WIDTH = lcdwidth;

	_LCD_RST= rst;
	_LCD_CD = cd;

	_LCD_mode = 2;

}

// Software SPI , mode 3
ERM19264_UC1609 :: ERM19264_UC1609(int16_t lcdwidth, int16_t lcdheight , int8_t rst, int8_t cd, int8_t cs, int8_t sclk, int8_t din)
{
	_LCD_HEIGHT = lcdheight;
	_LCD_WIDTH = lcdwidth;

	_LCD_RST= rst;
	_LCD_CD = cd;
	_LCD_CS = cs;
	_LCD_DIN = din;
	_LCD_SCLK = sclk;

	_LCD_mode = 3;
}

#else //buffer modes
// Hardware SPI , mode 2
ERM19264_UC1609 :: ERM19264_UC1609(int16_t lcdwidth, int16_t lcdheight ,int8_t rst, int8_t cd) :ERM19264_graphics(lcdwidth, lcdheight)
{
	_LCD_HEIGHT = lcdheight;
	_LCD_WIDTH = lcdwidth;

	_LCD_CD = cd;
	_LCD_RST= rst;

	_LCD_mode = 2;
}

// software SPI , mode 3
ERM19264_UC1609 :: ERM19264_UC1609(int16_t lcdwidth, int16_t lcdheight , int8_t rst, int8_t cd, int8_t cs, int8_t sclk, int8_t din) :ERM19264_graphics(lcdwidth, lcdheight)
{
	_LCD_HEIGHT = lcdheight;
	_LCD_WIDTH = lcdwidth;

	_LCD_CD = cd;
	_LCD_RST= rst;
	_LCD_CS = cs;
	_LCD_DIN = din;
	_LCD_SCLK = sclk;

	_LCD_mode = 3;
}

#endif

// Desc: begin Method initialise LCD
// Sets pinmodes and SPI setup
// Param1: VBiasPOT default = 0x49 , range 0x00 to 0xFE
void ERM19264_UC1609::LCDbegin (uint8_t VbiasPOT)
{
	bcm2835_gpio_fsel(_LCD_RST, BCM2835_GPIO_FSEL_OUTP);
	bcm2835_gpio_fsel(_LCD_CD, BCM2835_GPIO_FSEL_OUTP);

	_VbiasPOT  = VbiasPOT;

	if(GetCommMode() == 2)
	{
		LCDSPIon();
	}
	else
	{
		bcm2835_gpio_fsel( _LCD_CS, BCM2835_GPIO_FSEL_OUTP);
		bcm2835_gpio_fsel(_LCD_SCLK, BCM2835_GPIO_FSEL_OUTP);
		bcm2835_gpio_fsel(_LCD_DIN, BCM2835_GPIO_FSEL_OUTP);
	}

	LCDinit();
}

// Desc: Start SPI operations. Forces RPi SPI0 pins P1-19 (MOSI), P1-21 (MISO),
// P1-23 (CLK), P1-24 (CE0) and P1-26 (CE1)
// to alternate function ALT0, which enables those pins for SPI interface.
void ERM19264_UC1609::LCDSPIon(void)
{
	bcm2835_spi_begin();
	bcm2835_spi_setBitOrder(BCM2835_SPI_BIT_ORDER_MSBFIRST);
	bcm2835_spi_setDataMode(BCM2835_SPI_MODE0);
	// CLOCK_DIVIDER_64 = 3.90625MHz on Rpi2, 6.250MHz on RPI3
	bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_64);
	bcm2835_spi_chipSelect(BCM2835_SPI_CS0);
	bcm2835_spi_setChipSelectPolarity(BCM2835_SPI_CS0, LOW);
}

// Desc: stop  Spi
// End SPI operations. SPI0 pins P1-19 (MOSI), P1-21 (MISO), P1-23 (CLK),
// P1-24 (CE0) and P1-26 (CE1)
// are returned to their default INPUT behaviour.
void ERM19264_UC1609::LCDSPIoff(void)
{
	 bcm2835_spi_end();
}

// Call when powering down
void ERM19264_UC1609::LCDPowerDown(void)
{
	LCDReset();
	LCDEnable(0);
	UC1609_CD_SetLow ;
	UC1609_RST_SetLow ;
	if(GetCommMode()== 3)
	{
		UC1609_SDA_SetLow;
		UC1609_SCLK_SetLow ;
		UC1609_CS_SetLow ;
	}
	_sleep= true;
}


// Desc: Called from LCDbegin carries out Power on sequence and register init
// Can be used to reset LCD to default values.
void ERM19264_UC1609::LCDinit()
 {
	if(GetCommMode()== 3){UC1609_CS_SetLow;}
	UC1609_CD_SetHigh;

	bcm2835_delay(UC1609_INIT_DELAY2);

	LCDReset();

	send_command(UC1609_TEMP_COMP_REG, UC1609_TEMP_COMP_SET);
	send_command(UC1609_ADDRESS_CONTROL, UC1609_ADDRESS_SET);
	send_command(UC1609_FRAMERATE_REG, UC1609_FRAMERATE_SET);
	send_command(UC1609_BIAS_RATIO, UC1609_BIAS_RATIO_SET);
	send_command(UC1609_POWER_CONTROL,  UC1609_PC_SET);
	bcm2835_delay(UC1609_INIT_DELAY);

	send_command(UC1609_GN_PM, 0);
	send_command(UC1609_GN_PM, _VbiasPOT); //  changed by user

	send_command(UC1609_DISPLAY_ON, 0x01); // turn on display
	send_command(UC1609_LCD_CONTROL, UC1609_ROTATION_NORMAL); // rotate to normal
	_sleep= false;

	if(GetCommMode()== 3){UC1609_CS_SetHigh;}
}


// Desc: Sends a command to the display
// Param1: the command
// Param2: the values to change
void ERM19264_UC1609::send_command (uint8_t command, uint8_t value)
{
	UC1609_CD_SetLow;
	send_data(command | value);
	UC1609_CD_SetHigh;
}

// Desc: Resets LCD in a four wire setup called at start
// and  should also be called in a controlled power down setting
void ERM19264_UC1609::LCDReset ()
{
	UC1609_RST_SetLow;
	bcm2835_delay(UC1609_RESET_DELAY);
	UC1609_RST_SetHigh;
	bcm2835_delay(UC1609_RESET_DELAY2);
}

// Desc: turns on display
// Param1: bits 1  on , 0 off
void ERM19264_UC1609::LCDEnable (uint8_t bits)
{
	if(GetCommMode()== 3){UC1609_CS_SetLow ;}

	bits ? _sleep = false: _sleep = true;
	send_command(UC1609_DISPLAY_ON, bits);

	if(GetCommMode()== 3){UC1609_CS_SetHigh;}
}

// Desc: LCDIsSleeping
// Returns: bool  value of _sleep if true LCD is off and in sleep mode
bool ERM19264_UC1609::LCDIssleeping() { return  _sleep ;}

// Desc: Scroll the displayed image up by SL rows.
//The valid SL value is between 0 (for no
//scrolling) and (64).
//Setting SL outside of this range causes undefined effect on the displayed
//image.
// Param1: bits 0-64 line number y-axis
void ERM19264_UC1609::LCDscroll (uint8_t bits)
{
	if(GetCommMode()== 3){UC1609_CS_SetLow ;}

	send_command(UC1609_SCROLL, bits);

	if(GetCommMode()== 3){UC1609_CS_SetHigh;}
}

// Desc: Rotates the display
// Set LC[2:1] for COM (row) mirror (MY), SEG (column) mirror (MX).
// Param1: 4 possible values 000 010 100 110 (defined)
// If Mx is changed the buffer must BE updated
void ERM19264_UC1609::LCDrotate(uint8_t rotatevalue)
{
	if(GetCommMode()== 3){UC1609_CS_SetLow ;}

	switch (rotatevalue)
	{
		case 0: rotatevalue = 0; break;
		case 0x02: rotatevalue = UC1609_ROTATION_FLIP_ONE; break;
		case 0x04: rotatevalue = UC1609_ROTATION_NORMAL; break;
		case 0x06: rotatevalue = UC1609_ROTATION_FLIP_TWO; break;
		default: rotatevalue = UC1609_ROTATION_NORMAL; break;
	}
	send_command(UC1609_LCD_CONTROL, rotatevalue);

	if(GetCommMode()== 3){UC1609_CS_SetHigh;}
}

// Desc: invert the display
// Param1: bits, 1 invert , 0 normal
void ERM19264_UC1609::LCDinvert (uint8_t bits)
{
	if(GetCommMode()== 3){UC1609_CS_SetLow ;}

	send_command(UC1609_INVERSE_DISPLAY, bits);

	if(GetCommMode()== 3){UC1609_CS_SetHigh;}
}

// Desc: turns on all Pixels
// Param1: bits Set DC[1] to force all SEG drivers to output ON signals
// 1 all on ,  0 all off
void ERM19264_UC1609::LCDallpixelsOn(uint8_t bits)
{
 if(GetCommMode()== 3){UC1609_CS_SetLow ;}

	send_command(UC1609_ALL_PIXEL_ON, bits);

 if(GetCommMode()== 3){UC1609_CS_SetHigh;}
}

// Desc: Fill the chosen page(1-8)  with a datapattern
// Param1: Page_num 0-7
// Param2: datapattern can be set to 0 to FF (not buffer)
// Param3: optional delay in milliseconds can be set to zero normally.
void ERM19264_UC1609::LCDFillPage(uint8_t page_num, uint8_t dataPattern,uint8_t mydelay)
{
	if(GetCommMode()== 3){UC1609_CS_SetLow ;}
	send_command(UC1609_SET_COLADD_LSB, 0);
	send_command(UC1609_SET_COLADD_MSB, 0);
	send_command(UC1609_SET_PAGEADD, page_num);
	if(GetCommMode()== 3){UC1609_CS_SetHigh;}
	bcm2835_delayMicroseconds(5);
	if(GetCommMode()== 3){UC1609_CS_SetLow ;}
	uint8_t numofbytes = _LCD_WIDTH;
	for (uint8_t i = 0; i < numofbytes; i++)
	{
		send_data(dataPattern);
		bcm2835_delay(mydelay);
	}
	if(GetCommMode()== 3){UC1609_CS_SetHigh;}
}

// Desc: Fill the chosen page(1-8)  with a datapattern
// Param1: datapattern can be set to 0 to FF (not buffer)
// Param2: optional delay in milliseconds can be set to zero normally.
void ERM19264_UC1609::LCDFillScreen(uint8_t dataPattern, uint8_t delay)
{
	for (uint8_t row = 0; row < _LCD_PAGE_NUM; row++)
	{
		LCDFillPage(row,dataPattern,delay);
	}
}

//Desc: Draw a bitmap in PROGMEM to the screen
//Param1: x offset 0-192
//Param2: y offset 0-64
//Param3: width 0-192
//Param4 height 0-64
//Param5 the bitmap
void ERM19264_UC1609::LCDBitmap(int16_t x, int16_t y, uint8_t w, uint8_t h, const uint8_t* data)
{
	if(GetCommMode()== 3){UC1609_CS_SetLow ;}

	uint8_t tx, ty;
	uint16_t offset = 0;
	uint8_t column = (x < 0) ? 0 : x;
	uint8_t page = (y < 0) ? 0 : y >>3;

	for (ty = 0; ty < h; ty = ty + 8)
	{
		if (y + ty < 0 || y + ty >= _LCD_HEIGHT) {continue;}
		send_command(UC1609_SET_COLADD_LSB, (column & 0x0F));
		send_command(UC1609_SET_COLADD_MSB, (column & 0xF0) >> 4);
		send_command(UC1609_SET_PAGEADD, page++);

		for (tx = 0; tx < w; tx++)
		{
			if (x + tx < 0 || x + tx >= _LCD_WIDTH) {continue;}
			offset = (w * (ty >> 3)) + tx;
			send_data(data[offset]);
		}
	}

	if(GetCommMode()== 3){UC1609_CS_SetHigh;}
}

// Desc: Checks if software SPI is on
// Returns: true 2 if hardware SPi on ,  3 for software spi
int8_t  ERM19264_UC1609::GetCommMode(){return (_LCD_mode);}

// Desc: used in software SPI mode to shift out data
// Param1: the byte to go
// Other if using high freq MCU the delay define can be increased.
void ERM19264_UC1609::CustomshiftOut(uint8_t value)
{
	for (uint8_t  i = 0; i < 8; i++)
	{
	 	!!(value & (1 << (7 - i))) ? UC1609_SDA_SetHigh : UC1609_SDA_SetLow ;

		UC1609_SCLK_SetHigh;
		bcm2835_delayMicroseconds(UC1609_HIGHFREQ_DELAY);
		UC1609_SCLK_SetLow;
		bcm2835_delayMicroseconds(UC1609_HIGHFREQ_DELAY);
	}
}

//Desc: Send data byte with SPI to UC1609C
//Param1: the data byte
void ERM19264_UC1609::send_data(uint8_t byte)
{
	if(GetCommMode() == 2 )
			bcm2835_spi_transfer(byte);
	else if(GetCommMode() == 3)
			CustomshiftOut(byte);
}


// ******************************************************
// Functions below not & needed for no_buffer mode
#ifndef NO_BUFFER

//Desc: updates the buffer i.e. writes it to the screen
void ERM19264_UC1609::LCDupdate()
{

#ifdef MULTI_BUFFER
	LCDBuffer( this->ActiveBuffer->xoffset, this->ActiveBuffer->yoffset, this->ActiveBuffer->width, this->ActiveBuffer->height, (uint8_t*) this->ActiveBuffer->screenbitmap);
	return;
#endif

#ifdef SINGLE_BUFFER
	uint8_t x = 0; uint8_t y = 0; uint8_t w = this->bufferWidth; uint8_t h = this->bufferHeight;
	LCDBuffer( x,  y,  w,  h, (uint8_t*) this->buffer);
#endif
}

//Desc: clears the buffer i.e. does NOT write to the screen
void ERM19264_UC1609::LCDclearBuffer()
{
#ifdef MULTI_BUFFER
	 memset( this->ActiveBuffer->screenbitmap, 0x00, (this->ActiveBuffer->width * (this->ActiveBuffer->height/ 8))  );
	 return;
#endif

#ifdef SINGLE_BUFFER
	memset( this->buffer, 0x00, (this->bufferWidth * (this->bufferHeight /8))  );
#endif
}

//Desc: Draw a bitmap to the screen
//Param1: x offset 0-192
//Param2: y offset 0-64
//Param3: width 0-192
//Param4 height 0-64
//Param5 the bitmap
void ERM19264_UC1609::LCDBuffer(int16_t x, int16_t y, uint8_t w, uint8_t h, uint8_t* data)
{
 if(GetCommMode()== 3){UC1609_CS_SetLow ;}

	uint8_t tx, ty;
	uint16_t offset = 0;
	uint8_t column = (x < 0) ? 0 : x;
	uint8_t page = (y < 0) ? 0 : y/8;

	for (ty = 0; ty < h; ty = ty + 8)
	{
		if (y + ty < 0 || y + ty >= _LCD_HEIGHT) {continue;}

		send_command(UC1609_SET_COLADD_LSB, (column & 0x0F));
		send_command(UC1609_SET_COLADD_MSB, (column & 0XF0) >> 4);
		send_command(UC1609_SET_PAGEADD, page++);

		for (tx = 0; tx < w; tx++)
		{
			if (x + tx < 0 || x + tx >= _LCD_WIDTH) {continue;}
			offset = (w * (ty /8)) + tx;
			send_data(data[offset++]);
		}
	}

 if(GetCommMode()== 3){UC1609_CS_SetHigh;}
}

// Desc: Draws a Pixel to the screen overides the custom graphics library
// Passed x and y co-ords and colour of pixel.
void ERM19264_UC1609::drawPixel(int16_t x, int16_t y, uint16_t colour)
{

#ifdef MULTI_BUFFER
	if ((x < 0) || (x >= this->ActiveBuffer->width) || (y < 0) || (y >= this->ActiveBuffer->height)) {
		return;
	}
	uint16_t offset = (this->ActiveBuffer->width * (y/8)) + x;
	switch (colour)
	{
		case FOREGROUND: this->ActiveBuffer->screenbitmap[offset] |= (1 << (y & 7)); break;
		case BACKGROUND: this->ActiveBuffer->screenbitmap[offset] &= ~(1 << (y & 7)); break;
		case INVERSE: this->ActiveBuffer->screenbitmap[offset] ^= (1 << (y & 7)); break;
	}
	return;
#endif

#ifdef SINGLE_BUFFER
	if ((x < 0) || (x >= this->bufferWidth) || (y < 0) || (y >= this->bufferHeight)) {
		return;
	}
	uint16_t tc = (bufferWidth * (y /8)) + x;
	switch (colour)
	{
		case FOREGROUND:  this->buffer[tc] |= (1 << (y & 7)); break;
		case BACKGROUND:  this->buffer[tc] &= ~(1 << (y & 7)); break;
		case INVERSE: this->buffer[tc] ^= (1 << (y & 7)); break;
	}
#endif
}

// ***************************************************
#else
//  these functions are not needed by buffers modes
// and only used in "no buffer" mode
// ***********************************************

// Desc: goes to XY position
// Param1 : coloumn 0-192
// Param2  : page 0-7
void ERM19264_UC1609::LCDNoBufferGotoXY(uint8_t column , uint8_t page)
{
	 if(GetCommMode()== 3){UC1609_CS_SetLow ;}
	send_command(UC1609_SET_COLADD_LSB, (column & 0x0F));
	send_command(UC1609_SET_COLADD_MSB, (column & 0xF0) >> 4);
	send_command(UC1609_SET_PAGEADD, page++);
	 if(GetCommMode()== 3){UC1609_CS_SetHigh;}
}

// Desc: draws passed character.
// Param1: character 'A' or number in  the ASCII table 1-127(default)
void ERM19264_UC1609::LCDNoBufferChar(unsigned char character)
{
	if(GetCommMode()== 3){UC1609_CS_SetLow ;}
	UC1609_FONTPADDING;
	for (uint8_t column = 0 ; column < UC1609_FONTWIDTH ; column++)
	{
		send_data((uint8_t)(UC_custom_font[(character*UC1609_FONTWIDTH) + column]));
	}
	UC1609_FONTPADDING;
	if(GetCommMode()== 3){UC1609_CS_SetHigh ;}
}

// Desc: draws passed  character array
// Param1: pointer to start of character array
void ERM19264_UC1609::LCDNoBufferString(const  unsigned char *characters)
{

	if(GetCommMode()== 3){UC1609_CS_SetLow ;}
	
	char index;
	while (*characters)
	{
		index = *characters++;
		UC1609_FONTPADDING;
		for (uint8_t column = 0; column < UC1609_FONTWIDTH; column++)
		{
			send_data((uint8_t)(UC_custom_font[(index*UC1609_FONTWIDTH) + column]));
		}
		UC1609_FONTPADDING;
	};

	if(GetCommMode()== 3){UC1609_CS_SetHigh ;}
}
#endif
//****************  EOF ******************************8
