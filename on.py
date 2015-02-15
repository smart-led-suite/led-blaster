#!/usr/bin/env python
import os
#i="Hallo"
#print i
fade=1
luminance=float(0.0)
schrittweite=1
steps=500
if fade:
	for i  in range(0,steps,schrittweite):
		luminance = float(i)/steps
		test = os.system("echo 27=" + str(luminance) + "  > /dev/pi-blaster")
#	for i in range(steps/2,steps,schrittweite*4)	
#		luminance = float(i)/steps
#		test = os.system("echo 27=" + str(luminance) + " > /dev/pi-blaster")
	test = os.system("echo " "27=1" "  > /dev/pi-blaster")
	


