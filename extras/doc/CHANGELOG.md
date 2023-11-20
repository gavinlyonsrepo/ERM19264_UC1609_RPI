# Changelog

* version 1.0.0 May 2021
	* first release

* Version 1.1 June 2021
	* added fonts

* Version 1.4 Feb 2022
	* drawBitmap() function is now set up for both horizontal and vertical addressed bitmap data,
	Vertical is default. The addressing mode is changed by setDrawBitmapAddr(), new function.  
	* Enum added for font name labels instead of raw numbers, 
	This may cause font compiler warnings for sketch's written on versions before 1.4.0 , 
 	The sketch's will still compile and work,
	Simply replace font numbers  with the relevant enum text labels to get rid of warnings. 
	* A new function added for initialising a multibuffer struct , LCDinitBufferStruct(),
	backward compatible with old manual workflow. 

* Version 1.5 Dec 2022
	* Added Homespun + tiny fonts
	* Added DrawText method
	* Allowed User to pick SPI bus baudrate and Chip enable pin in "begin" method.
	* Changed UC1609_POWER_CONTROL from 0x2F to 0x28(to default charge pump current)

* Version 1.7.0 Nov 2023
	* Added support for string objects in the print class. 
	* Changed multi-buffer mode to a single buffer mode.
	* Allowed user to adjust Ram address control setting in Begin methods arguments.
