import RPi.GPIO as g
import sys

sensor_pin = 4
led_pin = 17

g.setmode(g.BCM)

g.setup(sensor_pin, g.IN)
g.setup(led_pin, g.OUT)

# pi-blaster
#piblaster = open("/dev/pi-blaster", "w")
#sys.stdout = piblaster

while True:
	if g.input(sensor_pin) == 1:
		g.output(led_pin, 1)
		#print str(led_pin) + "=1"
	else:
		g.output(led_pin, 0)
		#print str(led_pin) + "=0"
