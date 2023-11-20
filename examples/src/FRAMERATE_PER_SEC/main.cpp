
// Example file name : main.cpp
// Description:
// Test file for ERM19264_UC1609 library, showing FPS  
// URL: https://github.com/gavinlyonsrepo/ERM19264_UC1609_RPI
// *****************************
// Measured:: frame rate 214 fps at 64 clock divider on Rpi 3 v1.5 2022
// Measured:: frame rate 350 fps at 16 clock divider on Rpi 3 v1.5 2022

#include <bcm2835.h>
#include <time.h>
#include <stdio.h>
#include "ERM19264_UC1609.hpp"

// LCD Setup
const uint8_t RST = 25; // GPIO pin number pick any you want
const uint8_t CD = 24; // GPIO pin number pick any you want 
const uint8_t myLCDwidth  = 192;
const uint8_t myLCDheight = 64;

const uint32_t SPICLK_FREQ = 64; // Spi clock divider, see bcm2835SPIClockDivider enum bcm2835
const uint8_t SPI_CE_PIN = 0; // which HW SPI chip enable pin to use,  0 or 1
const uint8_t LCDcontrast = 0x49; //Constrast 00 to FF , 0x80 is default.
const uint8_t RAMaddressCtrl = 0x02; // RAM address control: Range 0-7, optional, default 2

 // instantiate  an object
ERM19264_UC1609 myLCD(myLCDwidth ,myLCDheight , RST, CD) ;

// vars for the test
uint16_t count  = 0;
bool colour = 1;
uint64_t  previousCounter =0;

// =============== Function prototype ================
void setup(void);
void myTest(void);
void EndTest(void);
void displayData(long , int );
static uint64_t counter( void );

// ======================= Main ===================
int main(int argc, char **argv)
{
	if(!bcm2835_init()){return -1;}

	setup();
	myTest();
	EndTest();

	return 0;
}
// =============== End of main  ===============


// ============== Function Space ============

void EndTest(void)
{
	myLCD.LCDPowerDown();
	bcm2835_close(); //Close lib, deallocating allocated mem & close /dev/mem
	printf("LCD End\r\n");
}

void setup()
{
	bcm2835_delay(50);
	printf("LCD Begin\r\n");
	myLCD.LCDbegin(RAMaddressCtrl, LCDcontrast, SPICLK_FREQ , SPI_CE_PIN); // initialize the LCD
	myLCD.LCDFillScreen(0x01);
	bcm2835_delay(2000);
}

void myTest() {


	myLCD.setTextColor(FOREGROUND);
	myLCD.setTextSize(1);
	// define a buffer to cover whole screen
	uint8_t screenBuffer[myLCDwidth * (myLCDheight/8)]; 
	myLCD.LCDbufferScreen = (uint8_t*) &screenBuffer;
	myLCD.LCDclearBuffer();  // Clear buffer

	while (count < 10000)
	{
		static long framerate = 0;
		displayData(framerate, count);
		framerate++;
		count++;
		bcm2835_delay(1);
	}

}


// Function to display data
void displayData(long currentFramerate, int count)
{
	myLCD.LCDclearBuffer();
	myLCD.setCursor(0, 0);
	myLCD.print("LHS Screen");

	myLCD.setCursor(0, 10);
	myLCD.print("768 bytes");

	myLCD.setCursor(0, 20);
	myLCD.print("G Lyons");

	myLCD.setCursor(0, 30);
	myLCD.print(count);
	// Values to count frame rate per second
	static long lastFramerate = 0;
	static uint16_t fps;
	uint64_t currentCounter = counter();
	if(currentCounter - previousCounter >= 1000000000)
	{
		//(Rolls over every 1 second)
		fps = currentFramerate - lastFramerate;
		lastFramerate = currentFramerate ;
		previousCounter = currentCounter;
		colour = !colour;
	}

	myLCD.setCursor(0, 40);
	myLCD.print(fps);
	myLCD.print(" fps");
	myLCD.setCursor(0, 50);
	myLCD.print("V 1.7");
	myLCD.drawFastVLine(92, 0, 63, FOREGROUND);
	
	myLCD.setCursor(97, 0);
	myLCD.print("RHS Screen");

	myLCD.fillRect(97, 10, 20, 20, colour);
	myLCD.fillCircle(137, 20, 10, FOREGROUND);
	myLCD.fillTriangle(157, 30, 167, 10, 177, 30, !colour);
	myLCD.drawRoundRect(107, 40, 60, 20, 10, FOREGROUND);
	
	myLCD.LCDupdate();
}


//This returns nano-seconds as a 64-bit unsigned number, monotonically increasing,
//probably since system boot.
//The actual resolution looks like microseconds. 
// Returns nanoseconds
static uint64_t counter( void )
{
  struct timespec now;
  clock_gettime( CLOCK_MONOTONIC, &now );
  return  ((uint64_t)now.tv_sec * 1000000000U) + (uint64_t)now.tv_nsec;
}

// ******************** EOF *********************8
