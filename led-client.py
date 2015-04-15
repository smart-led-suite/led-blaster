#!/usr/bin/env python
# -*- coding: utf-8 -*-
import os
import sys
import time
import pickle
import led
import subprocess

#           r    g   b
pins_rgb = [17, 18, 22] # red, green, blue pins
pins_white = [25]
pins = [] # pins list to work with later


white = 2  # set to 1 for only white, to 0 for only rgb and to 2 for both

experimental=1
exp1factor=0.005
exp2factor=3
firststepwidth=0.001 #stepwidth at currentLuminance=0 in exp1
previousStepwidth=0

if(white == 1):
	pins = pins_white
elif(white == 0):
	pins = pins_rgb
elif(white == 2):
	pins = pins_white
	pins.extend(pins_rgb)
speedfactor=2


linear=0
#experimental=1

# check if function.py is running

#p = subprocess.Popen("ps aux | grep \"python function.py\"", stdout=subprocess.PIPE, shell=True)
#(output, err) = p.communicate()

#print output

#if int(output) < 2:
#	subprocess.call("killall \"python") 

print "hallo"
args = sys.argv # get the arguments php gave us when calling the script
fade = args[1] 
speed = args[2]
print speed
# targeted luminance: white,  red,    green,    blue
targetLuminances = {} # empty dictionary, will be filled right next:
for i in range(0, len(pins)):
	targetLuminances[pins[i]] = (int(args[i + 3]) * 10) # we have to add 3 because pins start with the third argument (1st is name of script, 2nd fade)

steps=1000
stepwidth=1

standard_output = sys.stdout

piblaster = open("/dev/pi-blaster", "w")

def switch_leds(pin, pwm_value):
	sys.stdout = piblaster # set stdout so that the command is sent to /dev/pi-blaster
	sys.stdout = standard_output # set stdout to the original value so that we can debug

# turn all leds off
#for pinNr in range(len(pins)):
#	switch_leds(pins[pinNr], 0)

led.leds(fade, speed, targetLuminances[pins[0]], targetLuminances[pins[1]], targetLuminances[pins[2]], targetLuminances[pins[3]])


