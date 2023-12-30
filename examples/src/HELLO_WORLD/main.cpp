
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
bool setup(void);
bool myTest(void);
void EndTest(void);

// ======================= Main ===================
int main(int argc, char **argv) 
{
	if(!setup()) {return -1;}
	if(!myTest()) {return -1;}
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

bool myTest() {

	// define a buffer to cover whole screen 
	uint8_t screenBuffer[myLCDwidth * (myLCDheight/8)]; 
	myLCD.LCDbufferScreen = (uint8_t*) &screenBuffer;
	if(myLCD.LCDbufferScreen== nullptr) // check if pointer is still = null
	{
		printf("Error 1203 :: Problem assigning buffer pointer\r\n");
		return false;
	}
	myLCD.LCDclearBuffer();   // Clear buffer 
	myLCD.setTextColor(FOREGROUND);
	myLCD.setCursor(10, 10);
	myLCD.print("Hello world");
	myLCD.LCDupdate();  //write to active buffer
	delay(6000);
	return true;
}

// ============== EOF =========
