// Example file name : main.cpp
// Description:
// Test file for ERM19264_UC1609 library, showing use of TEXT  mode .
// URL: https://github.com/gavinlyonsrepo/ERM19264_UC1609_RPI
// *****************************
// NOTES :
// (1) In the <ERM19264_UC1609.h> USER BUFFER OPTION SECTION, at top of file
// option MULTI_BUFFER must be selected and only this option.
// ******************************

#include <bcm2835.h>
#include "ERM19264_UC1609.h"
#include <time.h>
#include <stdio.h>

#define LCDcontrast 0x49 //Constrast 00 to FF , 0x80 is default. user adjust
#define myLCDwidth  192
#define myLCDheight 64

#define DisplayDelay1 5000
#define DisplayDelay2 0
#define PI 3.14

// GPIO
#define RST 25 // GPIO pin number pick any you want
#define CD 24 // GPIO pin number pick any you want

ERM19264_UC1609 myLCD(myLCDwidth ,myLCDheight , RST, CD) ; // instantiate  an object

// =============== Function prototype ================
void setup(void);
void myLoop(void);
void DisplayText(MultiBuffer* );
void DisplayGraphics(MultiBuffer* );

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
	bcm2835_close(); // Close the library, 
	printf("LCD End\r\n");
	return 0;
}
// ======================= End of main  ===================

// ===================== Function Space =====================
void setup() {
	myLCD.LCDbegin(LCDcontrast); // initialize the LCD
	myLCD.LCDFillScreen(0x8F, 0); //splash screen bars
	bcm2835_delay(3000);
}

// ************** MAIN LOOP ***********
void myLoop()
{

	// Define a full screen buffer
	uint8_t  textBuffer[(myLCDwidth * (myLCDheight / 8)) + 1];
	MultiBuffer window;
	window.screenbitmap = (uint8_t*) &textBuffer;
	window.width = myLCDwidth;
	window.height = myLCDheight;
	window.xoffset = 0;
	window.yoffset = 0;
	
	// Call a function to display text
	DisplayText(&window);
	DisplayGraphics(&window);
}
// ************** END OF MAIN ***********

// A series of tests to display the text mode
// Test 1 Font size 3 float
// Test 2 font size 2 integer
// Test 3 font size 1 string inverted
// Test 4 draw a single character font size 4
// Test 5 print ASCII  font 0-127
// Test 6 print ASCII font 128-255,

void DisplayText(MultiBuffer* targetBuffer)
{

	myLCD.setTextWrap(true);
	myLCD.ActiveBuffer =  targetBuffer;
	myLCD.LCDclearBuffer(); // Clear the buffer
	// Test 1
	myLCD.setTextColor(FOREGROUND);
	myLCD.setTextSize(3);
	myLCD.setCursor(0, 0);
	myLCD.print(PI);

	// Test 2
	myLCD.setTextSize(2);
	myLCD.setCursor(0, 30);
	int count = 123;
	myLCD.print(count);

	// Test 3
	myLCD.setTextSize(1);
	myLCD.setTextColor(BACKGROUND, FOREGROUND);
	myLCD.setCursor(0, 50);
	myLCD.print("Hello World");

	// Test 4
	myLCD.drawChar(95, 15 , 'H', FOREGROUND, BACKGROUND, 6);

	myLCD.LCDupdate();  // Write to the buffer
	bcm2835_delay(DisplayDelay1);
	myLCD.LCDclearBuffer();

	// Test 5
	myLCD.setCursor(0, 0);
	myLCD.setTextColor(FOREGROUND);
	myLCD.setTextSize(1);
	myLCD.print("ASCII font ");
	myLCD.setCursor(0, 15);
	char offset = 0;

	// Print first 127 chars of font
	for (char i = offset; i < 126; i++)
	{
		if (i == '\n' || i == '\r') continue;
		myLCD.print(i);
		bcm2835_delay(DisplayDelay2);
	}
	myLCD.LCDupdate();  // Write to the buffer
	bcm2835_delay(DisplayDelay1);
	myLCD.LCDclearBuffer();

	myLCD.setCursor(0, 0);
	myLCD.setTextColor(FOREGROUND);
	myLCD.setTextSize(1);
	myLCD.print("ASCII font 128-255");

	uint8_t x = 0;
	uint8_t y = 15;
	myLCD.setCursor(x, y);

	for (uint8_t i = 128; i < 255; i++)
	{
		if (x > 180)
		{
			x = 0;
			y += 9;
		}
		myLCD.drawChar(x, y , i, FOREGROUND, BACKGROUND, 1);
		x += 7;
		bcm2835_delay(DisplayDelay2);
	}

	myLCD.LCDupdate();  // Write to the buffer
	bcm2835_delay(DisplayDelay1);
	myLCD.LCDclearBuffer();

} // end

// Function to display Graphics.
void  DisplayGraphics(MultiBuffer* targetBuffer)
{
  //Q1 ||  Q2
  //---------
  //Q3 ||  Q4
  //
  bool colour = 1;
  uint8_t count = 0;
  myLCD.ActiveBuffer =  targetBuffer;   // Set the buffer struct object
  myLCD.LCDclearBuffer(); // Clear the buffer
  while (count < 15)
  {
    colour = !colour;

    // Draw the X
    myLCD.drawLine(96,  0, 96, 64, FOREGROUND);
    myLCD.drawFastVLine(94, 0, 64, FOREGROUND);
    myLCD.drawFastHLine(0, 32, 192, FOREGROUND);

    //Q1
    myLCD.fillRect(0, 10, 20, 20, colour);
    myLCD.fillCircle(40, 20, 10, FOREGROUND);
    myLCD.fillTriangle(60, 30, 70, 10, 80, 30, !colour);
    //Q2
    myLCD.drawRect(100, 10, 20, 20, FOREGROUND);
    myLCD.drawCircle(140, 20, 10, colour);
    myLCD.drawTriangle(160, 30, 170, 10, 180, 30, FOREGROUND);
    //Q3
    myLCD.fillRoundRect(0, 40, 40, 20, 10, !colour);
    myLCD.fillRoundRect(45, 40, 40, 20, 10, colour);
    //Q4
    char i;
    for (i = 0; i < 10; i ++)
    {
      myLCD.drawRect(100 + i, 40 + i, 80 - i * 2, 20 - i * 2, FOREGROUND);
      myLCD.LCDupdate();
      bcm2835_delay(50);
    }
    myLCD.LCDclearBuffer();
    count++;
  }
}
