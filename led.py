#!/usr/bin/env python3
# -*- coding: utf-8 -*-
import os
import sys
import time
import pickle
import RPi.GPIO as g

#           r    g   b
pins_rgb = [17, 18, 22] # red, green, blue pins
pins_white = [25]
pins = [] # pins map to work with later

pwms = {} # holds the pwm objects

white = 2  # set to 1 for only white, to 0 for only rgb and to 2 for both

experimental=1
exp1factor=0.01
exp2factor=3
firststepwidth=0.001 #stepwidth at currentLuminance=0 in exp1
verschiebungY=1

if(white == 1):
	pins = pins_white
elif(white == 0):
	pins = pins_rgb
elif(white == 2):
	pins = pins_white
	pins.extend(pins_rgb)

print pins
# setup PWM
#g.setmode(g.BCM)
#for pin in pins:
#	g.setup(pin, g.OUT)
#	pwms[pin] = g.PWM(pin, 1000) # 1kHz
#	print "set pin " + str(pin) + " to pwm"


speedfactor=2

currentLuminances = {}
filename = "/tmp/luminances.p"
try:
	currentLuminances = pickle.load(open(filename, "rb"))
	if(len(currentLuminances) == 0):
		raise Exception("currentLuminances must have at least one value")
except Exception:
	for pin in range(len(pins)): # create file if it doesn't exist
		currentLuminances[pins[pin]] = 0
	pickle.dump(currentLuminances, open(filename, "wb"))


linear=0
#experimental=1
steps=1000


standard_output = sys.stdout



def switch_leds(pin, pwm_value):
	piblaster = open("/dev/pi-blaster", "w")
	sys.stdout = piblaster # set stdout so that the command is sent to /dev/pi-blaster
	print str(pin) + "=" + str(pwm_value)
	sys.stdout = standard_output # set stdout to the original value so that we can debug
	piblaster.close()
	#print "val: " + str(pwm_value * 100)
	#if(pwm_value == 0):
	#	pwms[pin].stop()
	#else:
	#	pwms[pin].start(pwm_value * 100)

# turn all leds off
#for pinNr in range(len(pins)):
#	switch_leds(pins[pinNr], 0)


def leds(fade, speed, w, r, g, b):
	print speed
	#exp1factor = 0.01 #default
	speed = int(speed)
	if speed < 50 and speed != 0:  #korrekte werte, experimentell herausgefunden
		exp1factor=float(speed)/1000
	else:
		exp1factor = 0.01 #default
	print exp1factor		
	targetLuminances = [w,r,g,b]
	stepwidth = 1
	if int(fade) == 1: # Fade-Level 1: nacheinander Faden
		for color in range(0, len(pins)):
			##
			colorPin = pins[color] # pin for the current color
			colorTargetLuminance = float(targetLuminances[color]) # targetLuminance for the current color
			currentLuminance = currentLuminances[colorPin]
			if int(colorTargetLuminance) == -10:
				continue # abbrechen. -1 (* 10) ist Zeichen, um nichts zu verändern
			##
			if currentLuminance < colorTargetLuminance: # hochfaden
				while currentLuminance < colorTargetLuminance:
					if experimental==1:
						#exp1factor=float(speed)/1000
						stepwidth = float(currentLuminance-currentLuminances[colorPin]) * float(exp1factor)	
					if stepwidth < 0.005:
						stepwidth = stepwidth + 0.001
					stepwidth += verschiebungY
					if experimental==2:
						stepwidth = float(steps) / ((colorTargetLuminance - currentLuminance)*exp2factor)		
					nextLuminance = currentLuminance + stepwidth
					if(nextLuminance > colorTargetLuminance):
						nextLuminance = colorTargetLuminance
					printLuminance = float(nextLuminance)/steps
					switch_leds(colorPin, printLuminance)
					currentLuminance = nextLuminance
			elif currentLuminance > colorTargetLuminance: # runterfaden
				while currentLuminance > colorTargetLuminance:
					if experimental==1:
						#exp1factor=(float(speed)/1000)-0.01
						stepwidth = float(currentLuminance) * float(exp1factor)	
					stepwidth += verschiebungY
					if experimental==2:
						stepwidth = float(steps) / ((currentLuminance - colorTargetLuminance)*exp2factor)			
					nextLuminance = currentLuminance - stepwidth
					if(nextLuminance < colorTargetLuminance):
						nextLuminance = colorTargetLuminance	
					printLuminance = float(nextLuminance)/steps
					switch_leds(colorPin, printLuminance)
					currentLuminance = nextLuminance
			switch_leds(colorPin, float(colorTargetLuminance) / steps)
			currentLuminance = colorTargetLuminance
			currentLuminances[colorPin] = currentLuminance # update currentLuminance
	elif int(fade) == 2: # Fadel-Level 2: gleichzeitig Faden
		for step in range(0, steps):
			for color in range(0, len(pins)):
				colorPin = pins[color] # pin for the current color
				colorTargetLuminance = float(targetLuminances[color]) # targetLuminance for the current color
				currentLuminance = currentLuminances[colorPin]
	#			print "c: " + str(color) + " target: " + str(colorTargetLuminance) + " current: " + str(currentLuminance)
				if int(colorTargetLuminance) == -10:
					continue # abbrechen. -1 (* 10) ist Zeichen, um nichts zu verändern
				##
				if currentLuminance < colorTargetLuminance: # = hochfaden
					if experimental==1:
						#exp1factor=float(speed)/1000
						stepwidth = float(currentLuminance-currentLuminances[colorPin]) * float(exp1factor)	
					if stepwidth < 0.005:
						stepwidth = stepwidth + 0.001
					stepwidth += verschiebungY
					if experimental==2:
						stepwidth = float(steps) / ((colorTargetLuminance - currentLuminance)*exp2factor)		
					nextLuminance = currentLuminance + stepwidth
				elif currentLuminance > colorTargetLuminance: # = runterfaden
					if experimental==1:
						#exp1factor=(float(speed)/1000)-0.01
						stepwidth = float(currentLuminance) * float(exp1factor)	
					stepwidth += verschiebungY
					if experimental==2:
						stepwidth = float(steps) / ((colorTargetLuminance - currentLuminance)*exp2factor)		
					nextLuminance = currentLuminance - stepwidth
				elif currentLuminance == colorTargetLuminance:
					continue
				switch_leds(colorPin, float(nextLuminance) / steps)
				currentLuminance = nextLuminance
				currentLuminances[colorPin] = currentLuminance # update currentLuminance
			if(targetLuminances == currentLuminances):
				break;
	for color in range(0, len(pins)): # Alle auf TargetLuminance stellen
		##
		colorPin = pins[color] # pin for the current color
		colorTargetLuminance = float(targetLuminances[color]) # targetLuminance for the current color
		currentLuminance = currentLuminances[colorPin]
		if int(colorTargetLuminance) == -10:
			continue # abbrechen. -1 (* 10) ist Zeichen, um nichts zu verändern
		##
		switch_leds(colorPin, float(colorTargetLuminance) / steps)
		currentLuminance = colorTargetLuminance
		currentLuminances[colorPin] = currentLuminance # update currentLuminance
	pickle.dump(currentLuminances, open(filename, "wb")) # and dump them
