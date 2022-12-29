
// Example file name : main.cpp
// Description:
// Test file for ERM19264_UC1609 library, showing use of mulitple buffers.  
// In this case: two, divided horizontally 
// The user can also divide vertically and create as many buffers as they want.
// URL: https://github.com/gavinlyonsrepo/ERM19264_UC1609_RPI
// *****************************
// Measured frame rate 214 fps at 64 clock divider on Rpi 3


#include <bcm2835.h>
#include <time.h>
#include <stdio.h>
#include "ERM19264_UC1609.hpp"

// LCD setup
#define LCDcontrast 0x25
#define myLCDwidth  192
#define myLCDheight 64
// GPIO
#define RES 25 // GPIO pin number pick any you want
#define DC 24 // GPIO pin number pick any you want
 // instantiate  an object
ERM19264_UC1609 myLCD(myLCDwidth ,myLCDheight , RES, DC) ;

// vars for the test
uint16_t count  = 0;
bool colour = 1;
uint64_t  previousCounter =0;

// =============== Function prototype ================
void setup(void);
void myTest(void);
void EndTest(void);
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
	myLCD.LCDbegin(LCDcontrast); // initialize the LCD
	myLCD.LCDFillScreen(0x01);
	bcm2835_delay(2000);
}

void myTest() {


	myLCD.setTextColor(FOREGROUND);
	myLCD.setTextSize(1);
	uint8_t  screenBuffer[(myLCDwidth * (myLCDheight / 8)) / 2];

	MultiBuffer left_side;
	// Intialise that struct with buffer details (&struct,  buffer, w, h, x-offset,y-offset)
	myLCD.LCDinitBufferStruct(&left_side, screenBuffer, myLCDwidth/2, myLCDheight, 0, 0);
	
	MultiBuffer right_side;
	// Intialise that struct with buffer details (&struct,  buffer, w, h, x-offset,y-offset)
	myLCD.LCDinitBufferStruct(&right_side, screenBuffer, myLCDwidth/2, myLCDheight, myLCDwidth/2, 0);
	
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
	myLCD.print("V 1.5");
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

// ******************** EOF *********************8
