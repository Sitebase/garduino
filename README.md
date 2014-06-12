Garduino
========

Components
----------
* [Moisture sensor](https://www.youtube.com/watch?v=Pm83L104naY)
* Humidity and Temperature (DHT11) - see the Arduino libraries folder, then dht and there is an exmaple of how to use it
* Light (LDR) - 

Build
-----
In the root folder of the project run:

	ino build

Build and install to my Arduino pro mini:

	ino build -m pro5v328
	ino upload -m pro5v328

Debug
-----
If the build gives errors try to run:

	ino clean
	ino build

Todo
----
* Plant profiles for optimized growing of specific herbs
* dht.getStatusString() returns TIMEOUT when connection problem and OK when connection is working. Maybe check for these values
* Water level sensor for the reservoir
* Add pH sensor
* Button to force start watering
* Add status lights for pump (in a later stage maybe switch to a rgb led or something that can represent the health of the plant)
* Add solar panel to power the arduino
* Buy water proof box for the electronics
* Maybe use interrupts for button based actions so we can use a slow main loop that only checks sensors each 30 seconds or so (more energy efficient). The buttons can then be directly handled when pushed.
* Use bandgap to detect low battery (see Interesting read), also see [this for a code example](http://forum.arduino.cc/index.php?topic=88935.0)
* Run from batteries instead of PSU

Interesting read
----------------
* [Microcontroller assisted gardening](http://www.make-digital.com/make/vol18/?pg=94#pg94)
* [pH levels for herbs](http://www.gardenersnet.com/atoz/phlevel3.htm)
* [Change pH level of soil](http://www.clemson.edu/extension/hgic/plants/other/soils/hgic1650.html)
* [Standalone Atmega](http://arduino.cc/en/Main/Standalone)
* [Command line build and upload your Arduino project with ino](http://inotool.org/)
* [Quick start with ino](http://inotool.org/quickstart)
* [Multithreading](http://arduino.cc/en/Tutorial/MultipleBlinks)
* [Arduino libray list](http://playground.arduino.cc/Main/LibraryList#Sched)
* [Interrupts](https://code.google.com/p/arduino-pcimanager/)
* [REST interface for reading out sensor information](https://github.com/sirleech/Webduino)
* [Use bandgap to detect low battery](http://jeelabs.org/2012/05/04/measuring-vcc-via-the-bandgap/)
* [Article about power saving](http://www.gammon.com.au/forum/?id=11497)
* [Self sufficient arduino board with solar panel](http://www.instructables.com/id/Self-Sufficient-Arduino-Board/)