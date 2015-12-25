
#include "fade.hpp"
#include "led.hpp"
#include "led-blaster-pre.hpp"
#include "config.h"
#include <vector>
#include <iostream>
#include <cstdlib>
#include <pigpio.h>
#include <stdio.h>
#include <pthread.h>
#include <stdint.h> //libary which includes uint8_t etc.
#include <utility>


//fade Directly (basically NOFADE)
void fadeDirectly(void)
{
	for (size_t ledsAvailable = 0; ledsAvailable < leds.size(); ledsAvailable++) {
		gpioPWM(leds[ledsAvailable].getPin(), leds[ledsAvailable].getTargetBrightness());
	}
}

//fade LEDs as simultaneously as possible (without too many complicated algorithms)
void fadeSimultaneous(uint32_t time)
{

	int totalSteps = 0; //total number of steps (used to calculate the delaytime)
	int brightnessDifference = 0;
	int delayUs = 0;
	uint32_t startTime = 0; //time to check if theres any overhead
	uint32_t endTime = 0; //time to check if theres any overhead
	bool brightnessWasChanged = false; 					// variable to check if some brightness was changed to see if we need to make a delay

	startTime = gpioTick(); //save start time so we can see how fast the function really is



	//for every color we have to know the number of steps
	for (size_t ledsAvailable = 0; ledsAvailable < leds.size(); ledsAvailable++)
	{
		// check which of the two is bigger so we won't get a 'negative difference'
		if ( leds[ledsAvailable].getTargetBrightness() <  leds[ledsAvailable].getCurrentBrightness())
		{
			//if target is smaller than current, we need to subtract current from target
			brightnessDifference =  leds[ledsAvailable].getCurrentBrightness() -  leds[ledsAvailable].getTargetBrightness();

		}
		if ( leds[ledsAvailable].getTargetBrightness() >  leds[ledsAvailable].getCurrentBrightness())
		{
			//same as above but vice versa
			brightnessDifference =  leds[ledsAvailable].getTargetBrightness() -  leds[ledsAvailable].getCurrentBrightness();
		}
		//if the difference is bigger than the current difference we'll save it
		if (brightnessDifference > totalSteps)
		{
			totalSteps = brightnessDifference;
		}
	}

	//goes step-by-step; 1000 steps right now.
	for (int step = 0; step < realPWMrange; step++)
	{
		// for each individual color at each individual step
		//using the object vector
		for (size_t ledsAvailable = 0; ledsAvailable < leds.size(); ledsAvailable++)
		{

			//if brightness has to be increased (current is lower than target)
			if (leds[ledsAvailable].getCurrentBrightness() < leds[ledsAvailable].getTargetBrightness())
			{
				//increase currentBRIGHTNESS of that color
				leds[ledsAvailable].setCurrentBrightness( leds[ledsAvailable].getCurrentBrightness() + 1);
				//write updated brightness to pin
				gpioPWM(leds[ledsAvailable].getPin(), leds[ledsAvailable].getCurrentBrightness());
				//as the brightness was changed, we'll want to make a delay after updating all colors
				brightnessWasChanged = true;
			}
			//if brightness has to be decreased (current is higher than target)
			else if (leds[ledsAvailable].getCurrentBrightness() > leds[ledsAvailable].getTargetBrightness())
			{
				//decrease brightness of that color
				leds[ledsAvailable].setCurrentBrightness( leds[ledsAvailable].getCurrentBrightness() - 1);
				//write updated brightness to pin
				gpioPWM(leds[ledsAvailable].getPin(), leds[ledsAvailable].getCurrentBrightness());
				//as the brightness was changed, we'll want to make a delay after updating all colors
				brightnessWasChanged = true;
			}
		}
		//if the brightness was changed we want to make a delay in order to actually SEE the fade ;-)
		if (brightnessWasChanged)
		{
			//calculate the delayUs needed to archieve the specified time


			//steps * delayUs * 1000 = time [in ms]
			//time is the variable which sets the time needed to fade
			//the calculation is always correct because if brightnessWasChanged is true the totalSteps MUST be non-zero
			if (time > 0)
			{
				delayUs = time * 1000 / totalSteps; //calculate delay in us
			}
			else
			{
				delayUs = FADE_DELAY_US; //variable specified in config.h; default is 1000
			}

			gpioDelay(delayUs); 		//make a delay
			brightnessWasChanged = false;				//set value to false again so it won't make a delay every time :)
		}
	}
	fadeDirectly(); 					//to make sure everything is at the right brightness
	endTime = gpioTick(); //time needed for the fade
	#ifdef DEBUG
		printf("time variable for fade: %d \n", time);
		printf("real time needed for fade: %d \n", (endTime - startTime) / 1000); //end-start gives elapsed time in micros; divided by 1000 we have it in millis to compare
	#endif
}

void turnLedsOff(uint32_t time)
{
	//set targetBrightness of all colors to 0
	for (size_t ledsAvailable = 0; ledsAvailable < leds.size(); ledsAvailable++)
	{
		leds[ledsAvailable].setTargetBrightness(0);
	}
	fadeSimultaneous(time); //fade leds in time to off.
}
