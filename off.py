#!/usr/bin/env python
import os
#i="Hallo"
#print i
fade=1
luminance=float(100.0)
steps=500
if fade:
	for i  in range(1000,0,-1):
		luminance = float(i)/steps
		test = os.system("echo 27=" + str(luminance) + "  > /dev/pi-blaster")
	test = os.system("echo " "27=0." "  > /dev/pi-blaster")


