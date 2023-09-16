![ ERM19264 ](https://github.com/gavinlyonsrepo/ERM19264_UC1609/blob/main/extras/image/color.jpg)

# ERM19264_UC1609_RPI

## Table of contents

  * [Overview](#overview)
  * [Output](#output)
  * [Installation](#installation)
  * [Test](#test)
  * [Hardware](#hardware)
  * [Software](#software)
	* [SPI](#spi)
	* [Fonts](#fonts)
	* [Bitmaps(#bitmaps)
	* [User Adjustments](#user-adjustments)
  * [Notes and Issues](#notes-and-issues)

## Overview

* Name : ERM19264_UC1609_RPI
* Title : Library for ERM19264-5 v3 LCD (UC1609C controller)
* Description :

1. Dynamic install-able system level Raspberry Pi C++ library.
2. Inverse, Scroll, rotate and contrast control.
3. 6 standard ASCII font + glyph font support
4. Graphics class included.
5. Sleep mode.
6. Multi-buffer mode
7. Bitmaps supported.
8. Hardware & software SPI options
9. Dependency: bcm2835 Library

* Author: Gavin Lyons
* Copyright: GNU GPL v3
* This is a Port of my Arduino [library](https://github.com/gavinlyonsrepo/ERM19264_UC1609)
* Developed on 
	1. Raspberry PI 3 model b, 
	2. C++ complier g++ (Raspbian 8.3.0-6)
	3. Raspbian 10 buster  OS
	4. bcm2835 Library 1.71 (Dependency)
    
## Output

Output Screenshots, From left to right top to bottom.

1. Full screen bitmap displayed
2. Multi buffer mode screen divided into two buffers
3. Different size and inverted ASCII fonts.
4. ASCII font 0-127 printed out (128-255 also available)
5. Glyph fonts 

![op](https://github.com/gavinlyonsrepo/ERM19264_UC1609/blob/main/extras/image/output.jpg)
![op](https://github.com/gavinlyonsrepo/ERM19264_UC1609_RPI/blob/main/extras/image/font.jpg)

## Installation


1. Make sure SPI bus is enabled on your raspberry PI

2. Install the dependency bcm2835 Library if not installed (at time of writing latest version is 1.68.)
	* The bcm2835 library is a dependency and provides SPI bus, delays and GPIO control.
	* Install the C libraries of bcm2835, [Installation instructions here](http://www.airspayce.com/mikem/bcm2835/)

3. Download the ERM19264_UC1609_RPI library 
	* Open a Terminal in a folder where you want to download,build & test library
	* Run following command to download from github.
    
```sh
curl -sL https://github.com/gavinlyonsrepo/ERM19264_UC1609_RPI/archive/1.5.tar.gz | tar xz
```

4. Run "make" to run the makefile to install library, it will be 
    installed to usr/lib and usr/include
    
```sh
cd ERM19264_UC1609_RPI-1.5
sudo make
```

## Test

1. Next step is to test LCD and installed library with an example.
Wire up your LCD. Next enter the examples folder and run the makefile in THAT folder, 
This makefile builds the examples file using the just installed library.
and creates a test exe file in "bin". Be sure to use "sudo" as the bcm2835 requires root permissions by default [ see here for more details on that](http://www.airspayce.com/mikem/bcm2835/) 
The default example file is "hello world",  user should see "hello world" 
on the LCD screen by end of this step.

```sh
cd examples/
make
sudo bin/test
```

2. There are seven examples files to try out. 
To decide which one the makefile builds simply edit "SRC" variable at top of the makefile in examples folder. In the "User SRC directory Option Section". Pick an example "SRC" directory path and ONE ONLY. Comment out the rest and repeat step 1.


## Hardware

9 pins , Vcc and GND, anode and cathode for the backlight LED and an SPI interface.
The backlight control is left up to user , connect to 3.3V thru a resistor.
If using Hardware SPI 3 pins will be tied to the SPI CEO, CLK and MOSI lines. if using software SPI you should be able use any GPIO you want for all five pins. Datasheets URLS are in the extras folder.

There are 3 different colours in range, Parts used purchased from [ebay](https://www.ebay.ie/itm/2-inch-White-192x64-Graphic-LCD-Display-Module-UC1609-SPI-for-Arduino/293617684779?hash=item445cfa512b:g:10MAAOSwYV9e6xsi)

1. ERM19264SBS-5 V3 LCD Display UC1609C controller ,  white on blue
2. ERM19264FS-5 V3 LCD Display  UC1609C controller , black on white
3. ERM19264DNS-5 V3 LCD Display  UC1609C controller white on black

The UC1609 controller chip is a 3.3V device but the ERM LCD module has a "662k" 3.3V regulator at back.
So the ERM LCD module will run at 5V as well if this is present.
It was always run it at 3.3V during testing.


![ ERM19264 ](https://github.com/gavinlyonsrepo/ERM19264_UC1609_RPI/blob/main/extras/image/wiring.png)

## Software

### SPI

Hardware and software SPI. Two different class constructors. User can pick the relevant constructor, see examples files. Hardware SPI is recommended, far faster and more reliable but Software SPI allows for more flexible GPIO selection. When running Software SPI it may be necessary on very high frequency MCU's to change the UC1609_HIGHFREQ_DELAY define, It is a microsecond delay by default it is at 0.

The SPI settings are in LCDSPIon function.
Speed is currently at BCM2835_SPI_CLOCK_DIVIDER_64. 
6.25MHz on RPI3. This can be adjusted in code or user can pass 
the divider value in the "begin" method as a parameter. These values are
defined by enum bcm2835SPIClockDivider. For full list see
[link.](http://www.airspayce.com/mikem/bcm2835/group__constants.html#gaf2e0ca069b8caef24602a02e8a00884e)
User can also adjust which SPI chip enable pin the use uing "begin" method parameter.


### Fonts

There are 6 standard scale-able ASCII fonts.
A print class is available to print out most passed data types.
The fonts 1-6 are byte high(at text size 1) scale-able fonts, columns of padding added by SW.
The example file TEXT_GRAPHICS shows how to use these. 

Six standard fonts available : 

| Font num | Font name | Font size xbyy |  Note |
| ------ | ------ | ------ | ------ |  
| 1 | Default | 5x8 | Full Extended ASCII 0 - 0xFF |
| 2 | Thick   | 7x8 | no lowercase letters , ASCII  0x20 - 0x5A |
| 3 | Seven segment | 4x8 | ASCII  0x20 - 0x7A |
| 4 | Wide | 8x8 | no lowercase letters, ASCII 0x20 - 0x5A |
| 5 | Tiny | 3x8 | ASCII  0x20 - 0x7E |
| 6 | Homespun  | 7x8 | ASCII  0x20 - 0x7E |

In addition to those 6 fonts, The ability to use glyph-fonts is also
available. I have packaged four fonts with the library, these are not installed
in the system but are in the include folder of examples. 
The example file FONTS_GLYPH shows how to use these.
The Font structures are in the ERM19264_gfxfont.h file. 
To use a font in your program, #include the corresponding .h
file and pass address of GFXfont struct to setFont().
Other fonts are available from the 
[ Adafruit GFX arduino library](https://github.com/adafruit/Adafruit-GFX-Library/).
If you copy over other fonts remove the PROGMEM keyword. Lots more detail there on
how these fonts are created and work.
The glyph-fonts use 3 special functions: setFontGlyph drawCharGlyph drawTextGlyph.
They cannot currently use the print class, also cannot be inverted.
Inverting only works with the standard fonts because the characters are a uniform size. It's not a sensible thing to do with proportionally-spaced fonts with glyphs of varying sizes and that may overlap.  


### Bitmaps

There is a few different ways of displaying bitmaps, 

| Num | Method | Buffer mode |   Data addressing | Note |
| ------ | ------ | ------ | ------ |  ------ |  
| 1 | LCDBitmap() | any  | Vertical |  Writes directly to screen , no buffer used. | 
| 2 | LCDBuffer() | Multi or Single |  Vertical  |  For internal use mostly | 
| 3 | Multi buffer init  | Multibuffer | Vertical  |  Can be used when initialising a MB | 
| 4 | drawBitmap() | Multi or Single | Vertical | default,  setDrawBitmapAddr(true) | 
| 5 | drawBitmap() | Multi or Single |  Horizontal | setDrawBitmapAddr(false) |

See the bitmap example file for more details on each method. Bitmaps can be turned to data [here at link]( https://javl.github.io/image2cpp/) , Bitmaps should be defined as const  buffers non-const, for methods 3 buffer can be initialised with bitmap data.

### User Adjustments

When the user calls LCDbegin() to start LCD they can specify a contrast setting from 0x00 to 0xFF.
Datasheet says 0x49 is default. (VbiasPOT). Lower contrast works better on the blue version.
It is also possible for user to change LCD bias ,  Temperature coefficient, frame rate and power control but this must be done by changing defines in header file. Choose lower frame rate for lower power, and choose higher frame rate to improve LCD contrast and minimize flicker. See Data sheet for range of values
here. Defaults where found to be fine during all testing of this library.

| Parameter | default Values |  Define | Register |
| ------ | ------ |  ------ | ------ |
| LCD bias |  9 | BIAS_RATIO_SET | BR 1:0 |
| Temp coefficient | -0.00%/ C |  TEMP_COMP_SET | TC 1:0  |
| Frame rate | 95 fps |  FRAMERATE_SET |  LC 4:3 |
| Power control | 1.4mA + internal V LCD |  PC_SET | PC 2:0 |
| V bias Bot(contrast) | 0x49h default|  Set by user with LCDbegin | PM 7:0 |

## Notes and Issues

Some users(Of the arduino  library) have reported the LCD not initialising correctly with this software.
It was found that by adjusting the UC1609_ADDRESS_SET setting from 0x02 to 0x01.
it resolved problem.  This setting is on line 41 of ERM19264_UC1609.h file.
See [ github issue 4](https://github.com/gavinlyonsrepo/ERM19264_UC1609/issues/4) for details. I suspect the root cause is different versions of product on market.
In a future version I will allow user to adjust this setting in "LCDbegin" method parameters.
