
// Example file name : main.cpp
// Description:
// Test file for ERM19264_UC1609 library, showing use of fonts
// URL: https://github.com/gavinlyonsrepo/ERM19264_UC1609_RPI
// *****************************


#include <bcm2835.h>
#include <time.h>
#include <stdio.h>

#include "ERM19264_UC1609.h"

// Include the fonts for test
#include "FreeSans12pt7b.h"
#include "FreeSans18pt7b.h"
#include "FreeSerif12pt7b.h"
#include "FreeSerif18pt7b.h"

// LCD SETUP
#define LCDcontrast 0x29 //Constrast 00 to FF , 0x49 is default.
#define myLCDwidth  192
#define myLCDheight 64
// // GPIO pin number pick any you want
#define RST 25
#define CD 24
//instantiate object
ERM19264_UC1609 myLCD(myLCDwidth ,myLCDheight, RST, CD );

#define MY_TEST_DELAY2 bcm2835_delay(2000)
#define MY_TEST_DELAY1 bcm2835_delay(1000)

// =============== Function prototype ================
void setup(void);
void myTest(void);
void EndTest(void);
void testReset(void);

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
	bcm2835_delay(1500);
}

void EndTest()
{
	myLCD.LCDPowerDown();
	bcm2835_close(); // Close the library
	printf("LCD End\r\n");
}

void myTest() {

	// define a buffer to cover whole screen
	uint8_t  screenBuffer[myLCDwidth * (myLCDheight/8)];

	// Declare a buffer struct
	MultiBuffer mybuffer;

	// Intialise that struct with buffer details (&struct,  buffer, w, h, x-offset,y-offset)
	myLCD.LCDinitBufferStruct(&mybuffer, screenBuffer, myLCDwidth, myLCDheight, 0, 0);

	// Assign address of struct to be the active buffer pointer
	myLCD.ActiveBuffer = &mybuffer;
	myLCD.LCDclearBuffer();   // Clear active buffer

	// Test 1 Font FreeSans12pt7b
	const char *txt = "FreeSans               12pt7b";
	myLCD.setFontGlyph(&FreeSans12pt7b);
	myLCD.drawTextGlyph(0, 18, txt, FOREGROUND, 1);
	myLCD.drawCharGlyph(10, 62, '!', FOREGROUND, 1);
	testReset();

	// Test 2 Font FreeSans18pt7b
	txt = "Free Sans   18pt7b";
	myLCD.setFontGlyph(&FreeSans18pt7b);
	myLCD.drawTextGlyph(0, 26, txt, FOREGROUND, 1);
	testReset();

	// Test 3 Font FreeSerif12pt7b
	txt = "FreeSerif                12pt7b";
	myLCD.setFontGlyph(&FreeSerif12pt7b);
	myLCD.drawTextGlyph(0, 18, txt, FOREGROUND, 1);
	myLCD.drawCharGlyph(10, 62, '!', FOREGROUND, 1);
	testReset();

	// Test 4 Font FreeSErif18pt7b
	txt = "Free Serif     18pt7b";
	myLCD.setFontGlyph(&FreeSerif18pt7b);
	myLCD.drawTextGlyph(0, 26, txt, FOREGROUND, 1);
	testReset();

	// Test 5 compare 1 & 3
	txt = "FreeSans12pt7b";
	myLCD.setFontGlyph(&FreeSans12pt7b);
	myLCD.drawTextGlyph(0, 18, txt, FOREGROUND, 1);
	txt = "FreeSerif12pt7b";
	myLCD.setFontGlyph(&FreeSerif12pt7b);
	myLCD.drawTextGlyph(0, 46, txt, FOREGROUND, 1);
	testReset();

	// Test 6 display a number
	int count=0;
	char myStr[10];
	myLCD.setFontGlyph(&FreeSans12pt7b);

	while(count < 1000)
	{
		sprintf(myStr, "%d", count);
		myLCD.drawTextGlyph(0, 18, myStr, FOREGROUND, 1);
		count+= 23;
		myLCD.LCDupdate();
		MY_TEST_DELAY1;
		myLCD.LCDclearBuffer();
	}
}

void testReset()
{
	myLCD.LCDupdate();  //write to active buffer
	MY_TEST_DELAY2;
	myLCD.LCDclearBuffer();   // Clear active buffer
}
// ============== EOF =========
