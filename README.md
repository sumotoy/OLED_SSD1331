OLED_SSD1331
============

ATTENTION!
This library it's now using a modified version of Adafruit_GFX library https://github.com/sumotoy/Adafruit-GFX-Library
That it's faster and almost 100& compatible with regular one (apart the new text rendering that support multiple fonts).
If you plan to use the original Adafruit_GFX library you can still use it! Just uncomment this line in .cpp file of this library:

//#include "glcdfont.c" //comment out if you are using the custom version of Adafruit_GFX!!!!!!!!!

That's it.
-------------------------------------------------------------------------------------------------------------------------

This it's a Hyper Fast library for drive solomon tech SSD1332 OLED displays. It's compatible with Arduino, Arduino DUE and Teensy 3.x and uses native SPI.
The features are:
 - Hyper Fast speed. On Teensy 3.x it uses SPI DMA at 24Mhz.
 - Compatible with Adafruit protocol, it uses Adafruit GFX library and it's fully supported.
 - Works with serial SPI protocol so only 5 or 4 pin are needed (2 of them still shareable).
 - Massive use of hardware accellerated commands for SSD chip.


Version History:
 - 0.5b1: First working release with full tested 24Mhz/DMA SPI on Teensy 3.
 - 0.5b2: Even faster!
 - 0.6:Compatible with Teensyduino 1.24

 
Note:
If you are using a chinese module from Ebay you should pay attention if it's already setup for SPI serial.
If not, follow instructions of the seller to configure it.
Some chinese module have been poorly designed so the supply 3V3 volt cannot be used since it's usually not drive correctly the internal hi-voltage dc converter. In that case use the 5v pin of the module, you can still drive it with 3v3 logic since it's compatible but leave unconnected the 3V3 pin.
