#!/usr/bin/env python
import os
import sys

pin=17   # pin of the white leds
pin_r = 17 # red leds
pin_g = 18 # green leds
pin_b = 22 # blue leds

speedfactor=2

actualLuminance=0
targetLuminance=1000
printLuminance=0

linear=0
experimental=0
exp2factor=3

fade=0

schrittweite=1
steps=1000
stepwidth=1

standard_output = sys.stdout

piblaster = open("/dev/pi-blaster", "w")

def switch_leds(pin, pwm_value):
	sys.stdout = piblaster # set stdout so that the command is sent to /dev/pi-blaster
	print str(pin) + "=" + str(pwm_value)
	sys.stdout = standard_output # set stdout to the original value so that we can debug

if fade:
	while actualLuminance < targetLuminance:
		if experimental==1:
			stepwidth = float(steps) / (targetLuminance - actualLuminance)	
		if experimental==2:
			stepwidth = float(steps) / ((targetLuminance - actualLuminance)*exp2factor)		
		nextLuminance = actualLuminance + stepwidth	
		printLuminance = float(nextLuminance)/steps
		switch_leds(pin, printLuminance)
		actualLuminance = nextLuminance

print str(pin) + "=" + str(float(targetLuminance)/steps)
