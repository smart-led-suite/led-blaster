
#include "fade.hpp"
#include "led-blaster-pre.hpp"
#include "config.h"
#include <iostream>
#include <cstdlib>
#include <pigpio.h>
#include <stdio.h>
#include <pthread.h>
#include <stdint.h> //libary which includes uint8_t etc.

/*
//**************************************FADE********************
//fade LEDs successively (nacheinander) in wrgb order
void fadeSuccessively(uint16_t time) 
{
	int totalSteps = 0; //total number of steps (used to calculate the delaytime)
	int delayUs = FADE_DELAY_US;
	
	for (int color = 0; color < COLORS; color++) //for every color we have to know the number of steps
	{
		int brightnessDifference = 0;
		if (targetBrightness[color] < currentBrightness[color])				// check which of the two is bigger so we won't get a 'negative difference'
		{
			brightnessDifference = currentBrightness[color] - targetBrightness[color]; //if target is smaller than current, we need to subtract current from target
		}
		if (targetBrightness[color] > currentBrightness[color])		
		{
			brightnessDifference = targetBrightness[color] - currentBrightness[color]; //same as above but vice versa
		}		
		totalSteps += brightnessDifference;		//add difference to totalSteps because this is successive fading
	}
	
	if (totalSteps > 0)
	{
		//steps * delayUs * 1000 = time [in ms] 
		//time is the variable which sets the time needed to fade
		//the calculation is always correct because totalSteps MUST be non-zero
		if (time > 0)
		{
			delayUs = time * 1000 / totalSteps; //calculate delay in us
		}
		else
		{
			delayUs = FADE_DELAY_US; //variable specified in config.h; default is 1000
		}
		
	}		
	for (int color = 0; color < COLORS; color++)  //color after color in wrgb order
	{
		for (currentBrightness[color]; currentBrightness[color] < targetBrightness[color]; currentBrightness[color]++)  //as long as the target is greater than current, we want to increase the brightness
		{
			gpioPWM(pins[color], currentBrightness[color]); //write the new current brightness to pin
			gpioDelay(delayUs);				// and make a small delay
		}
		for (currentBrightness[color]; currentBrightness[color] > targetBrightness[color]; currentBrightness[color]--)  //and vice versa
		{
			gpioPWM(pins[color], currentBrightness[color]);
			gpioDelay(delayUs);
		}
		gpioPWM(pins[color], targetBrightness[color]); //to make sure everything is at the right brightness
	}
}
*/


//fade Directly (basically NOFADE)
void fadeDirectly(uint16_t targetBrightness[]) 
{
	for (int color = 0; color < COLORS; color++) //for every color
	{		
		gpioPWM(pins[color], targetBrightness[color]); //set pin of each color to its target brightness
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
	
	startTime = gpioTick();
	for (int color = 0; color < COLORS; color++) //for every color we have to know the number of steps
	{
		if (targetBrightness[color] < currentBrightness[color])				// check which of the two is bigger so we won't get a 'negative difference'
		{
			brightnessDifference = currentBrightness[color] - targetBrightness[color]; //if target is smaller than current, we need to subtract current from target
		}
		if (targetBrightness[color] > currentBrightness[color])		
		{
			brightnessDifference = targetBrightness[color] - currentBrightness[color]; //same as above but vice versa
		}		
		if (brightnessDifference > totalSteps) //if the difference is bigger than the current difference we'll save it
		{
			totalSteps = brightnessDifference;
		}
	}	


	for (int step = 0; step < realPWMrange; step++) 			//goes step-by-step; 1000 steps right now.
	{
		for (int color = 0; color < COLORS; color++) 			// for each individual color at each individual step
		{
			if (currentBrightness[color] < targetBrightness [color]) //if brightness has to be increased (current is lower than target)
			{
				currentBrightness[color]++;			//increase currentBrightness of that color
				gpioPWM(pins[color], currentBrightness[color]); //write updated currentBrightness to pin
				brightnessWasChanged = true;			//as the brightness was changed, we'll want to make a delay after updating all colors
			}
			else if (currentBrightness[color] > targetBrightness [color])	//if brightness has to be decreased (current is higher than target)
			{
				currentBrightness[color]--;			//decrease currentBrightness of that color
				gpioPWM(pins[color], currentBrightness[color]);	//write updated currentBrightness to pin
				brightnessWasChanged = true;			//as the brightness was changed, we'll want to make a delay after updating all colors
			}
		}
		if (brightnessWasChanged)					//if the brightness was changed we want to make a delay in order to actually SEE the fade ;-)
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
	fadeDirectly(targetBrightness); 					//to make sure everything is at the right brightness
	endTime = gpioTick(); //time needed for the fade
	#ifdef DEBUG
		printf("time variable for fade: %d \n", time);
		printf("real time needed for fade: %d \n", (endTime - startTime) / 1000); //end-start gives elapsed time in micros; divided by 1000 we have it in millis to compare
	#endif
}

void turnLedsOff(uint32_t time) 
{
	for (int color = 0; color < COLORS; color++)  {  //set targetBrightness of all colors to 0
		targetBrightness[color] = 0;	
	}
	fadeSimultaneous(time); //fade leds in time to off.
}
