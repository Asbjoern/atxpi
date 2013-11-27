#!/bin/bash

PATH=/usr/bin:/usr/local/bin

gpio -g mode 24 out
gpio -g write 25 1
