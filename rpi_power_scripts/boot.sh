#!/bin/bash

PATH=/usr/bin:/usr/local/bin

gpio -g mode 23 in
gpio -g mode 24 out
gpio -g mode 25 out

gpio -g write 24 1
gpio -g write 25 0
