#include "init.hpp"
#include "led-blaster-pre.hpp"
#include "config.h"

#include <iostream>
#include <cstdlib>
#include <pigpio.h>
#include <stdio.h>
#include <pthread.h>
using namespace std;
//*********************************general INIT*******************************************
bool initGeneral(void) 
{	
	gpioTerminate(); //shut down all DMA channels and stuff so we can start fresh and easy ;-)
	
	if (gpioInitialise() < 0) //initializes the gpio libary
	{
	   cout << "pigpio initialisation failed." << endl;
	   return 1;
	}
	else
	{
	   cout << "pigpio initialised okay. initGeneral successful." << endl;
	   return 0;
	}
}


//***************************PINS INIT******************************************************


bool initPin(int pin, uint16_t mode) 
{
	#if PWM_CONFIG
		int realRange = 0;
		int setRange = 0;
		int targetRange = 0;
		
		int targetFrequency = 0;
		int frequency = 0;
	
		cout << "enter target frequency :" << endl;
		cin >> targetFrequency;
		cout << "set frequency to: " << targetFrequency << endl;
		frequency = gpioSetPWMfrequency(pin, targetFrequency);
		cout << "range was set to closest frequency: " << frequency << endl;
		cout << "check frequency: " << gpioGetPWMfrequency(pin) << endl;	
		
		cout << "enter target range :" << endl;
		cin >> targetRange;
		cout << "set range to: " << targetRange << endl;
		realRange = gpioSetPWMrange(pin, targetRange);
		cout << "range was set to real range: " << realRange << endl;
		cout << "check range: " << gpioGetPWMrange(pin) << endl;
	#endif		
	
	
	if (gpioSetMode(pin, 1)) //set pin to output (1), returns 0 if it went ok
	{  
		cout << "error while setting " << pin << " to output" << endl;
		return 1;
	}
	
	/**************INFOS ABOUT PWM FREQUENCY AND RANGE*******************************
	some infos for setting the pwm range of each pin
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
	now you can check the realPWMRange to see how many steps you actually have 
	(i.e. if you set your frequency to 200Hz and the range to 5000 this will
	return 1000 (see calculations above))
	BUT if I understand it correctly it uses the range you set (5000 in this example)
	so 5000 is the maximum brightness BUT between 0 and 4 there's no real step! 
	this is the difference between the realRange and the range. In my opinion 
	it's good if both have the same value (which is true for 200Hz and 1000steps) 
	********************************************************************************
	*/

	#ifdef DETAILED_PIN_INIT_INFORMATION
		cout << "set " << pin << " to output" << endl;
	#endif
	gpioSetPWMfrequency(pin, PWM_FREQUENCY);
	#ifdef DETAILED_PIN_INIT_INFORMATION
		cout << "check frequency: " << gpioGetPWMfrequency(pin) << endl;
	#endif
	gpioSetPWMrange(pin, PWM_RANGE);
	realPWMrange = gpioGetPWMrealRange(pin);
	#ifdef DETAILED_PIN_INIT_INFORMATION
		cout << "check real pwm range: " << realPWMrange << endl;
	#endif
	PWMrange = gpioGetPWMrange(pin);
	#ifdef DETAILED_PIN_INIT_INFORMATION
		cout << "check pwm range: " << PWMrange << endl;
	#endif
	
		
	gpioPWM(pin, 0); //set brightness to 0
	cout << "pwm set to 0 correctly" << endl;
	
	cout << "init of pin " << pin << " finished successfully." << endl;
	return 0;
}


