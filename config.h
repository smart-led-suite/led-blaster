#ifndef _config_h
#define _config_h

/* USEFUL DEVELOPER INFO

compiler params:

g++ -std=c++0x -g -o led-blaster-pre led-blaster-pre.cpp fade.cpp file.cpp init.cpp modes.cpp led.cpp -lpigpio -lrt -lpthread
YOU CAN ALSO USE THE MAKEFILE! (sudo make)
// std=c++0x sets the compiler to use the new c++11 standard which we use for the maps.

EVERY NEW MODE WHICH USES A WHILE LOOP is should stop it's loop when his mode is no longer active (mode is therefore a global variable)

A client program should ALWAYS change mode to 0 if it wants to adjust the individual brightness, otherwise the leds will flicker and won't behave as expected!


*/

//************************* DEFINE PROGRAM OPTIONS (comment to disable unless otherwise noted)******************************+

//#define DEBUG //define DEBUG information

#define PWM_CONFIG 0  //0 to config via #define; 1 to config with cli

#define FIFO_FILE	"/dev/led-blaster"

//CLI fade is the only possible option now and is always active!!!
#define CLI_FADE //set variables via cli (if ndef its made via input file inputData.txt

//#define DETAILED_PIN_INIT_INFORMATION //detailed info in pin init

#define FADE_TIME_MS 1000 //default value of fadeDelayMs
#define FADE_DELAY_US 0 //default value of fadeDelayUs
#define SIMULTANEOUS_DELAY_FACTOR 3 //factor to multiply the delay in simultaneous fade mode (useful?)

#define COLORS 4 //number of different leds, currently wrgb, maybe soon w/ autodetect

//if activated, you can i.e. adjust the speed variable without restarting mode 1 etc.
#define MODE_LIVE_MANIPULATING  //according to the pthread documentation this is NOT recommended. we may delete this option in the future if it causes problems.

#define SIGINT_PIBLASTER_TERMINATE_TIME_VALUE 1000 //time value which is send to fade function if sigint is detected (i.e. because of ctrl+c)
#define SIGTERM_PIBLASTER_TERMINATE_FAST_TIME_VALUE 100 //time value which is send to fade function if sigterm is detected (i.e. because of kill or killall command)

//********************************ADVANCED OPTIONS****************************************************************

/************* PWM CONFIG ***********************
 default sample rate is 5 -> 5us per step
 pwm real range (n of steps) is ((1/PWM_FREQUENCY) * 1'000'000) / SAMPLE_RATE
 examples: 	FREQUENCY=200Hz,	T=5000us, 	RANGE=1000
		FREQUENCY=500Hz, 	T=2000us, 	RANGE=400
200hz and 1000steps is probably a good setting
alternative: 255 steps (=8bit-rgb mixing)
		2550 steps (=8-bit w/better fade) BUT low frequency or change sample rate (currently not possible)
 some more detailed infos for setting the pwm range of each pin (see the initPin function)
	a sample rate of 5 (default) means that one step can be at least 5us "long"
	so if you use a frequency of 200Hz there will be 1000 real steps
	Calculations:
		1/200s is one cycle = 0,005s which is 5ms (Millis)
		1ms equals 1000us (Microseconeds)
		so within 5ms there are 1000 5us steps possible
		-> if you increase the frequency you have to decrease the range
	as far as I understand its realized like that in the pigpio libary:
	you set the frequency to whatever you like (w/ SetPWMFrequency)
	you set the pwm_range to whatever you like (w/ SetPWMRange)
	now you can check the pwmSteps to see how many steps you actually have
	(i.e. if you set your frequency to 200Hz and the range to 5000 this will return 1000 (see calculations above))
	BUT if I understand it correctly it uses the range you set (5000 in this example) so 5000 is the maximum brightness BUT
	between 0 and 4 there's no real step!
	this is the difference between the realRange and the range. In my opinion it's good if both have the same value (which is true for 200Hz and 1000steps)
	*/
#define PWM_RANGE 1000
#define PWM_FREQUENCY 200





#endif //config.h
