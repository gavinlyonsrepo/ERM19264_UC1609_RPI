
// Example file name : main.cpp
// Description:
// Test file for ERM19264_UC1609 library, showing use of various functions
//  (1) All Pixels on
//  (2) Invert screen
//  (3) Rotate screen
//  (4) Scroll Screen
//  (5) Enable and disable Screen 
//  (6) Power down (LCD OFF)
//
// URL: https://github.com/gavinlyonsrepo/ERM19264_UC1609_RPI
// *****************************


#include <bcm2835.h>
#include <time.h>
#include <stdio.h>
#include "ERM19264_UC1609.hpp"

// LCD setup
const uint8_t RST = 25; // GPIO pin number pick any you want
const uint8_t CD = 24; // GPIO pin number pick any you want 
const uint8_t myLCDwidth  = 192;
const uint8_t myLCDheight = 64;

const uint32_t SPICLK_FREQ = 64; // Spi clock divider, see bcm2835SPIClockDivider enum bcm2835
const uint8_t SPI_CE_PIN = 0; // which HW SPI chip enable pin to use,  0 or 1
const uint8_t LCDcontrast = 0x49; //Constrast 00 to FF , 0x80 is default.
const uint8_t RAMaddressCtrl = 0x02; // RAM address control: Range 0-7, optional, default 2

ERM19264_UC1609 myLCD(myLCDwidth ,myLCDheight , RST, CD ) ;  


// =============== Function prototype ================
bool setup(void);
void myTest(void);
void EndTest(void);

// ======================= Main ===================
int main(int argc, char **argv) 
{
	if(!setup()) {return -1;}
	myTest();
	EndTest();

	return 0;
}
// ======================= End of main  ===================

// ===================== Function Space =====================
bool setup() {
	printf("LCD Test Begin\r\n");
	// Check if Bcm28235 lib installed and print version.
	if(!bcm2835_init())
	{
		printf("Error 1201: init bcm2835 library , Is it installed ?\r\n");
		return false;
	}else
	{
		printf("bcm2835 library Version Number :: %u\r\n",bcm2835_version());
		bcm2835_delay(100);
	}

	if(!myLCD.LCDbegin(RAMaddressCtrl, LCDcontrast, SPICLK_FREQ , SPI_CE_PIN))  // initialize the LCD
	{
		printf("Error 1202: bcm2835_spi_begin :Cannot start spi, Running as root?\n");
		return false;
	}
	printf("ERM19264 Library version number :: %u \r\n", myLCD.LCDLibVerNumGet());
	myLCD.LCDFillScreen(0x33); // display splash screen bars, optional for effect
	bcm2835_delay(1500);
	return true;
}

void EndTest()
{
	myLCD.LCDPowerDown();
	bcm2835_close(); // Close library, deallocating allocated memory & closing /dev/mem
	printf("LCD End\r\n");
}

void myTest()
{
	// define a buffer to cover whole screen
	uint8_t screenBuffer[myLCDwidth * (myLCDheight/8)]; 
	myLCD.LCDbufferScreen = (uint8_t*) &screenBuffer;
	if(myLCD.LCDbufferScreen== nullptr) // check if pointer is still = null
	{
		printf("Error 1203 :: Problem assigning buffer pointer\r\n");
		exit(-1);
	}
	myLCD.LCDclearBuffer();   // Clear buffer

	// Set text parameters
	myLCD.setTextColor(FOREGROUND);
	myLCD.setTextSize(2);

	// Test 1 LCD all pixels on
	myLCD.setCursor(20, 30);
	myLCD.print("All Pixels on");
	myLCD.LCDupdate();
	bcm2835_delay(4000);
	myLCD.LCDclearBuffer();
	myLCD.LCDupdate();
	myLCD.LCDallpixelsOn(1);
	bcm2835_delay(2000);
	myLCD.LCDallpixelsOn(0);
	bcm2835_delay(2000);

	// Test 2 inverse
	myLCD.setCursor(20, 30);
	myLCD.print("inverse test  ");
	myLCD.LCDupdate();
	myLCD.LCDinvert(0); // Normal
	bcm2835_delay(2000);
	myLCD.LCDinvert(1); // Inverted
	bcm2835_delay(4000);
	myLCD.LCDinvert(0);

	// Test3 LCD rotate
	myLCD.LCDclearBuffer();
	myLCD.setCursor(20, 30);
	myLCD.print("rotate test");
	myLCD.LCDupdate();
	bcm2835_delay(2000);
	myLCD.LCDrotate(UC1609_ROTATION_FLIP_ONE);
	myLCD.LCDupdate();
	bcm2835_delay(5000);
	myLCD.LCDrotate(UC1609_ROTATION_FLIP_TWO);
	myLCD.LCDupdate();
	bcm2835_delay(5000);
	myLCD.LCDrotate(UC1609_ROTATION_FLIP_THREE);
	myLCD.LCDupdate();
	bcm2835_delay(5000);
	myLCD.LCDrotate(UC1609_ROTATION_NORMAL);
	myLCD.LCDupdate();
	bcm2835_delay(5000);

	// Test4 LCD scroll
	myLCD.LCDclearBuffer();
	//myLCD.LCDupdate();
	myLCD.setCursor(0, 40);
	myLCD.print("scroll test");
	for (uint8_t i = 0 ; i < 62 ; i ++) 
	{
		myLCD.LCDscroll(i);
		myLCD.LCDupdate();
		bcm2835_delay(100);
	}
	myLCD.LCDscroll(0);

	//Test5 LCD enable and disable
	myLCD.LCDclearBuffer();
	myLCD.setCursor(0, 30);
	myLCD.print("LCD Disable test");
	myLCD.LCDupdate();
	bcm2835_delay(5000);
	myLCD.LCDEnable(0);
	bcm2835_delay(5000);
	myLCD.LCDEnable(1);
	myLCD.LCDclearBuffer();

	// Test 6 Powerdown
	myLCD.setCursor(5, 10);
	myLCD.print("End Tests");
	myLCD.setCursor(5, 35);
	myLCD.print("Power down in 5");
	myLCD.LCDupdate();
	bcm2835_delay(5000);
}

// *********** EOF ************
