# atxpi #

Raspberry Pi ATX power controller using the [teensy](http://www.pjrc.com/teensy/) microcontroller. The atxpi emulates ACPI
behavior, handling ATX power supply states, power / reset buttons, and LEDs.

## Usage ##

### Switches ###

* Power state 0:
	* Power button pressed: power on
	* Reset button pressed: do nothing

* Power state 1:
	* Power button pressed for 1 second: Tell RPI to shutdown by setting GPIO pin 23 HIGH
	* Power button pressed for 5 seconds: Force power off
	* Reset button pressed for 1 second: Force power cycle

### GPIO pins ###
The RPI will monitor GPIO 23 and shutdown if it is set to HIGH. After the RPI boots it will set GPIO 24 HIGH, telling the
teensy that the RPI is up. When GPIO 24 goes from HIGH to LOW the teensy will wait 5 seconds then cut power, unless GPIO
25 is HIGH. GPIO 25 is set to HIGH when the RPI is rebooting, preventing the teensy from cutting power power when GPIO 24
changes from HIGH to LOW.

## RPI Power Scripts Installation ##
(Working on it)

## Components ##
* [Teensy 2.0](http://www.pjrc.com/store/teensy.html)
* [Logic Level Converter](https://www.sparkfun.com/products/11978)
* [ATX Connector Breakout Board](https://www.sparkfun.com/products/9558)
* [ATX Power Supply Connector](https://www.sparkfun.com/products/9498)

## Schematic ##
![atxpi schematic](https://raw.github.com/steelcaverobotics/atxpi/gh-pages/images/atxpi_schematic.png)
