
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
// NOTES :
// (1) In the <ERM19264_UC1609.h> USER BUFFER OPTION SECTION at top of file
// , option MULTI_BUFFER must be selected
// and only this option. It is on by default.
// ******************************

#include <bcm2835.h>
#include "ERM19264_UC1609.h"
#include <time.h>
#include <stdio.h>

#define LCDcontrast 0x50 //Contrast 00 to FF , 0x49 is default. user adjust
#define mylcdwidth  192
#define mylcdheight 64

// GPIO 
#define RST 25 // GPIO pin number pick any you want
#define CD 24 // GPIO pin number pick any you want 

ERM19264_UC1609 mylcd(mylcdwidth ,mylcdheight , RST, CD ) ;  

// =============== Function prototype ================
void setup(void);
void myLoop(void);

// ======================= Main ===================
int main(int argc, char **argv) 
{
	if(!bcm2835_init())
	{
		return -1;
	}
	bcm2835_delay(500);
	printf("LCD Begin\r\n");
	setup();
	myLoop();
	mylcd.LCDSPIoff();
	mylcd.LCDPowerDown();
	bcm2835_close(); // Close lib, deallocate allocated mem & close /dev/mem
	printf("LCD End\r\n");
	return 0;
}
// ======================= End of main  ===================


void setup()
{
	mylcd.LCDbegin(LCDcontrast);  // initialize the LCD
	mylcd.LCDFillScreen(0x11, 0); // Clears screen
	bcm2835_delay(1500);
}

void myLoop()
{
	// Define a full screen buffer and struct
	uint8_t  screenBuffer[mylcdwidth * (mylcdheight/8)];
	
	MultiBuffer whole_screen;
	whole_screen.screenbitmap = (uint8_t*) &screenBuffer;
	whole_screen.width = mylcdwidth;
	whole_screen.height = mylcdheight;
	whole_screen.xoffset = 0;
	whole_screen.yoffset = 0;
	
	mylcd.ActiveBuffer =  &whole_screen; // set buffer object
	mylcd.LCDclearBuffer(); // clear the buffer

	// Set text parameters
	mylcd.setTextColor(FOREGROUND);
	mylcd.setTextSize(2);

	// Test 1 LCD all pixels on
	mylcd.setCursor(20, 30);
	mylcd.print("All Pixels on");
	mylcd.LCDupdate();
	bcm2835_delay(4000);
	mylcd.LCDclearBuffer();
	mylcd.LCDupdate();
	mylcd.LCDallpixelsOn(1);
	bcm2835_delay(2000);
	mylcd.LCDallpixelsOn(0);
	bcm2835_delay(2000);

	// Test 2 inverse
	mylcd.setCursor(20, 30);
	mylcd.print("inverse test  ");
	mylcd.LCDupdate();
	mylcd.LCDinvert(0); // Normal
	bcm2835_delay(2000);
	mylcd.LCDinvert(1); // Inverted
	bcm2835_delay(4000);
	mylcd.LCDinvert(0);


	// Test3 LCD rotate
	mylcd.LCDclearBuffer();
	mylcd.setCursor(20, 30);
	mylcd.print("rotate test");
	mylcd.LCDupdate();
	bcm2835_delay(2000);
	mylcd.LCDrotate(UC1609_ROTATION_FLIP_ONE);
	mylcd.LCDupdate();
	bcm2835_delay(5000);
	mylcd.LCDrotate(UC1609_ROTATION_FLIP_TWO);
	mylcd.LCDupdate();
	bcm2835_delay(5000);
	mylcd.LCDrotate(UC1609_ROTATION_FLIP_THREE);
	mylcd.LCDupdate();
	bcm2835_delay(5000);
	mylcd.LCDrotate(UC1609_ROTATION_NORMAL);
	mylcd.LCDupdate();
	bcm2835_delay(5000);


	// Test4 LCD scroll
	mylcd.LCDclearBuffer();
	//mylcd.LCDupdate();
	mylcd.setCursor(0, 40);
	mylcd.print("scroll test");
	for (uint8_t i = 0 ; i < 62 ; i ++) 
	{
		mylcd.LCDscroll(i);
		mylcd.LCDupdate();
		bcm2835_delay(100);
	}
	mylcd.LCDscroll(0);

	//Test5 LCD enable and disable
	mylcd.LCDclearBuffer();
	mylcd.setCursor(0, 30);
	mylcd.print("LCD Disable test");
	mylcd.LCDupdate();
	bcm2835_delay(5000);
	mylcd.LCDEnable(0);
	bcm2835_delay(5000);
	mylcd.LCDEnable(1);
	mylcd.LCDclearBuffer();

	// Test 6 Powerdown
	mylcd.setCursor(5, 10);
	mylcd.print("End Tests");
	mylcd.setCursor(5, 35);
	mylcd.print("Power down in 5");
	mylcd.LCDupdate();
	bcm2835_delay(5000);
}

// *********** EOF ************
