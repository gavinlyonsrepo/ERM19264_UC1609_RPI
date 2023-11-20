
// Example file name : main.cpp
// Description:
// Test file for ERM19264_UC1609 library, showing use of fonts
// URL: https://github.com/gavinlyonsrepo/ERM19264_UC1609_RPI
// *****************************


#include <bcm2835.h>
#include <time.h>
#include <stdio.h>

#include "ERM19264_UC1609.hpp"

// Include the fonts for test
#include "FreeSans12pt7b.h"
#include "FreeSans18pt7b.h"
#include "FreeSerif12pt7b.h"
#include "FreeSerif18pt7b.h"

const uint8_t RST = 25; // GPIO pin number pick any you want
const uint8_t CD = 24; // GPIO pin number pick any you want 
const uint8_t myLCDwidth  = 192;
const uint8_t myLCDheight = 64;

const uint32_t SPICLK_FREQ = 64; // Spi clock divider, see bcm2835SPIClockDivider enum bcm2835
const uint8_t SPI_CE_PIN = 0; // which HW SPI chip enable pin to use,  0 or 1
const uint8_t LCDcontrast = 0x49; //Constrast 00 to FF , 0x80 is default.
const uint8_t RAMaddressCtrl = 0x02; // RAM address control: Range 0-7, optional, default 2

//instantiate object
ERM19264_UC1609 myLCD(myLCDwidth ,myLCDheight, RST, CD );

#define MY_TEST_DELAY2 bcm2835_delay(2000)
#define MY_TEST_DELAY1 bcm2835_delay(1000)

// =============== Function prototype ================
void setup(void);
void myTest(void);
void EndTest(void);
void testReset(void);

// ======================= Main ===================
int main(int argc, char **argv)
{
	if(!bcm2835_init()) {return -1;}

	setup();
	myTest();
	EndTest();

	return 0;
}
// ======================= End of main  ===================


// ===================== Function Space =====================
void setup() {
	bcm2835_delay(50);
	printf("LCD Begin\r\n");
	myLCD.LCDbegin(RAMaddressCtrl, LCDcontrast, SPICLK_FREQ , SPI_CE_PIN); // initialize the LCD
	myLCD.LCDFillScreen(0x33); // splash screen bars
	bcm2835_delay(1500);
}

void EndTest()
{
	myLCD.LCDPowerDown();
	bcm2835_close(); // Close the library
	printf("LCD End\r\n");
}

void myTest() {

	// define a buffer to cover whole screen
	uint8_t screenBuffer[myLCDwidth * (myLCDheight/8)]; 
	myLCD.LCDbufferScreen = (uint8_t*) &screenBuffer;
	myLCD.LCDclearBuffer();   // Clear buffer

	// Test 1 Font FreeSans12pt7b
	const char *txt = "FreeSans               12pt7b";
	myLCD.setFontGlyph(&FreeSans12pt7b);
	myLCD.drawTextGlyph(0, 18, txt, FOREGROUND, 1);
	myLCD.drawCharGlyph(10, 62, '!', FOREGROUND, 1);
	testReset();

	// Test 2 Font FreeSans18pt7b
	txt = "Free Sans   18pt7b";
	myLCD.setFontGlyph(&FreeSans18pt7b);
	myLCD.drawTextGlyph(0, 26, txt, FOREGROUND, 1);
	testReset();

	// Test 3 Font FreeSerif12pt7b
	txt = "FreeSerif                12pt7b";
	myLCD.setFontGlyph(&FreeSerif12pt7b);
	myLCD.drawTextGlyph(0, 18, txt, FOREGROUND, 1);
	myLCD.drawCharGlyph(10, 62, '!', FOREGROUND, 1);
	testReset();

	// Test 4 Font FreeSErif18pt7b
	txt = "Free Serif     18pt7b";
	myLCD.setFontGlyph(&FreeSerif18pt7b);
	myLCD.drawTextGlyph(0, 26, txt, FOREGROUND, 1);
	testReset();

	// Test 5 compare 1 & 3
	txt = "FreeSans12pt7b";
	myLCD.setFontGlyph(&FreeSans12pt7b);
	myLCD.drawTextGlyph(0, 18, txt, FOREGROUND, 1);
	txt = "FreeSerif12pt7b";
	myLCD.setFontGlyph(&FreeSerif12pt7b);
	myLCD.drawTextGlyph(0, 46, txt, FOREGROUND, 1);
	testReset();

	// Test 6 display a number
	int count=0;
	char myStr[10];
	myLCD.setFontGlyph(&FreeSans12pt7b);

	while(count < 1000)
	{
		sprintf(myStr, "%d", count);
		myLCD.drawTextGlyph(0, 18, myStr, FOREGROUND, 1);
		count+= 23;
		myLCD.LCDupdate();
		MY_TEST_DELAY1;
		myLCD.LCDclearBuffer();
	}
}

void testReset()
{
	myLCD.LCDupdate(); //write to buffer
	MY_TEST_DELAY2;
	myLCD.LCDclearBuffer(); // Clear buffer
}
// ============== EOF =========
