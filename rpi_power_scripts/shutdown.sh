#!/bin/bash

PATH=/bin:/sbin:/usr/bin:/usr/local/bin

gpio -g mode 23 in

while [ 1 ]; do
	if [ $(gpio -g read 23) = "1" ]; then
		poweroff
		break
	fi

	sleep 0.5
done
