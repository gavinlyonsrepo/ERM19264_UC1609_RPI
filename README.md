![ ERM19264 ](https://github.com/gavinlyonsrepo/ERM19264_UC1609/blob/main/extras/image/color.jpg)

Table of contents
---------------------------

  * [Overview](#overview)
  * [Output](#output)
  * [Installation](#installation)
  * [Hardware](#hardware)
  * [Features](#features)
  * [Files](#files)
  * [Tested](#tested)
  * [Ports](#ports)
  
Overview
--------------------
* Name : ERM19264_UC1609_RPI
* Title : Library for ERM19264-5 v3 LCD  (UC1609C controller) 
* Description : 

1. Raspberry pi C++ library.      
2. Inverse, Scroll, rotate and contrast control. 
3. Extended scale-able ASCII font.
4. Graphics class included.
5. Sleep mode.
6. 3 different modes: Multi-buffer , single buffer , no buffer
7. Bitmaps supported.
8. Hardware & software SPI options

* Tested on RPI 3 Model B+
* Author: Gavin Lyons
* History: See Changelog in extras/doc folder
* Copyright: GNU GPL v3
* This is a Port of my Arduino [library](https://github.com/gavinlyonsrepo/ERM19264_UC1609)

Output
---------------------------------

Output Screenshots, From left to right top to bottom.

1. Full screen bitmap displayed
2. Multi buffer mode screen divided into two buffers
3. Different size and type of fonts 
4. ASCII font 0-127 printed out (128-255 also available

![op](https://github.com/gavinlyonsrepo/ERM19264_UC1609/blob/main/extras/image/output.jpg)


Installation
------------------------------

0. Install the C libraries of bcm2835, see: http://www.airspayce.com/mikem/bcm2835/ 
1. curl -sL https://github.com/gavinlyonsrepo/ERM19264_UC1609_RPI/archive/1.0.tar.gz | tar xz
2. cd ERM19264_UC1609_RPI_1.0
3. make 
4. sudo ./bin/test
5. There are 7 different main.cpp in the examples folder copy the one to run into src folder
    Hellworld is in there by default.

Hardware
----------------------------

9 pins , Vcc and GND, anode and cathode for the backlight LED and an SPI interface.
The backlight control is left up to user coonect to 3.3V thru a resistor.
If using Hardware SPI two of  pins will be tied to the SPI CLK and MOSI lines if using software SPI you should be able use any GPIO you want for all five pins.
Datasheets are in the extras folder. 

There are 3 different colours in range, Parts used purchased from [ebay](https://www.ebay.ie/itm/2-inch-White-192x64-Graphic-LCD-Display-Module-UC1609-SPI-for-Arduino/293617684779?hash=item445cfa512b:g:10MAAOSwYV9e6xsi)
 
1. ERM19264SBS-5 V3 LCD Display UC1609C controller ,  white on blue
2. ERM19264FS-5 V3 LCD Display  UC1609C controller , black on white
3. ERM19264DNS-5 V3 LCD Display  UC1609C controller white on black

The library was tested on 1 and 2. 
The UC1609 controller chip is a 3.3V device but the ERM LCD module has a "662k" 3.3V regulator at back.
So the ERM LCD module will run at 5V as well if this is present.
It was always run it at 3.3V during testing. 
The Backlight should always be connected to 3.3V according to datasheets.  


![ ERM19264 ](https://github.com/gavinlyonsrepo/ERM19264_UC1609_RPI/blob/main/extras/image/wiring.jpg)

Features
-------------------------

*SPI*

Hardware and software SPI. Two different class constructors. User can pick the relevant constructor, see examples files. Hardware SPI is recommended, far faster and more reliable but Software SPI allows for more flexible GPIO selection. When running Software SPI it may be necessary on very high frequency MCU's to change the UC1609_HIGHFREQ_DELAY define, It is a microsecond delay by default it is at 0. 

*buffers*

3 buffer modes 

1. MULTI_BUFFER (default)
2. SINGLE_BUFFER 
3. NO_BUFFER , Text only no buffer , relatively light weight.  Turns LCD into simple character LCD(216 characters) , Bitmaps can still be written directly to screen but no graphics possible.

To switch between modes, user must make a change to the USER BUFFER OPTION SECTION  at top of 
ERM19264_UC1609.h file.  Pick ONE option and one option ONLY. The example files at top, say which option to pick. If wrong option is picked, example files will not work or maybe even compile.

*fonts*

The scalable ASCII (extended 0-255) font is a standard 5 by 7 ASCII font with two  columns  of padding added. So 7 by 8 in effect. In standard text size and "no buffer" mode, this means: 192/7 * 64/8 = 27 * 8 = 216 characters. 

*bitmaps*

Bitmaps are written directly to screen unless Bitmap set to a buffer.
Best to use multi-buffer to share screen between bitmaps and text + graphics.
Bitmaps can be turned to data [here at link]( https://javl.github.io/image2cpp/) use vertical addressing draw mode. 

*User adjustments*

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

