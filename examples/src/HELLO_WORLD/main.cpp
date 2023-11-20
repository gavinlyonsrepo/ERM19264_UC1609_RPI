
// Example file name : main.cpp
// Description:
// Test file for ERM19264_UC1609 library, showing use of hello world
// URL: https://github.com/gavinlyonsrepo/ERM19264_UC1609_RPI
// *****************************


#include <bcm2835.h>
#include <time.h>
#include <stdio.h>
#include "ERM19264_UC1609.hpp"

const uint8_t RST = 25; // GPIO pin number pick any you want
const uint8_t CD = 24; // GPIO pin number pick any you want 
const uint8_t myLCDwidth  = 192;
const uint8_t myLCDheight = 64;

const uint32_t SPICLK_FREQ = 64; // Spi clock divider, see bcm2835SPIClockDivider enum bcm2835
const uint8_t SPI_CE_PIN = 0; // which HW SPI chip enable pin to use,  0 or 1
const uint8_t LCDcontrast = 0x49; // Contrast 00 to FF , 0x49 is default.
const uint8_t RAMaddressCtrl = 0x02; // RAM address control: Range 0-7, optional, default 2

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
	myLCD.LCDbegin(RAMaddressCtrl, LCDcontrast, SPICLK_FREQ , SPI_CE_PIN); // initialize the LCD
	myLCD.LCDFillScreen(0x33); // display splash screen bars
	bcm2835_delay(2000);
}

void EndTest()
{
	myLCD.LCDPowerDown();
	bcm2835_close(); // Close library, deallocating allocated memory & closing /dev/mem
	printf("LCD End\r\n");
}

void myTest() {

	// define a buffer to cover whole screen 
	uint8_t screenBuffer[myLCDwidth * (myLCDheight/8)]; 
	myLCD.LCDbufferScreen = (uint8_t*) &screenBuffer;
	myLCD.LCDclearBuffer();   // Clear buffer 

	myLCD.setTextColor(FOREGROUND);
	myLCD.setCursor(10, 10);
	myLCD.print("Hello world");
	myLCD.LCDupdate();  //write to active buffer
	delay(6000);
}

// ============== EOF =========
