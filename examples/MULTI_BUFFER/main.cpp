
// Example file name : main.cpp
// Description:
// Test file for ERM19264_UC1609 library, showing use of mulitple buffers.  
// In this case: two, divided horizontally 
// The user can also divide vertically and create as many buffers as they want.
// URL: https://github.com/gavinlyonsrepo/ERM19264_UC1609_RPI
// *****************************
// NOTES :
// (1) In the <ERM19264_UC1609.h> USER BUFFER OPTION SECTION, at top of file
// option MULTI_BUFFER must be selected
// and only this option. It is on by default.
// (2) measured frame rate 214 fps at 64 clock divider on Rpi 3
// ******************************
//

#include <bcm2835.h>
#include "ERM19264_UC1609.h"
#include <time.h>
#include <stdio.h>

#define LCDcontrast 0x25
#define myLCDwidth  192
#define myLCDheight 64

// GPIO
#define RES 25 // GPIO pin number pick any you want
#define DC 24 // GPIO pin number pick any you want

ERM19264_UC1609 myLCD(myLCDwidth ,myLCDheight , RES, DC) ; // instantiate  an object

// vars for the test
uint16_t count  = 0;
bool colour = 1;
uint64_t  previousCounter =0;

// =============== Function prototype ================
void setup(void);
void myLoop(void);
void display_Left(MultiBuffer* , long , int );
void display_Right(MultiBuffer* );
static uint64_t counter( void );

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

	myLCD.LCDSPIoff();
	myLCD.LCDPowerDown();

	bcm2835_close(); //Close lib, deallocating allocated mem & close /dev/mem
	printf("LCD End\r\n");
	return 0;
}
// ======================= End of main  ===================


// ************* SETUP ***************
void setup()
{
	myLCD.LCDbegin(LCDcontrast); // initialize the LCD
	myLCD.LCDFillScreen(0x01, 0);
	bcm2835_delay(2400);
}

// *********** MAIN LOOP ******************
void myLoop() {


	myLCD.setTextColor(FOREGROUND);
	myLCD.setTextSize(1);
	uint8_t  screenBuffer[(myLCDwidth * (myLCDheight / 8)) / 2];

	MultiBuffer left_side;
	left_side.screenbitmap = (uint8_t*) &screenBuffer;
	left_side.width = (myLCDwidth / 2) ;
	left_side.height = myLCDheight;
	left_side.xoffset = 0;
	left_side.yoffset = 0;


	MultiBuffer right_side;
	right_side.screenbitmap = (uint8_t*) &screenBuffer;
	right_side.width = (myLCDwidth / 2);
	right_side.height = myLCDheight;
	right_side.xoffset = (myLCDwidth / 2);
	right_side.yoffset = 0;

	while (count < 10000)
	{
		static long framerate = 0;
		display_Left(&left_side, framerate, count);

		display_Right(&right_side);

		framerate++;
		count++;
		bcm2835_delay(1);
	}

}
// *********** END OF MAIN ***********

// Function to display left hand side buffer
void display_Left(MultiBuffer* targetbuffer, long currentFramerate, int count)
{
	myLCD.ActiveBuffer = targetbuffer; // set target buffer object
	myLCD.LCDclearBuffer();
	myLCD.setCursor(0, 0);
	myLCD.print("LHS Buffer");

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
	myLCD.print("V 1.0.0");
	myLCD.drawFastVLine(92, 0, 63, FOREGROUND);
	myLCD.LCDupdate();
}

// Function to display right hand side buffer
void display_Right(MultiBuffer* targetbuffer)
{
	myLCD.ActiveBuffer = targetbuffer; // set target buffer object
	myLCD.LCDclearBuffer();
	myLCD.setCursor(0, 0);
	myLCD.print("RHS buffer");

	myLCD.fillRect(0, 10, 20, 20, colour);
	myLCD.fillCircle(40, 20, 10, FOREGROUND);
	myLCD.fillTriangle(60, 30, 70, 10, 80, 30, !colour);
	myLCD.drawRoundRect(10, 40, 60, 20, 10, FOREGROUND);
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
