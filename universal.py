#!/usr/bin/env python

import os

#i="Hallo"
#print i
pin=27

speedfactor=2

actualLuminance=0
targetLuminance=1000
printLuminance=0

linear=0
experimental=2
exp2factor=3

fade=1

schrittweite=1
steps=1000
stepwidth=1

if fade:
	while actualLuminance < targetLuminance:
		if experimental==1:
			stepwidth = float(steps) / (targetLuminance - actualLuminance)	
		if experimental==2:
			stepwidth = float(steps) / ((targetLuminance - actualLuminance)*exp2factor)
#		stepwidth = stepwidth * speedfactor
#		nextLuminance = actualLuminance + stepwidth			
		nextLuminance = actualLuminance + stepwidth	
		printLuminance = float(nextLuminance)/steps
		test = os.system("echo " + str(pin) + "=" + str(printLuminance) + "  > /dev/pi-blaster")
		actualLuminance = nextLuminance
		print stepwidth

	#for i in range(steps/2,steps,schrittweite*4)	
#		luminance = float(i)/steps
#		test = os.system("echo 27=" + str(luminance) + " > /dev/pi-blaster")
#	test = os.system("echo " "27=1" "  > /dev/pi-blaster")
