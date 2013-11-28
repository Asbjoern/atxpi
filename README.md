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
25 is HIGH. GPIO 25 is set to HIGH when the RPI is rebooting, preventing the teensy from cutting power when GPIO 24
changes from HIGH to LOW.

## RPI Power Scripts Installation (Arch) ##
Log into the RPI and become root. Then clone the repo and copy required files.

    [root@alarmpi ~]$ git clone https://github.com/steelcaverobotics/atxpi.git
    [root@alarmpi ~]$ mkdir /opt/atxpi
    [root@alarmpi ~]$ cp atxpi/rpi_power_scripts/*.sh /opt/atxpi/
    [root@alarmpi ~]$ chmod 755 /opt/atxpi/*.sh
    [root@alarmpi ~]$ cp atxpi/service_files/arch/*.service /usr/lib/systemd/system/

Then enable the services, skipping `atxpi-gpio-shutdown.service` if you don't want the RPI to shutdown when the power button is pressed.

    [root@alarmpi ~]$ systemctl enable atxpi-gpio-boot.service
    [root@alarmpi ~]$ systemctl enable atxpi-gpio-reboot.service
    [root@alarmpi ~]$ systemctl enable atxpi-gpio-shutdown.service

Finally, start the services. Do not start `atxpi-gpio-reboot.service`, as this will be started when the RPI is rebooting.

    [root@alarmpi ~]$ systemctl start atxpi-gpio-boot.service
    [root@alarmpi ~]$ systemctl start atxpi-gpio-shutdown.service

## RPI Power Scripts Installation (Raspbian) ##
(Working on it)

## Components ##
* [Teensy 2.0](http://www.pjrc.com/store/teensy.html)
* [Logic Level Converter](https://www.sparkfun.com/products/11978)
* [ATX Connector Breakout Board](https://www.sparkfun.com/products/9558)
* [ATX Power Supply Connector](https://www.sparkfun.com/products/9498)

## Schematic ##
![atxpi schematic](https://raw.github.com/steelcaverobotics/atxpi/gh-pages/images/atxpi_schematic.png)
