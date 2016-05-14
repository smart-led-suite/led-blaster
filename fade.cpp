
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

int fadeSteps = 1000; //1000 as a default (same value as we expect due to the #defines)

int setFadeSteps(int *pwmSteps)
{
  fadeSteps = *pwmSteps;
}

//fade Directly (basically NOFADE)
void fadeDirectly(ledInformationStruct * led)
{
	for (size_t ledsAvailable = 0; ledsAvailable < led->leds.size(); ledsAvailable++)
  {
		gpioPWM(led->leds[ledsAvailable].getPin(), led->leds[ledsAvailable].getTargetBrightness());
	}
}

//fade LEDs as simultaneously as possible (without too many complicated algorithms)
void fadeSimultaneous(ledInformationStruct * led)
{

	int totalSteps = 0; //total number of steps (used to calculate the delaytime)
	int brightnessDifference = 0;
	int delayUs = 0;
	uint32_t startTime = 0; //time to check if theres any overhead
	uint32_t endTime = 0; //time to check if theres any overhead
	bool brightnessWasChanged = false; 					// variable to check if some brightness was changed to see if we need to make a delay

	startTime = gpioTick(); //save start time so we can see how fast the function really is



	//for every color we have to know the number of steps
	for (size_t ledsAvailable = 0; ledsAvailable < led->leds.size(); ledsAvailable++)
	{
		// check which of the two is bigger so we won't get a 'negative difference'
		if ( led->leds[ledsAvailable].getTargetBrightness() <  led->leds[ledsAvailable].getCurrentBrightness())
		{
			//if target is smaller than current, we need to subtract current from target
			brightnessDifference =  led->leds[ledsAvailable].getCurrentBrightness() -  led->leds[ledsAvailable].getTargetBrightness();
		}
		if ( led->leds[ledsAvailable].getTargetBrightness() >  led->leds[ledsAvailable].getCurrentBrightness())
		{
			//same as above but vice versa
			brightnessDifference =  led->leds[ledsAvailable].getTargetBrightness() -  led->leds[ledsAvailable].getCurrentBrightness();
		}
		//if the difference is bigger than the current difference we'll save it
		if (brightnessDifference > totalSteps)
		{
			totalSteps = brightnessDifference;
		}
	}

	//goes step-by-step; 1000 steps right now.
	for (int step = 0; step < led->pwmSteps; step++)
	{
		// for each individual color at each individual step
		//using the object vector
		for (size_t ledsAvailable = 0; ledsAvailable < led->leds.size(); ledsAvailable++)
		{

			//if brightness has to be increased (current is lower than target)
			if (led->leds[ledsAvailable].getCurrentBrightness() < led->leds[ledsAvailable].getTargetBrightness())
			{
				//increase currentBRIGHTNESS of that color and write it to the pin
				led->leds[ledsAvailable].setCurrentBrightness( led->leds[ledsAvailable].getCurrentBrightness() + 1);
				//as the brightness was changed, we'll want to make a delay after updating all colors
				brightnessWasChanged = true;
			}
			//if brightness has to be decreased (current is higher than target)
			else if (led->leds[ledsAvailable].getCurrentBrightness() > led->leds[ledsAvailable].getTargetBrightness())
			{
				//decrease brightness of that color and write it to the pin
				led->leds[ledsAvailable].setCurrentBrightness( led->leds[ledsAvailable].getCurrentBrightness() - 1);

				//as the brightness was changed, we'll want to make a delay after updating all colors
				brightnessWasChanged = true;
			}
		}
		//if the brightness was changed we want to make a delay in order to actually SEE the fade ;-)
		if (brightnessWasChanged)
		{
			//calculate the delayUs needed to archieve the specified fadeTime
			//steps * delayUs * 1000 = fadeTime [in ms]
			//fadeTime is the variable which sets the Time needed to fade
			//the calculation is always correct because if brightnessWasChanged is true the totalSteps MUST be non-zero
			if (led->fadeTime > 0)
			{
				delayUs = led->fadeTime * 1000 / totalSteps; //calculate delay in us
			}
			else
			{
				delayUs = FADE_DELAY_US; //variable specified in config.h; default is 1000
			}

			gpioDelay(delayUs); 		//make a delay
			brightnessWasChanged = false;				//set value to false again so it won't make a delay every time :)
		}
	}
	fadeDirectly(led); 					//to make sure everything is at the right brightness
	endTime = gpioTick(); //time needed for the fade
	#ifdef DEBUG
		printf("time variable for fade: %d \n", time);
		printf("real time needed for fade: %d \n", (endTime - startTime) / 1000); //end-start gives elapsed time in micros; divided by 1000 we have it in millis to compare
	#endif
}

void turnLedsOff(ledInformationStruct * led)
{
	//set targetBrightness of all colors to 0
	for (size_t ledsAvailable = 0; ledsAvailable < led->leds.size(); ledsAvailable++)
	{
		led->leds[ledsAvailable].setTargetBrightness(0);
	}
	fadeSimultaneous(led);//fade leds in time to off.

}
