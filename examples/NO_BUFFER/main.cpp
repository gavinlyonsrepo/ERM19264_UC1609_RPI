
// Example file name : main.cpp
// Description:
// Test file for ERM19264_UC1609 library, showing use of "No buffer" mode .
// in this mode the LCD can be used as a relatively light weight character LCD
// 8 rows and 216 characters, 7by8 ,font size (192/7 * 64/8 ),
// you can also write bitmaps directly to screen in this mode.
// URL: https://github.com/gavinlyonsrepo/ERM19264_UC1609_RPI
// *****************************
// NOTES :
// (1) In the <ERM19264_UC1609.h> USER BUFFER OPTION SECTION, at top of file
// option NO_BUFFER must be selected and only this option.
// ******************************

#include <bcm2835.h>
#include "ERM19264_UC1609.h"
#include <time.h>
#include <signal.h>

#define LCDcontrast 0x49 // Contrast 00 to FF , 0x49 is default. user adjust
#define myLCDwidth  192
#define myLCDheight 64

// GPIO 
#define RST 25 // GPIO pin number pick any you want
#define CD 24 // GPIO pin number pick any you want 

ERM19264_UC1609 myLCD(myLCDwidth ,myLCDheight , RST, CD) ; // instantiate  an object 

// 'small image', 20x20px
const uint8_t smallImage [] = {
	0xff, 0x3f, 0x0f, 0x07, 0x03, 0x13, 0x33, 0x39, 0x39, 0x79, 0xf9, 0xf9, 0xfb, 0xf3, 0xf7, 0xe3,
	0x87, 0x0f, 0x1f, 0xff, 0xf9, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x0f,
	0x1d, 0x19, 0x10, 0x19, 0x0f, 0x00, 0xc0, 0xf0, 0x0f, 0x0f, 0x0f, 0x0e, 0x0c, 0x0c, 0x08, 0x08,
	0x08, 0x00, 0x00, 0x08, 0x08, 0x08, 0x0c, 0x0c, 0x0e, 0x0f, 0x0f, 0x0f
};

// =============== Function prototype ================
void setup(void);
void Tests(void);

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
	Tests();
	bcm2835_delay(2000);
	myLCD.LCDSPIoff();
	myLCD.LCDPowerDown();
	bcm2835_close(); // Close the library, deallocating any allocated memory and closing /dev/mem
	printf("LCD End\r\n");
	return 0;
}
// ================= End of main ================

// ============ Function space ==============

void setup()
{
	myLCD.LCDbegin(LCDcontrast); // initialize the LCD
	myLCD.LCDFillScreen(0x33, 0); // Fill screen with effect
	bcm2835_delay(2000);
}

//Function to run a group of tests
// Test 0 clear screen
// Test 0.5 bitmap
// Test 1 String function and goto function
// Test 2 clear page function
// Test 3 character function
// Test 4 print entire ASCII font with character function 0-127 
// Test 5 print extended ASCII 128-255

void Tests()
{
	//Test 0 clear screen

	myLCD.LCDFillScreen(0x00, 0); // Clear the screen
	bcm2835_delay(500);

	// Test0.5 bitmap
	myLCD.LCDBitmap(10, 10 , 20, 20, smallImage);
	bcm2835_delay(2000);

	unsigned char test0_1[] = "This is page 0";
	unsigned char test1_2[] = "123456789012345678901234567";
	unsigned char test1_3[] = "This is page 7";

	// Test 1 String function and goto function
	myLCD.LCDNoBufferGotoXY(0, 0); // (Col , page ) Col 0 to 191 , page 0 to 7
	myLCD.LCDNoBufferString(test0_1); // Write a character string
	myLCD.LCDNoBufferGotoXY(0, 4);
	myLCD.LCDNoBufferString(test1_2);
	myLCD.LCDNoBufferGotoXY(0, 7);
	myLCD.LCDNoBufferString(test1_3);
	bcm2835_delay(5000);

	// Test 2 clear page function

	myLCD.LCDFillPage(7, 0x00, 0); // Clear page
	myLCD.LCDFillPage(6, 0x7E, 0); // Write pattern (0111 1110) to a page
	bcm2835_delay(5000);
	myLCD.LCDFillScreen(0x00, 0); // Clear the screen

	// Test 3 character function
	myLCD.LCDNoBufferGotoXY(100, 2);
	myLCD.LCDNoBufferChar('G');  // write single  character
	myLCD.LCDNoBufferChar('L');
	bcm2835_delay(2000);
	myLCD.LCDFillScreen(0x00, 0); // Clear the screen

	// Test 4 print entire font with character function
	myLCD.LCDNoBufferGotoXY(0, 0);
	uint8_t row = 1;
	char offset = 0;
	for (unsigned char i = offset; i < 126 ; i++)
	{
		if (i % 27 == 0) myLCD.LCDNoBufferGotoXY(0, row++);
		myLCD.LCDNoBufferChar(i);
		bcm2835_delay(15);
	}
	bcm2835_delay(3000);
	myLCD.LCDFillScreen(0x00, 0); // Clear the screen

	// TEST 5 print ASCII font 128-255 with character function
	// For characters after 'z{|}' 

	myLCD.LCDNoBufferGotoXY(0, 0);
	row = 1;
	for (unsigned char k = 128; k < 255 ; k++)
	{
		if (k % 27 == 0) myLCD.LCDNoBufferGotoXY(0, row++);
		myLCD.LCDNoBufferChar(k);
		bcm2835_delay(15);
	}
	bcm2835_delay(3000);
	myLCD.LCDFillScreen(0x00, 0); // Clear the screen

}
