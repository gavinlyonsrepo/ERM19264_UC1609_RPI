
// Example file name : main.cpp
// Description:
// Test file for ERM19264_UC1609 library, showing use of hello world
// URL: https://github.com/gavinlyonsrepo/ERM19264_UC1609_RPI
// *****************************


#include <bcm2835.h>
#include <time.h>
#include <stdio.h>
#include "ERM19264_UC1609.h"

#define LCDcontrast 0x49 //Constrast 00 to FF , 0x49 is default.
#define myLCDwidth  192
#define myLCDheight 64

// GPIO 
#define RST 25 // GPIO pin number pick any you want
#define CD 24 // GPIO pin number pick any you want 

ERM19264_UC1609 myLCD(myLCDwidth ,myLCDheight, RST, CD ); //instantiate object 

// =============== Function prototype ================
void setup(void);
void myTest(void);
void EndTest(void);

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
	myLCD.LCDbegin(LCDcontrast); // initialize the LCD
	myLCD.LCDFillScreen(0x33); // splash screen bars
	bcm2835_delay(2500);
}

void EndTest()
{
	myLCD.LCDPowerDown();
	bcm2835_close(); // Close the library, deallocating any allocated memory and closing /dev/mem
	printf("LCD End\r\n");
}

void myTest() {

	// define a buffer to cover whole screen 
	uint8_t  screenBuffer[myLCDwidth * (myLCDheight/8)]; 

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
	delay(6000);
}

// ============== EOF =========
