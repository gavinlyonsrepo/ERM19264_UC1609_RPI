
// Example file name : main.cpp
// Description:
// Test file for ERM19264_UC1609 library, showing use of hello world
// URL: https://github.com/gavinlyonsrepo/ERM19264_UC1609_RPI
// *****************************
// NOTES :
// (1) In the <ERM19264_UC1609.h> USER BUFFER OPTION SECTION, at top of file
// option MULTI_BUFFER must be selected
// and only this option. It is on by default.
// ******************************

#include <bcm2835.h>
#include "ERM19264_UC1609.h"
#include <time.h>
#include <stdio.h>

#define LCDcontrast 0x49 //Constrast 00 to FF , 0x49 is default. user adjust
#define myLCDwidth  192
#define myLCDheight 64
#define mode 2

// GPIO 
#define RST 25 // GPIO pin number pick any you want
#define CD 24 // GPIO pin number pick any you want 

ERM19264_UC1609 myLCD(myLCDwidth ,myLCDheight, RST, CD ) ; // instantiate  an object 

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
	setup();
	myLoop();
	myLCD.LCDSPIoff();
	myLCD.LCDPowerDown();
	bcm2835_close(); // Close the library, deallocating any allocated memory and closing /dev/mem
	printf("LCD End\r\n");
	return 0;
}
// ======================= End of main  ===================

// ===================== Function Space =====================
void setup() {
	printf("LCD Begin\r\n");
	myLCD.LCDbegin(LCDcontrast); // initialize the LCD
	myLCD.LCDFillScreen(0x2F, 0); //splash screen bars
	bcm2835_delay(3000);
}

void myLoop() {

	// define a buffer to cover whole screen 
	uint8_t  screenBuffer[myLCDwidth * (myLCDheight/8)]; // 1024 bytes = 128 * 64/8

	// Declare a buffer struct
	MultiBuffer mybuffer;

	// Intialise struct 
	mybuffer.screenbitmap = (uint8_t*) &screenBuffer; // point it to the buffer
	mybuffer.width = myLCDwidth ;
	mybuffer.height = myLCDheight;
	mybuffer.xoffset = 0;
	mybuffer.yoffset = 0;

	// Assign address of struct to be the active buffer pointer 
	myLCD.ActiveBuffer = &mybuffer;

	myLCD.LCDclearBuffer();   // Clear active buffer 

	myLCD.setTextColor(FOREGROUND);
	myLCD.setCursor(10, 10);
	myLCD.print("Hello world");
	myLCD.LCDupdate();  //write to active buffer
	delay(5000);
}

// ============== EOF =========
