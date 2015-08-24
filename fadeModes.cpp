
#include "fadeModes.hpp"
#include "led-blaster-pre.hpp"
#include "config.h"
#include <iostream>
#include <cstdlib>
#include <pigpio.h>
#include <stdio.h>
#include <pthread.h>
#include <stdint.h> //libary which includes uint8_t etc.


//**************************************FADE********************
//fade LEDs successively (nacheinander) in wrgb order
void fadeSuccessively(uint16_t delay, uint16_t targetBrightness[]) 
{
	for (int color = 0; color < COLORS; color++) 
	{
		for (currentBrightness[color]; currentBrightness[color] < targetBrightness[color]; currentBrightness[color]++) 
		{
			gpioPWM(pins[color], currentBrightness[color]);
			gpioDelay(delay);
		}
		for (currentBrightness[color]; currentBrightness[color] > targetBrightness[color]; currentBrightness[color]--) 
		{
			gpioPWM(pins[color], currentBrightness[color]);
			gpioDelay(delay);
		}
		gpioPWM(pins[color], targetBrightness[color]); //to make sure everything is at the right brightness
	}
}

//fade Directly (basically NOFADE)
void fadeDirectly(uint16_t targetBrightness[]) 
{
	for (int color = 0; color < COLORS; color++) //for every color
	{		
		gpioPWM(pins[color], targetBrightness[color]); //set pin of each color to its target brightness
	}
}

//fade LEDs as simultaneously as possible (without too many complicated algorithms)
void fadeSimultaneous(uint16_t delay, uint16_t targetBrightness[])
{	
	bool brightnessWasChanged = false; 					// variable to check if some brightness was changed to see if we need to make a delay
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
			//as there are less steps compared to successiveFade we multiply the normal delay by a factor
			//I'm not sure if that makes sense so maybe its will be changed in the future
			gpioDelay(delay*SIMULTANEOUS_DELAY_FACTOR); 		//make a delay
			brightnessWasChanged = false;				//set value to false again so it won't make a delay every time :)
		}	
	}
	fadeDirectly(targetBrightness); 					//to make sure everything is at the right brightness
}
