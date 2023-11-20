// Example file name : main.cpp
// Description:
// Test file for ERM19264_UC1609 library, showing use of TEXT & graphics.
// URL: https://github.com/gavinlyonsrepo/ERM19264_UC1609_RPI
// *****************************


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

// instantiate  an object for HW SPI
ERM19264_UC1609 myLCD(myLCDwidth ,myLCDheight , RST, CD) ; 

// Vars for test
#define DisplayDelay1 9000
#define DisplayDelay2 5
#define PI 3.14

// =============== Function prototype ================
void setup(void);
void myTest(void);
void DisplayText();
void DisplayGraphics();
void EndTest(void);
void TestReset(void);

// ======================= Main ===================
int main(int argc, char **argv)
{
	if(!bcm2835_init()){return -1;}

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
	myLCD.LCDFillScreen(0x8F); //splash screen bars
	myLCD.setTextWrap(true);
	bcm2835_delay(2500);
}

void EndTest()
{
	myLCD.LCDPowerDown();
	bcm2835_close(); // Close the library, 
	printf("LCD End\r\n");
}

void TestReset()
{
	myLCD.LCDupdate();  // Write to the buffer
	bcm2835_delay(DisplayDelay1);
	myLCD.LCDclearBuffer();
}

void myTest()
{

	// define a buffer to cover whole screen 
	uint8_t screenBuffer[myLCDwidth * (myLCDheight/8)]; 
	myLCD.LCDbufferScreen = (uint8_t*) &screenBuffer;
	myLCD.LCDclearBuffer();   // Clear buffer 
	
	// Call a function to display text
	DisplayText();
	DisplayGraphics();
}

// A series of tests to display the text mode
// Test 1 Font size 3 float
// Test 2 font size 2 integer
// Test 3 font size 1 string inverted
// Test 4 draw a single character font size 4
// Test 5 print ASCII  font 0-127
// Test 6 print ASCII font 128-255,
// Test 7 thick font 2 (NO LOWERCASE)
// Test 8 seven seg font 3
// Test 9 wide font (NO LOWERCASE)
// Test 10 tiny font 5
// Test 11 Homespun font 6
// Test 12 print class
// Test 13 drawtext
// Test 14 string object method using print class
void DisplayText()
{

	myLCD.LCDclearBuffer(); // Clear the buffer
	
	// Test 1
	myLCD.setTextColor(FOREGROUND);
	myLCD.setTextSize(3);
	myLCD.setCursor(0, 0);
	myLCD.print(PI);

	// Test 2
	myLCD.setTextSize(2);
	myLCD.setCursor(0, 30);
	int count = -123;
	myLCD.print(count);

	// Test 3
	myLCD.setTextSize(1);
	myLCD.setTextColor(BACKGROUND, FOREGROUND);
	myLCD.setCursor(0, 50);
	myLCD.print("Hello World");

	// Test 4
	myLCD.drawChar(95, 15 , 'H', FOREGROUND, BACKGROUND, 6);

	TestReset();

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
	TestReset();

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

	TestReset();
	
	// Test 7
	myLCD.setFontNum(UC1609Font_Thick);
	myLCD.setTextSize(1);
	myLCD.setCursor(0, 0);
	myLCD.print("THE THICK FONT");
	myLCD.setCursor(0, 9);
	myLCD.print("1234567890123456ABCDEFGH");
	myLCD.drawChar(70, 20 , 'H', FOREGROUND, BACKGROUND, 4);
	myLCD.setCursor(0, 45);
	myLCD.setTextSize(2);
	myLCD.print("TEST");
	TestReset();
	
	// Test 8
	myLCD.setFontNum(UC1609Font_Seven_Seg);
	myLCD.setTextSize(1);
	myLCD.setCursor(0, 0);
	myLCD.print("SEVEN SEG Font");
	myLCD.setCursor(0, 9);
	myLCD.print("123456789012345678012345abcABCDEF");
	myLCD.setCursor(0, 30);
	myLCD.setTextSize(5);
	myLCD.print("14:30");
	TestReset();
	
	// Test 9
	myLCD.setFontNum(UC1609Font_Wide);
	myLCD.setTextSize(1);
	myLCD.setCursor(0, 0);
	myLCD.print("WIDE FONT");
	myLCD.setCursor(0, 9);
	myLCD.print("12345678901234ABCDEF");
	myLCD.setCursor(0, 30);
	myLCD.setTextSize(2);
	myLCD.print("13:57");
	myLCD.drawChar(120, 20 , 'H', FOREGROUND, BACKGROUND, 4);
	TestReset();
	
	// Test 10
	myLCD.setFontNum(UC1609Font_Tiny);
	myLCD.setTextSize(1);
	myLCD.setCursor(0, 0);
	myLCD.print("tiny FONT");
	myLCD.setCursor(0, 9);
	myLCD.print("12345678901234ABCDEF");
	myLCD.setCursor(0, 30);
	myLCD.setTextSize(2);
	myLCD.print("11:59");
	myLCD.drawChar(120, 20 , 'H', FOREGROUND, BACKGROUND, 4);
	TestReset();
	
	// Test 11
	myLCD.setFontNum(UC1609Font_Homespun);
	myLCD.setTextSize(1);
	myLCD.setCursor(0, 0);
	myLCD.print("homespun font");
	myLCD.setCursor(0, 9);
	myLCD.print("12345678901234ABCDEF");
	myLCD.setCursor(0, 30);
	myLCD.setTextSize(2);
	myLCD.print("12:52");
	myLCD.drawChar(120, 20 , 'H', FOREGROUND, BACKGROUND, 4);
	TestReset();
	
	// Test 12
	myLCD.setFontNum(UC1609Font_Default);
	myLCD.setTextSize(1);
	myLCD.setCursor(0, 0);
	myLCD.print(47 , DEC);
	myLCD.setCursor(0, 10);
	myLCD.print(47 , HEX); 
	myLCD.setCursor(0, 20);
	myLCD.print(47, BIN);
	myLCD.setCursor(0, 30);
	myLCD.print(47 , OCT);
	myLCD.setCursor(0, 40);
	myLCD.print(-3.788, 2); // print -3.79
	TestReset();
	
	// Test 13
	char myString[9] = {'1', '3', ':', '2', '6', ':', '1', '8'};
	myLCD.setFontNum(UC1609Font_Tiny);
	myLCD.drawText(0,0, myString, FOREGROUND, BACKGROUND,1);
	myLCD.setFontNum(UC1609Font_Wide);
	myLCD.drawText(0,32, myString, FOREGROUND, BACKGROUND,2);
	TestReset();
	
	// Test 14 string method string object method using print
	std::string timeInfo = "12:45";
	myLCD.setCursor(0, 0);
	myLCD.print(timeInfo);
	TestReset();
	
} // end DisplayText

// Function to display Graphics.
void  DisplayGraphics()
{
//Q1 ||  Q2
//---------
//Q3 ||  Q4
//
bool colour = 1;
uint8_t count = 0;
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
} // end Display graphics


// *************** EOF *********************
