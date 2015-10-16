# led-blaster

project inspired by pi-blaster and based on pigpio libary (http://abyz.co.uk/rpi/pigpio/) which uses a mix between hw and sw pwm.

the goal is to create a universal code in which you write settings and so on in FIFO style (just like pi-blaster).
the following things are on our toDo-list:

1. assign pins to led colors (i.e. 17 = blue led)
2. enter desired fade mode (linear, exp., no fade)
3. enter target brigthness for each led/for all rgbw leds (the program will then fade according to the fade mode
4. option to connect an sound sensor 
5. some additional modes like continious fade to random color w/ defined speed etc.
6. an alarm clock which fades on all/some leds slowly at a given time (sunrise simulation to wake up easily) (in coordination with some PHP code...)
7. more to come...

#commands

echo them into /dev/led-blaster

r = value  //set redBrightness to value 0...RANGE
g = value  //set greenBrightness to value 0...RANGE
b = value  //set blueBrightness to value 0...RANGE
w = value  //set whiteBrightness to value 0...RANGE

mode = value //choose mode obviously
mode0 = fade to fixed brightness
mode2 = fade to random colors

time = value
select the time (in ms) needed until fade is completed
