# atxpi #

Raspberry Pi ATX power controller using the [teensy](http://www.pjrc.com/teensy/) microcontroller. The atxpi emulates ACPI behavior, handling ATX power supply states, power / reset buttons, and LEDs.

---

 - [Parts List](#parts-list)
 - [Power / Reset Buttons](#buttons)
 - [GPIO Pins](#gpio)
 - [RPI Power Scripts Installation](#power-scripts)
	- [Arch](#power-scripts-arch)
	- [Raspbian](#power-scripts-raspbian)
 - [Schematic](#schematic)

---

<h2 id="parts-list">Parts List</h2>

 - [Teensy 2.0](http://www.pjrc.com/store/teensy.html)
 - [Logic Level Converter](https://www.sparkfun.com/products/11978)
 - [ATX Connector Breakout Board](https://www.sparkfun.com/products/9558)
 - [ATX Power Supply Connector](https://www.sparkfun.com/products/9498)

---

<h2 id="buttons">Power / Reset Buttons</h2>

 - Power state 0:
	- Power button pressed: power on
	- Reset button pressed: do nothing

 - Power state 1:
	- Power button pressed for 1 second: Tell RPI to shutdown by setting GPIO pin 23 HIGH
	- Power button pressed for 5 seconds: Force power off
	- Reset button pressed for 1 second: Force power cycle

---

<h2 id="gpio">GPIO Pins</h2>

The RPI will monitor GPIO 23 and shutdown if it is set to HIGH. After the RPI boots it will set GPIO 24 HIGH, telling the teensy that the RPI is up. When GPIO 24 goes from HIGH to LOW the teensy will wait 5 seconds then cut power, unless GPIO 25 is HIGH. GPIO 25 is set to HIGH when the RPI is rebooting, preventing the teensy from cutting power when GPIO 24 changes from HIGH to LOW.

---

<h2 id="power-scripts">RPI Power Scripts Installation</h2>

<h3 id="power-scripts-arch">Arch</h3>

Log into the RPI and become root. You will need to install git and WiringPi:

	[root@alarmpi ~]$ pacman -Sy git wiringpi

Next, clone the repo and copy required files.

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

<h3 id="power-scripts-raspbian">Raspbian</h3>

Log into the RPI and become root. You will need to install git and WiringPi:

	root@raspberrypi:~# apt-get install git-core
	root@raspberrypi:~# git clone git://git.drogon.net/wiringPi
	root@raspberrypi:~# cd wiringPi/
	root@raspberrypi:~# ./build

Next, clone the repo and copy required files.

	root@raspberrypi:~# git clone https://github.com/steelcaverobotics/atxpi.git
	root@raspberrypi:~# mkdir /opt/atxpi
	root@raspberrypi:~# cp atxpi/rpi_power_scripts/*.sh /opt/atxpi/
	root@raspberrypi:~# chmod 755 /opt/atxpi/*.sh
	root@raspberrypi:~# cp atxpi/service_files/raspbian/* /etc/init.d/
	root@raspberrypi:~# chmod 755 /etc/init.d/atxpi-gpio-*

Then enable the services, skipping `atxpi-gpio-shutdown` if you don't want the RPI to shutdown when the power button is pressed.

	root@raspberrypi:~# update-rc.d atxpi-gpio-boot defaults
	root@raspberrypi:~# update-rc.d atxpi-gpio-reboot defaults
	root@raspberrypi:~# update-rc.d atxpi-gpio-shutdown defaults

Finally, start the services.

	root@raspberrypi:~# /etc/init.d/atxpi-gpio-boot start
	root@raspberrypi:~# /etc/init.d/atxpi-gpio-shutdown start

---

<h2 id="schematic">Schematic</h2>

<a href="https://dl.dropboxusercontent.com/u/7536257/atxpi/atxpi_schematic_large.png" target="_blank">
	<img alt="atxpi schematic" src="https://dl.dropboxusercontent.com/u/7536257/atxpi/atxpi_schematic_small.png" border="0" />
</a>
